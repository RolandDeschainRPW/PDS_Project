//
// Created by raffa on 28/08/2019.
//

#include "../include/SharedDocument.h"

SharedDocument::SharedDocument(QString filename, QObject* parent) : QObject(parent), filename(filename) {
    // Initializing a map for free Site Ids.
    for (qint32 i = 0; i < MAX_SITE_IDS; i++) free_site_ids.push_back(true);
    document = new QTextDocument(this);
}

// Returns the assigned site id (or -1 if not assigned).
qint32 SharedDocument::addSharedEditor(QTcpSocket* clientConnection) {
    quint32 cnt = 0;
    foreach (bool free, this->free_site_ids) {
        if (free) {
            SharedEditor* new_se = new SharedEditor(clientConnection, cnt, this);
            sharedEditors.insert(sharedEditors.begin() + cnt, new_se);
            this->free_site_ids[cnt] = false;
            qDebug() << "I assigned the following Site Id to the incoming client: " << cnt;
            return cnt;
        }
        cnt++;
    }
    // No Site Ids available.
    return SharedDocument::SITE_ID_UNASSIGNED;
}

QVector<Symbol> SharedDocument::getSymbols() {
    return _symbols;
}

QString SharedDocument::getFilename() {
    return filename;
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
                    qDebug() << "Message sent!";
                }
            }
    }
}

void SharedDocument::readMessage(Message& msg) {
    this->processSymbol(msg);
    messages.push(msg);
    this->dispatchMessages();
}

void SharedDocument::processSymbol(const Message& msg) {
    qint32 index = 0;
    if (msg.getType() == Message::INSERT_TYPE) {
        if (_symbols.empty()) _symbols.push_back(msg.getSymbol());
        else {
                    foreach (Symbol s, _symbols) {
                    if (this->comparePositions(s.getPos(), msg.getSymbol().getPos())) {
                        _symbols.insert(_symbols.begin() + index, msg.getSymbol());
                        qDebug() << "Current symbols on Server: " << tr(this->symbols_to_string().toStdString().c_str());
                        //qDebug().noquote() << "Current positions for each symbol on Server:\n" << tr(this->positions_to_string().toStdString().c_str());
                        return;
                    }
                    index++;
                }
            _symbols.push_back(msg.getSymbol());
        }
        qDebug() << "I have INSERTED a symbol!";
    } else /* Message::ERASE_TYPE */ {
                foreach (Symbol s, _symbols){
                if (s.getChar() == msg.getSymbol().getChar() && s.getId() == msg.getSymbol().getId()) {
                    _symbols.erase(_symbols.begin() + index);
                    qDebug() << "I have ERASED a symbol!";
                    qDebug() << "Current symbols on Server: " << tr(this->symbols_to_string().toStdString().c_str());
                    //qDebug().noquote() << "Current positions for each symbol on Server:\n" << tr(this->positions_to_string().toStdString().c_str());
                    return;
                }
                index++;
            }
        qDebug() << "ERASING already done!";
    }
    qDebug() << "Current symbols on Server: " << tr(this->symbols_to_string().toStdString().c_str());
    //qDebug().noquote() << "Current positions for each symbol on Server:\n" << tr(this->positions_to_string().toStdString().c_str());
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
            pos_str.append(QString("[%1] ->").arg(cnt));
                    foreach (qint32 digit, s.getPos()) pos_str.append(QString(" %1 ").arg(digit));
            pos_str.append("\n");
            cnt++;
        }
    return pos_str;
}