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
    SharedDocument(QString file_path, QString folder_path, QObject* parent = 0);

    void addSharedEditor(QTcpSocket* clientConnection, QString nickname, qint32* site_id, quint32* counter);
    void readMessage(Message& msg);
    void disconnectClient(qint32 siteId, quint32 counter);
    void notifyIncomingUser(qint32 site_id, QImage profile_pic, QString nickname_to_send);
    SharedEditor* getSharedEditor(qint32 site_id);
    QVector<QPair<qint32, QString>> getConnectedCollaboratorsNicknames();
    QVector<Symbol> getSymbols();
    QString getFilePath();
    qint32 getEditorsCounter();

    static const qint32 SITE_ID_UNASSIGNED = -1;

public slots:
    void automaticSave();

signals:
    void saving();

private:
    QString file_path;
    QString folder_path;
    QVector<Symbol> _symbols;
    QVector<SharedEditor*> sharedEditors;
    QTextDocument* document = nullptr;
    std::queue<Message> messages;
    qint32 editors_counter = 0;

    void dispatchMessages();
    void processMessage(const Message& msg);
    void processSymbol(const Symbol& symbol, int msg_type, bool open = false);
    void updateSymbolsDb(const Symbol& symbol, int update_type);
    void updateDocument(const Symbol& symbol, int msg_type);
    bool comparePositions(std::optional<QVector<qint32>> pos1_opt, std::optional<QVector<qint32>> pos2_opt);
    QString getCollaboratorNickname(qint32 site_id);
    QString symbols_to_string();
    QString positions_to_string();
};


#endif //PDS_SERVER_SHAREDDOCUMENT_H
