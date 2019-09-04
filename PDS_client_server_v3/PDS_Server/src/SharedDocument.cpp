//
// Created by raffa on 28/08/2019.
//

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlRecord>
#include <QVector>

#include "../include/SharedDocument.h"

// This constructor is used in both New Document and Open Document cases.
// collaborators.db, symbols.db, symbols.txt and the html file MUST be read in case of
// pre-existing data on Server!
SharedDocument::SharedDocument(QString file_path, QString folder_path, QObject* parent) : QObject(parent), file_path(file_path), folder_path(folder_path) {
    // Initializing a map for free Site Ids.
    for (qint32 i = 0; i < MAX_SITE_IDS; i++) free_site_ids.push_back(true);
    QString collaborators_db_path = QDir::toNativeSeparators(folder_path + "/collaborators.db");
    QSqlDatabase collaborators_db = QSqlDatabase::addDatabase("QSQLITE", "collaborators_db_conn");
    collaborators_db.setDatabaseName(collaborators_db_path);
    collaborators_db.open();
    {
        QSqlQuery query(collaborators_db);
        bool done = query.exec("SELECT SITE_ID FROM COLLABORATORS");
        if (!done) qDebug() << query.lastError();

        if (query.first()) {
            do {
                QVariant val = query.value(0);
                qint32 site_id = val.toInt();
                free_site_ids[site_id] = false;
            } while (query.next());
        }
    }
    collaborators_db.close();

    // Reading from file.
    document = new QTextDocument(this);
    QFile file = QFile(file_path);
    file.open(QIODevice::ReadOnly);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        qDebug().noquote() << "Warning!\n\t-> Cannot read file: " << file.errorString();
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    document->setPlainText(text);

    // Reading symbols data.
    QString symbols_db_path = QDir::toNativeSeparators(folder_path + "/symbols.db");
    QSqlDatabase symbols_db = QSqlDatabase::addDatabase("QSQLITE", "symbols_db_conn");
    symbols_db.setDatabaseName(symbols_db_path);
    symbols_db.open();
    {
        QSqlQuery query = QSqlQuery(symbols_db);
        bool done = query.exec("SELECT * FROM SYMBOLS");
        if (!done) qDebug() << query.lastError();

        QSqlRecord record = query.record();
        if (query.first()) {
            do {
                QChar ch = query.value(record.indexOf("CHARACTER")).toChar();
                qint32 site_id = query.value(record.indexOf("SITE_ID")).toInt();
                quint32 cnt = query.value(record.indexOf("COUNTER")).toUInt();
                quint32 id = static_cast<quint32>(site_id) + cnt;
                QVector<qint32> pos = QVector<qint32>();

                QString symbols_txt_path = QDir::toNativeSeparators(folder_path + "/symbols.txt");
                QFile symbols_txt(symbols_txt_path);
                QTextStream stream(&symbols_txt);
                QString line = stream.readLine();
                bool finished = false;
                while (!line.isNull() && !finished) {
                    QStringList list = line.simplified().split(',');
                    qint32 searched_id = list[0].trimmed().toInt();
                    if (id == searched_id) {
                        for (int i = 1; i < list.size(); i++)
                            pos.push_back(list[i].trimmed().toInt());
                        finished = true;
                    }
                }
                Symbol s = Symbol(ch, site_id, cnt, pos);
                this->processSymbol(s, Message::INSERT_TYPE);
            } while (query.next());
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

void SharedDocument::addSharedEditor(QTcpSocket* clientConnection, QString username, qint32* site_id, quint32* counter) {
    // Retrieving Site Id of incoming Shared Editor.
    QSqlDatabase collaborators_db = QSqlDatabase::database("collaborators_db_conn");
    collaborators_db.open();
    {
        QSqlQuery query(collaborators_db);
        bool done = query.exec("SELECT * FROM COLLABORATORS WHERE USERNAME='" + username + "'");
        if (!done) qDebug() << query.lastError();
        QSqlRecord record = query.record();

        if (query.first()) {
            *site_id = query.value(record.indexOf("SITE_ID")).toInt();
            *counter = query.value(record.indexOf("COUNTER")).toUInt();
            editors_counter++;
            SharedEditor* new_se = new SharedEditor(clientConnection, *site_id, this);
            sharedEditors.insert(sharedEditors.begin() + *site_id, new_se);
        }
    }
    collaborators_db.close();
}

qint32 SharedDocument::addCollaborator(QString username) {
    // Here a new Site Id is assigned to the incoming Shared Editor.
    quint32 candidate_id = 0;
    foreach (bool free, this->free_site_ids) {
        if (free) {
            this->free_site_ids[candidate_id] = false;

            // Adding data to collaborators.db.
            QSqlDatabase collaborators_db = QSqlDatabase::database("collaborators_db_conn");
            collaborators_db.open();
            {
                collaborators_db.transaction();
                quint32 counter = 0;
                QSqlQuery query(collaborators_db);
                bool done = query.exec("INSERT INTO COLLABORATORS(USERNAME, SITE_ID, COUNTER) VALUES('" + username + "', " + candidate_id + ", " + counter + ")");
                if (!done) qDebug() << query.lastError();
                collaborators_db.commit();
            }
            collaborators_db.close();
            return candidate_id;
        }
        candidate_id++;
    }
    return SharedDocument::SITE_ID_UNASSIGNED;
}

QVector<Symbol> SharedDocument::getSymbols() {
    return _symbols;
}

QString SharedDocument::getFilePath() {
    return file_path;
}

SharedEditor* SharedDocument::getSharedEditor(qint32 siteId) {
    foreach(SharedEditor* se, sharedEditors) {
        if (se->getSiteId() == siteId) return se;
    }
    return nullptr;
}

void SharedDocument::disconnectClient(qint32 siteId) {
    quint32 cnt = 0;
    foreach (SharedEditor* se, sharedEditors) {
        if (se->getSiteId() == siteId) {
            SharedEditor* tmp = this->sharedEditors[cnt];
            this->sharedEditors[cnt]->getClientConnection()->disconnectFromHost();
            this->sharedEditors.erase(sharedEditors.begin() + cnt);
            this->free_site_ids[siteId] = true;
            delete tmp; // To prevent Memory Leakage!
            return;
        }
        cnt++;
    }
    editors_counter--;

    if (editors_counter == 0) emit saving();
}

void SharedDocument::dispatchMessages() {
    Message tmp_msg;
    while (!messages.empty()) {
        tmp_msg = messages.front();
        messages.pop();

        foreach (SharedEditor* se, sharedEditors) {
            if (tmp_msg.getSiteId() != se->getSiteId()) {
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_5_13);
                out << tmp_msg;
                se->getClientConnection()->write(block);
            }
        }
    }
}

void SharedDocument::readMessage(Message& msg) {
    this->processMessage(msg);
    messages.push(msg);
    this->dispatchMessages();
}

void SharedDocument::processSymbol(const Symbol& symbol, int msg_type) {
    qint32 index = 0;
    if (msg_type == Message::INSERT_TYPE) {
        if (_symbols.empty()) _symbols.push_back(symbol);
        else {
            foreach (Symbol s, _symbols) {
                if (this->comparePositions(s.getPos(), symbol.getPos())) {
                    _symbols.insert(_symbols.begin() + index, symbol);
                    this->updateSymbolsDb(symbol, msg_type);
                    emit saving();
                    return;
                }
                index++;
            }
            _symbols.push_back(symbol);
        }
    } else /* Message::ERASE_TYPE */ {
        foreach (Symbol s, _symbols) {
            if (s.getChar() == symbol.getChar() && s.getId() == symbol.getId()) {
                _symbols.erase(_symbols.begin() + index);
                this->updateSymbolsDb(symbol, msg_type);
                emit saving();
                return;
            }
            index++;
        }
    }
    this->updateSymbolsDb(symbol, msg_type);
    emit saving();
}

void SharedDocument::updateSymbolsDb(const Symbol& symbol, int update_type) {
    QSqlDatabase symbols_db = QSqlDatabase::database("symbols_db_conn");
    symbols_db.open();
    {
        symbols_db.transaction();
        QSqlQuery query(symbols_db);
        bool done;
        if (update_type == Message::INSERT_TYPE) {
            QString ch = symbol.getChar();
            qint32 site_id = symbol.getSiteId();
            quint32 counter = symbol.getCounter();
            done = query.exec("INSERT INTO SYMBOLS(CHARACTER, SITE_ID, COUNTER) VALUES('" + ch + "', " + site_id + ", " + counter + ")");
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
        pos_str.append(QString("%1").arg(s.getId()));
        foreach (qint32 digit, s.getPos()) pos_str.append(QString(", %1").arg(digit));
        pos_str.append("\n");
        cnt++;
    }
    return pos_str;
}