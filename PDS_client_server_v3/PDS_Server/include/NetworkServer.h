//
// Created by raffa on 21/08/2019.
//

#ifndef PDS_SERVER_NETWORKSERVER_H
#define PDS_SERVER_NETWORKSERVER_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include <optional>

#include <QDialog>
#include <QString>
#include <QVector>

#include "../include/Symbol.h"
#include "../include/Message.h"
#include "../include/SharedEditor.h"
#include "../include/SharedDocument.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
class QNetworkSession;
class QTcpSocket;
class SharedEditor;
QT_END_NAMESPACE

class NetworkServer : public QDialog {
Q_OBJECT

public:
    static const int MAX_SITE_IDS = 100;
    static const int USERNAME_NOT_FOUND = -1;
    static const int WRONG_PASSWORD = -2;
    static const int SEARCH_FAILED = -3;

    explicit NetworkServer(QString usersListFileName, QWidget *parent = nullptr);

private slots:
    void sessionOpened();
    void readFromNewConnection();

private:
    QString usersListFileName;
    //QVector<User> activeUsers;
    //QVector<QString> documentsFileNames;
    //QVector<SharedDocument> openDocuments;
    std::queue<Message> messages;
    QVector<SharedEditor*> sharedEditors;
    QVector<Symbol> _symbols;
    QLabel* statusLabel = nullptr;
    QTcpServer* tcpServer = nullptr;
    QNetworkSession* networkSession = nullptr;
    QVector<bool> free_site_ids;

    void readMessage(Message& msg);
    bool comparePositions(std::optional<QVector<qint32>> pos1_opt, std::optional<QVector<qint32>> pos2_opt);
    void dispatchMessages();
    void connectClient(QTcpSocket* clientConnection, QString username, QString password);
    void disconnectClient(qint32 siteId);
    void signUpNewUser(QTcpSocket* clientConnection, QString username, QString password);
    int searchUsername(QString username);
    int checkPassword(int lineIndex, QString password);
    void readFromExistingConnection(QTcpSocket* clientConnection);
    void processNewConnections(QTcpSocket* clientConnection);
    void processSymbol(const Message& msg);
    QString symbols_to_string();
    //void saveDocument();

    // For debugging purposes.
    QString positions_to_string();
};

#endif //PDS_SERVER_NETWORKSERVER_H
