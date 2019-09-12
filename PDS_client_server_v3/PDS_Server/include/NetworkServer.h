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
#include <QImage>

#include "../include/Symbol.h"
#include "../include/Message.h"
#include "../include/SharedEditor.h"
#include "../include/SharedDocument.h"
#include "../include/Request.h"
#include "../include/Collaborator.h"

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
    QString users_db_path;
    QVector<QString> activeUsers;
    QVector<SharedDocument*> openDocuments;
    QLabel* statusLabel = nullptr;
    QTcpServer* tcpServer = nullptr;
    QNetworkSession* networkSession = nullptr;

    void connectClient(QTcpSocket* clientConnection, QString username, QString password);
    void signUpNewUser(QTcpSocket* clientConnection, QString username, QString password, QString nickname, QImage profile_pic, QString image_format);
    void createNewDocumentDirectory(QString username, QString filename);
    bool isThisUsernameInDatabase(QString username, QString* password = nullptr, QString* nickname = nullptr);
    bool isThisNicknameInDatabase(QString nickname, QString* username = nullptr);
    bool readFromExistingConnection(Request& req);
    void processNewConnections(QTcpSocket* clientConnection, Request& req);
    void writeStartDataToClient(QTcpSocket* clientConnection, bool new_document, QString filename, QString owner_username, QString opener_nickname = "");
    SharedDocument* getDocument(QString file_path);
    void removeFromActiveUsers(QString username);
    void removeFromOpenDocuments(SharedDocument* document);
    bool addCollaborator(QString nickname, QString username, QString filename);
    void addPermission(QString collaborator_username, QString owner_nickname, QString file_path);
    void modifyProfile(QTcpSocket* clientConnection, QString username, QString image_format, std::optional<QImage> opt_profile_pic, QString new_password);
    void sendProfilePic(QTcpSocket* clientConnection, QString username);
    QImage searchProfilePic(QString username, QString* image_format = nullptr);
    QVector<Collaborator> getConnectedCollaborators(SharedDocument* document);
};

#endif //PDS_SERVER_NETWORKSERVER_H
