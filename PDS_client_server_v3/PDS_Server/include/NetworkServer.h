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
#include <QSqlDatabase>

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
    explicit NetworkServer(QString usersDbFileName, QWidget *parent = nullptr);

private slots:
    void sessionOpened();
    void readFromNewConnection();

private:
    QString usersDbFileName;
    QVector<QString> activeUsers;
    QVector<SharedDocument*> openDocuments;
    QLabel* statusLabel = nullptr;
    QTcpServer* tcpServer = nullptr;
    QNetworkSession* networkSession = nullptr;
    QSqlDatabase users_db;

    void connectClient(QTcpSocket* clientConnection, QString username, QString password);
    void signUpNewUser(QTcpSocket* clientConnection, QString username, QString password);
    void createNewDocumentDirectory(QString username, QString filename);
    bool isThisUsernameInDatabase(QString username, QString* password = nullptr);
    void readFromExistingConnection(QTcpSocket* clientConnection);
    void processNewConnections(QTcpSocket* clientConnection);
    void writeStartDataToClient(QTcpSocket* clientConnection, bool new_document, QString filename);
    SharedDocument* getDocument(QString filename);
    //void saveDocument();
};

#endif //PDS_SERVER_NETWORKSERVER_H
