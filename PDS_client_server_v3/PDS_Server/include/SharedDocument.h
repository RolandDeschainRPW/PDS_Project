//
// Created by raffa on 28/08/2019.
//

#ifndef PDS_SERVER_SHAREDDOCUMENT_H
#define PDS_SERVER_SHAREDDOCUMENT_H

#include <queue>

#include <QTextDocument>
#include <QtNetwork>

#include "../include/Message.h"
#include "../include/Symbol.h"
#include "../include/SharedEditor.h"

class SharedDocument : public QObject {
    Q_OBJECT

public:
    SharedDocument(QString filename, QObject* parent = 0);

    qint32 addSharedEditor(QTcpSocket* clientConnection);
    QVector<Symbol> getSymbols();
    QString getFilename();
    SharedEditor* getSharedEditor(qint32 siteId);
    void readMessage(Message& msg);
    void disconnectClient(qint32 siteId);

    static const qint32 MAX_SITE_IDS = 7; // Maximum number of collaborators per document (7 + the owner = 8).
    static const qint32 SITE_ID_UNASSIGNED = -1;

private:
    QString filename;
    QVector<Symbol> _symbols;
    QVector<SharedEditor*> sharedEditors;
    QTextDocument* document = nullptr;
    std::queue<Message> messages;
    QVector<bool> free_site_ids;

    bool comparePositions(std::optional<QVector<qint32>> pos1_opt, std::optional<QVector<qint32>> pos2_opt);
    void dispatchMessages();
    void processSymbol(const Message& msg);
    QString symbols_to_string();
    QString positions_to_string();
};


#endif //PDS_SERVER_SHAREDDOCUMENT_H
