//
// Created by raffa on 28/08/2019.
//

#include <iostream>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlRecord>
#include <QVector>

#include "../include/SerialSize.h"
#include "../include/SharedDocument.h"

// This constructor is used in both New Document and Open Document cases.
// collaborators.db, symbols.db, symbols.txt and the html file MUST be read in case of
// pre-existing data on Server!
SharedDocument::SharedDocument(QString file_path, QString folder_path, QObject* parent) : QObject(parent), file_path(file_path), folder_path(folder_path) {
    // Reading from file.
    document = new QTextDocument(this);
    QFile file = QFile(file_path);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug().noquote() << "Warning!\n\t-> Cannot read file: " << file.errorString();
        qDebug() << file_path;
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    document->setPlainText(text);
    file.close();

    // Reading symbols data.
    QString symbols_db_path = QDir::toNativeSeparators(folder_path + "/symbols.db");
    QSqlDatabase symbols_db = QSqlDatabase::addDatabase("QSQLITE", QDir::toNativeSeparators(folder_path + "/symbols.db"));
    symbols_db.setDatabaseName(symbols_db_path);
    symbols_db.open();
    {
        QSqlQuery query = QSqlQuery(symbols_db);
        bool done = query.exec("SELECT * FROM SYMBOLS");
        if (!done) qDebug() << query.lastError();

        while (query.next()) {
            QVariant tmp1 = query.value(0); // CHARACTER
            QVariant tmp2 = query.value(1); // SITE_ID
            QVariant tmp3 = query.value(2); // COUNTER

            QChar ch = tmp1.toString()[0];
            qint32 site_id = tmp2.toInt();
            quint32 cnt = tmp3.toUInt();
            QVector<qint32> pos = QVector<qint32>();

            QString symbols_txt_path = QDir::toNativeSeparators(folder_path + "/symbols.txt");
            QFile symbols_txt(symbols_txt_path);
            if (!symbols_txt.open(QIODevice::ReadOnly | QFile::Text)) {
                qDebug().noquote() << "Warning!\n\t-> Cannot read file: " << file.errorString();
                return;
            }

            QTextStream stream(&symbols_txt);
            QString line = stream.readLine();
            bool finished = false;
            while (!line.isNull() && !finished) {
                QStringList list = line.simplified().split(',');
                qint32 searched_site_id = list[0].trimmed().toInt();
                qint32 searched_counter = list[1].trimmed().toInt();
                if (site_id == searched_site_id && cnt == searched_counter) {
                    for (int i = 2; i < list.size(); i++)
                        pos.push_back(list[i].trimmed().toInt());
                    finished = true;
                }
                line = stream.readLine();
            }
            Symbol s = Symbol(ch, site_id, cnt, pos);
            this->processSymbol(s, Message::INSERT_TYPE, true);
        }
    }
    symbols_db.close();

    connect(this, &SharedDocument::saving, this, &SharedDocument::automaticSave);
}

void SharedDocument::automaticSave() {
    // Saving on file.
    document->setPlainText(symbols_to_string());
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        qDebug().noquote() << "Warning!\n\t-> Cannot save file: " << file.errorString();
        return;
    }
    QTextStream file_out(&file);
    QString text = document->toPlainText();
    file_out << text;
    file.close();

    // Updating symbols.txt.
    QString symbols_txt_path = QDir::toNativeSeparators(folder_path + "/symbols.txt");
    QFile symbols_txt(symbols_txt_path);
    if (!symbols_txt.open(QIODevice::WriteOnly | QFile::Text)) {
        qDebug().noquote() << "Warning!\n\t-> Cannot save file: " << symbols_txt.errorString();
        return;
    }
    QTextStream symbols_out(&symbols_txt);
    QString symbols_positions = this->positions_to_string();
    symbols_out << symbols_positions;
    symbols_txt.close();
}

void SharedDocument::addSharedEditor(QTcpSocket* clientConnection, QString nickname, qint32* site_id, quint32* counter) {
    QString collaborators_db_path = QDir::toNativeSeparators(folder_path + "/collaborators.db");
    QSqlDatabase collaborators_db = QSqlDatabase::addDatabase("QSQLITE", collaborators_db_path);
    collaborators_db.setDatabaseName(collaborators_db_path);
    collaborators_db.open();
    {
        QSqlQuery query(collaborators_db);
        bool done = query.exec("SELECT ROWID, NICKNAME, COUNTER FROM COLLABORATORS WHERE NICKNAME='" + nickname + "'");
        if (!done) qDebug() << query.lastError();

        if (query.first()) {
            QVariant tmp1 = query.value(0); // ROWID as SITE_ID
            QVariant tmp2 = query.value(2); // COUNTER

            *site_id = tmp1.toInt();
            *counter = tmp2.toUInt();
            editors_counter++;
            SharedEditor* new_se = new SharedEditor(clientConnection, *site_id, _symbols.size(), this);
            sharedEditors.push_back(new_se);
        }
    }
    collaborators_db.close();
    QSqlDatabase::removeDatabase(collaborators_db_path);
}

SharedEditor* SharedDocument::getSharedEditor(qint32 site_id) {
    foreach (SharedEditor* se, sharedEditors)
        if (se->getSiteId() == site_id) return se;

    return nullptr;
}

QVector<QPair<qint32, QString>> SharedDocument::getConnectedCollaboratorsNicknames() {
    QVector<QPair<qint32, QString>> collaborators_nicknames;

    foreach (SharedEditor* se, sharedEditors) {
        QString nickname = this->getCollaboratorNickname(se->getSiteId());
        QPair<qint32, QString> tmp = QPair<qint32, QString>(se->getSiteId(), nickname);
        collaborators_nicknames.push_back(tmp);
    }

    return collaborators_nicknames;
}

QString SharedDocument::getCollaboratorNickname(qint32 site_id) {
    QString nickname = "";

    QString collaborators_db_path = QDir::toNativeSeparators(folder_path + "/collaborators.db");
    QSqlDatabase collaborators_db = QSqlDatabase::addDatabase("QSQLITE", collaborators_db_path);
    collaborators_db.setDatabaseName(collaborators_db_path);
    collaborators_db.open();
    {
        QSqlQuery query(collaborators_db);
        bool done = query.exec("SELECT NICKNAME FROM COLLABORATORS WHERE ROWID=" + QString::number(site_id));
        if (!done) qDebug() << query.lastError();

        if (query.first()) {
            QVariant tmp = query.value(0);
            nickname = tmp.toString();
        }
    }
    collaborators_db.close();
    QSqlDatabase::removeDatabase(collaborators_db_path);

    return nickname;
}

void SharedDocument::notifyIncomingUser(qint32 site_id, QImage profile_pic, QString nickname_to_send) {
    Message msg_to_send = Message(site_id, Message::NOTIFY_CONNECTION, std::nullopt, profile_pic, nickname_to_send);
    this->readMessage(msg_to_send);
}

QVector<Symbol> SharedDocument::getSymbols() {
    return _symbols;
}

QString SharedDocument::getFilePath() {
    return file_path;
}

qint32 SharedDocument::getEditorsCounter() {
    return editors_counter;
}

void SharedDocument::disconnectClient(qint32 siteId, quint32 counter) {
    quint32 cnt = 0;
    foreach (SharedEditor* se, sharedEditors) {
        if (se->getSiteId() == siteId) {
            se->getClientConnection()->abort();
            SharedEditor* tmp = se;
            this->sharedEditors.erase(sharedEditors.begin() + cnt);
            qint32 disconnected_site_id = tmp->getSiteId();
            delete tmp; // To prevent Memory Leakage!

            // Updating data in collaborators.db.
            QString collaborators_db_path = QDir::toNativeSeparators(folder_path + "/collaborators.db");
            QSqlDatabase collaborators_db = QSqlDatabase::addDatabase("QSQLITE", collaborators_db_path);
            collaborators_db.setDatabaseName(collaborators_db_path);
            collaborators_db.open();
            {
                collaborators_db.transaction();
                QSqlQuery query(collaborators_db);
                bool done = query.exec("UPDATE COLLABORATORS SET COUNTER=" + QString::number(counter) + " WHERE ROWID=" + QString::number(siteId));
                if (!done) qDebug() << query.lastError();
                collaborators_db.commit();
            }
            collaborators_db.close();
            QSqlDatabase::removeDatabase(collaborators_db_path);

            editors_counter--;
            if (editors_counter == 0) {
                QSqlDatabase::removeDatabase(QDir::toNativeSeparators(folder_path + "/collaborators.db"));
                QSqlDatabase::removeDatabase(QDir::toNativeSeparators(folder_path + "/symbols.db"));
                emit saving();
            }

            Message msg_to_send = Message(disconnected_site_id, Message::NOTIFY_DISCONNECTION);
            this->readMessage(msg_to_send);
            return;
        }
        cnt++;
    }
}

void SharedDocument::dispatchMessages() {
    Message tmp_msg;

    while (!messages.empty()) {
        tmp_msg = messages.front();
        messages.pop();

        // Calculating message size in bytes.
        SerialSize size;
        qint64 msg_size = size(tmp_msg);

        foreach (SharedEditor* se, sharedEditors) {
            if (tmp_msg.getSiteId() != se->getSiteId()) {
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_5_13);
                out << msg_size << tmp_msg;
                se->getClientConnection()->write(block);
            }
        }
    }
}

void SharedDocument::readMessage(Message& msg) {
    if (msg.getType() == Message::INSERT_TYPE || msg.getType() == Message::ERASE_TYPE)
        this->processMessage(msg);
    if (msg.getType() == Message::CURSOR_POS_CHANGED_TYPE) {
        this->getSharedEditor(msg.getSiteId())->setCursorPos(msg.getCursorPosition());
    }

    // If Message::NOTIFY_CONNECTION or Message::NOTIFY_DISCONNECTION,
    // the message is directly pushed into the messages queue.
    messages.push(msg);
    this->dispatchMessages();
}

void SharedDocument::processSymbol(const Symbol& symbol, int msg_type, bool open) {
    qint32 index = 0;
    if (msg_type == Message::INSERT_TYPE) {
        if (_symbols.empty()) _symbols.push_back(symbol);
        else {
            foreach (Symbol s, _symbols) {
                if (this->comparePositions(s.getPos(), symbol.getPos())) {
                    _symbols.insert(_symbols.begin() + index, symbol);
                    if (!open) this->updateDocument(symbol, msg_type);
                    return;
                }
                index++;
            }
            _symbols.push_back(symbol);
        }
    } else /* Message::ERASE_TYPE */ {
        foreach (Symbol s, _symbols) {
            if (s.getChar() == symbol.getChar() && s.getSiteId() == symbol.getSiteId() && s.getCounter() == symbol.getCounter()) {
                _symbols.erase(_symbols.begin() + index);
                if (!open) this->updateDocument(symbol, msg_type);
                return;
            }
            index++;
        }
    }
    if (!open) this->updateDocument(symbol, msg_type);
}

void SharedDocument::updateDocument(const Symbol& symbol, int msg_type) {
    this->updateSymbolsDb(symbol, msg_type);
    emit saving();
}

void SharedDocument::updateSymbolsDb(const Symbol& symbol, int update_type) {
    QSqlDatabase symbols_db = QSqlDatabase::database(QDir::toNativeSeparators(folder_path + "/symbols.db"));
    symbols_db.open();
    {
        symbols_db.transaction();
        QSqlQuery query(symbols_db);
        bool done;
        if (update_type == Message::INSERT_TYPE) {
            QString ch = symbol.getChar();
            if (QString::compare(ch, "'") == 0) ch = "''"; // Escaping single quote character in SQL.
            qint32 site_id = symbol.getSiteId();
            quint32 counter = symbol.getCounter();
            done = query.exec("INSERT INTO SYMBOLS(CHARACTER, SITE_ID, COUNTER) VALUES('" + ch + "', " + QString::number(site_id) + ", " + QString::number(counter) + ")");
        } else /* Message::ERASE_TYPE */ {
            qint32 site_id = symbol.getSiteId();
            quint32 counter = symbol.getCounter();
            done = query.exec("DELETE FROM SYMBOLS WHERE SITE_ID=" + QString::number(site_id) + " AND COUNTER=" + QString::number(counter));
        }
        if (!done) qDebug() << query.lastError();

        symbols_db.commit();
    }
    symbols_db.close();
}

void SharedDocument::processMessage(const Message& msg) {
    Symbol s = msg.getSymbol();
    this->processSymbol(s, msg.getType());
}

// Returns true if pos1 > pos2
bool SharedDocument::comparePositions(std::optional<QVector<qint32>> pos1_opt,
                                     std::optional<QVector<qint32>> pos2_opt) {
    QVector<qint32> pos1 = (pos1_opt.has_value()) ? pos1_opt.value() : QVector<qint32>();
    QVector<qint32> pos2 = (pos2_opt.has_value()) ? pos2_opt.value() : QVector<qint32>();

    qint32 digit1 = (!pos1.empty()) ? pos1[0] : 0;
    qint32 digit2 = (!pos2.empty()) ? pos2[0] : 0;

    if (digit1 > digit2) return true;
    else if (digit1 < digit2) return false;
    else if (digit1 == digit2 && pos1.size() == 1 && pos2.size() == 1) return false;
    else {
        QVector<qint32> slice1 = (pos1.begin() + 1 < pos1.end()) ? pos1.mid(1) : QVector<qint32>();
        QVector<qint32> slice2 = (pos2.begin() + 1 < pos2.end()) ? pos2.mid(1) : QVector<qint32>();
        return this->comparePositions(slice1, slice2);
    }
}

QString SharedDocument::symbols_to_string() {
    QString str;
    foreach (Symbol s, _symbols) {
        str += s.getChar();
    }
    return str;
}

QString SharedDocument::positions_to_string() {
    QString pos_str = "";
    quint32 cnt = 0;
    foreach (Symbol s, _symbols) {
        pos_str.append(QString("%1, %2").arg(s.getSiteId()).arg(s.getCounter()));
        foreach (qint32 digit, s.getPos()) pos_str.append(QString(", %1").arg(digit));
        pos_str.append("\n");
        cnt++;
    }
    return pos_str;
}