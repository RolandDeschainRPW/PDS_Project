//
// Created by raffa on 21/08/2019.
//

#include "../include/Request.h"
#include "../include/NetworkServer.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlField>
#include <QVariant>
#include <QSqlError>

#include "../include/Response.h"

NetworkServer::NetworkServer(QString usersDbFileName,
                        QWidget *parent) : QDialog(parent),
                                        statusLabel(new QLabel) {
    // Initialiting directories.
    if (!QDir("documents").exists()) QDir().mkdir("documents");
    if (!QDir("profiles").exists()) QDir().mkdir("profiles");

    // Connecting to users database
    bool newly_created = !QFile(usersDbFileName).exists();
    users_db_path = QDir::toNativeSeparators(usersDbFileName);
    QSqlDatabase users_db = QSqlDatabase::addDatabase("QSQLITE", "users_db_conn");
    users_db.setDatabaseName(users_db_path);
    users_db.open(); // If it doesn't exists, it will be created.
    if (newly_created) {
        {
            users_db.transaction();
            QSqlQuery query(users_db);
            bool done = query.exec("CREATE TABLE USERS("
                       "USERNAME VARCHAR(30) PRIMARY KEY COLLATE NOCASE, "
                       "PASSWORD VARCHAR(30) NOT NULL, "
                       "NICKNAME VARCHAR(30) NOT NULL COLLATE NOCASE"
                       ")");
            if (!done) qDebug() << query.lastError();
            users_db.commit();
        }
    }
    users_db.close();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &NetworkServer::sessionOpened);

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }

    auto quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpServer, &QTcpServer::newConnection, this, &NetworkServer::readFromNewConnection);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = nullptr;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QVBoxLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QVBoxLayout(this);
    }

    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);

    setWindowTitle(QGuiApplication::applicationDisplayName());

    qDebug() << "Server started.";
}

void NetworkServer::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("NetworkServer"), tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The Server is running on\n\nIP: %1\nport: %2\n\nRun the Client(s) now.").arg(ipAddress).arg(tcpServer->serverPort()));
}

void NetworkServer::connectClient(QTcpSocket* clientConnection, QString username, QString password) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    // Verifying if credentials are correct.
    QString real_password;
    QString nickname;
    bool found = isThisUsernameInDatabase(username, &real_password, &nickname);
    if (!found || QString::compare(password, real_password, Qt::CaseSensitive) != 0) {
        out << Response(Response::WRONG_CREDENTIALS, QMap<QString, QString>());
        clientConnection->write(block);
        return;
    }

    // Verifying if the username is already active.
    foreach (QString usr, activeUsers) {
        if (QString::compare(usr, username, Qt::CaseInsensitive) == 0) {
            out << Response(Response::USERNAME_ACTIVE, QMap<QString, QString>());
            clientConnection->write(block);
            return;
        }
    }

    // Listing all user's docs subfolders.
    QMap<QString, QString> all_docs;
    QString parent = QString(QDir::toNativeSeparators("documents/" + username));
    QDirIterator dirs = QDirIterator(parent, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while(dirs.hasNext()) {
        dirs.next();
        QString filename = dirs.fileName() + ".html";
        all_docs.insertMulti(filename, nickname);
    }

    // Listing all docs the user has access to.
    QString permissions_db_path = QDir::toNativeSeparators("documents/" + username + "/permissions.db");
    QSqlDatabase permissions_db = QSqlDatabase::addDatabase("QSQLITE", permissions_db_path);
    permissions_db.setDatabaseName(permissions_db_path);
    permissions_db.open();
    {
        QSqlQuery query(permissions_db);
        bool done = query.exec("SELECT * FROM PERMISSIONS");
        if (!done) qDebug() << query.lastError();

        while (query.next()) {
            QVariant tmp1 = query.value(0); // OWNER
            QVariant tmp2 = query.value(1); // DOCUMENT

            QString owner = tmp1.toString();
            QString document = tmp2.toString() + ".html";
            all_docs.insertMulti(document, owner);
        }
    }
    permissions_db.close();
    QSqlDatabase::removeDatabase(permissions_db_path);

    // Adding current user to active users.
    activeUsers.push_back(username);

    connect(clientConnection, &QAbstractSocket::disconnected, this, [this, username] {
        this->removeFromActiveUsers(username);
    });

    // Telling the client that Login went well.
    out << Response(Response::SUCCESSFUL_LOGIN, all_docs, username);
    clientConnection->write(block);
}

// It returns false when the socket has been disconnected.
bool NetworkServer::readFromExistingConnection(QTcpSocket* clientConnection) {
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_5_13);
    in.startTransaction();

    Request next_req;
    in >> next_req;
    qint32 site_id = next_req.getSiteId();
    quint32 counter = next_req.getCounter();
    QString file_path = next_req.getFilename();
    if (!in.commitTransaction()) {
        qDebug().noquote() << "Something went wrong!\n\t-> I could not read the incoming request!";
        return true;
    }
    if (next_req.getRequestType() == Request::MESSAGE_TYPE) {
        Message msg = next_req.getMessage();
        this->getDocument(file_path)->readMessage(msg);
        return true;
    } else if (next_req.getRequestType() == Request::DISCONNECT_TYPE) {
        SharedDocument* document = this->getDocument(file_path);
        document->disconnectClient(site_id, counter);
        if (document->getEditorsCounter() == 0) this->removeFromOpenDocuments(document);
        return false;
    }
}

void NetworkServer::removeFromOpenDocuments(SharedDocument* document) {
    qint32 i = 0;
    foreach (SharedDocument* sd, openDocuments) {
        // If the two pointers are equal, they indicate the same memory location.
        if (sd == document) {
            this->openDocuments.erase(openDocuments.begin() + i);
            delete document; // To prevent Memory Leakage!
            return;
        }
        i++;
    }
}

void NetworkServer::removeFromActiveUsers(QString username) {
    qint32 i = 0;
    foreach (QString active_username, this->activeUsers) {
        if (QString::compare(username, active_username, Qt::CaseInsensitive) == 0) {
            activeUsers.erase(activeUsers.begin() + i);
            return;
        }
        i++;
    }
}

SharedDocument* NetworkServer::getDocument(QString file_path) {
    foreach (SharedDocument* sd, openDocuments) {
        if (QString::compare(file_path, sd->getFilePath(), Qt::CaseInsensitive) == 0)
            return sd;
    }
    return nullptr;
}

void NetworkServer::readFromNewConnection() {
    QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QIODevice::readyRead, this, [this, clientConnection] {
        this->processNewConnections(clientConnection);
    });
}

void NetworkServer::processNewConnections(QTcpSocket* clientConnection) {
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_5_13);
    in.startTransaction();

    Request next_req;
    in >> next_req;
    if (!in.commitTransaction()) {
        qDebug().noquote() << "Something went wrong!\n\t-> I could not read the incoming request!";
        clientConnection->abort();
        return;
    }
    if (next_req.getRequestType() == Request::CONNECT_TYPE) {
        this->connectClient(clientConnection, next_req.getUsername(), next_req.getPassword());
    } else if (next_req.getRequestType() == Request::SIGN_UP_TYPE) {
        this->signUpNewUser(clientConnection, next_req.getUsername(), next_req.getPassword(), next_req.getNickname());
    } else if (next_req.getRequestType() == Request::OPEN_DOCUMENT_TYPE) {
        // Retrieving the document's owner username by its nickname.
        QString owner_username;
        this->isThisNicknameInDatabase(next_req.getNickname(), &owner_username);

        // Retrieving the opener's nickname.
        QString opener_nickname;
        this->isThisUsernameInDatabase(next_req.getUsername(), nullptr, &opener_nickname);

        this->writeStartDataToClient(clientConnection, false, next_req.getFilename(), owner_username, opener_nickname);
    } else if (next_req.getRequestType() == Request::NEW_DOCUMENT_TYPE) {
        this->createNewDocumentDirectory(next_req.getUsername(), next_req.getFilename());
        this->writeStartDataToClient(clientConnection, true, next_req.getFilename(), next_req.getUsername());
    } else if (next_req.getRequestType() == Request::ADD_COLLABORATOR_TYPE) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_13);
        qint32 result;
        QString collaborator_username;
        QString owner_nickname;

        if (!this->isThisNicknameInDatabase(next_req.getNickname(), &collaborator_username))
            result = Response::NICKNAME_NON_EXISTENT;
        else {
            result = Response::NICKNAME_ACTIVE;
            this->isThisUsernameInDatabase(next_req.getUsername(), nullptr, &owner_nickname);
            this->addCollaborator(next_req.getNickname(), next_req.getUsername(), next_req.getFilename());
            this->addPermission(collaborator_username, owner_nickname, next_req.getFilename());
        }

        Response res = Response(result, QMap<QString, QString>());
        out << res;
        clientConnection->write(block);
    }
}

void NetworkServer::writeStartDataToClient(QTcpSocket* clientConnection, bool new_document, QString filename, QString owner_username, QString opener_nickname) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    QString file_path = QDir::toNativeSeparators("documents/" + owner_username + "/" + filename + "/" + filename + ".html");
    QString folder_path = QDir::toNativeSeparators("documents/" + owner_username + "/" + filename);
    SharedDocument* doc;
    qint32 site_id_assigned;
    quint32 counter;
    QString owner_nickname;

    // Retrieving the nickname corresponding to the owner username.
    this->isThisUsernameInDatabase(owner_username, nullptr, &owner_nickname);

    if (new_document) {
        doc = new SharedDocument(file_path, folder_path, this);
        this->addCollaborator(owner_nickname, owner_username, filename);
        doc->addSharedEditor(clientConnection, owner_nickname, &site_id_assigned, &counter);
        openDocuments.push_back(doc);
    } else /* Opening an existing document. */ {
        doc = this->getDocument(file_path);

        if (doc == nullptr) { /* Document not already opened. */
            doc = new SharedDocument(file_path, folder_path, this);
            openDocuments.push_back(doc);
        }

        doc->addSharedEditor(clientConnection, opener_nickname, &site_id_assigned, &counter);
        if (site_id_assigned == SharedDocument::SITE_ID_UNASSIGNED) /* No more Site Ids available. */ {
            qDebug().noquote() << "I cannot host the incoming client!\n\t-> No more Site Ids available!";
            clientConnection->abort();
            return;
        }
    }
    out << site_id_assigned << counter << file_path << doc->getSymbols();
    clientConnection->write(block);
    disconnect(clientConnection, &QIODevice::readyRead, this, 0);
    connect(clientConnection, &QIODevice::readyRead, this, [this, clientConnection] {
        while (clientConnection->bytesAvailable()) {
            // When the socket has been disconnected, the cycle gets broken.
            if (!this->readFromExistingConnection(clientConnection)) break;
        }
    });

}

void NetworkServer::createNewDocumentDirectory(QString username, QString filename) {
    QDir().mkdir(QDir::toNativeSeparators("documents/" + username + "/" + filename));
    QString new_file = QDir::toNativeSeparators("documents/" + username + "/" + filename + "/" + filename + ".html");
    QFile(new_file).open(QIODevice::ReadWrite);

    QString symbols_pos_data_path = QDir::toNativeSeparators("documents/" + username + "/" + filename + "/" + "symbols.txt");
    QFile(symbols_pos_data_path).open(QIODevice::ReadWrite);

    QString collaborators_db_path = QDir::toNativeSeparators("documents/" + username + "/" + filename + "/collaborators.db");
    QSqlDatabase collaborators_db = QSqlDatabase::addDatabase("QSQLITE", collaborators_db_path);
    collaborators_db.setDatabaseName(collaborators_db_path);
    collaborators_db.open();
    {
        collaborators_db.transaction();
        QSqlQuery query(collaborators_db);
        bool done = query.exec("CREATE TABLE COLLABORATORS("
                   "NICKNAME VARCHAR(30) PRIMARY KEY COLLATE NOCASE, "
                   "COUNTER INTEGER NOT NULL"
                   ")");
        if (!done) qDebug() << query.lastError();
        collaborators_db.commit();
    }
    collaborators_db.close();
    QSqlDatabase::removeDatabase(collaborators_db_path);

    QString symbols_db_path = QDir::toNativeSeparators("documents/" + username + "/" + filename + "/symbols.db");
    QSqlDatabase symbols_db = QSqlDatabase::addDatabase("QSQLITE", symbols_db_path);
    symbols_db.setDatabaseName(symbols_db_path);
    symbols_db.open();
    {
        symbols_db.transaction();
        QSqlQuery query = QSqlQuery(symbols_db);
        bool done = query.exec("CREATE TABLE SYMBOLS("
                   "CHARACTER VARCHAR(1) NOT NULL, "
                   "SITE_ID INTEGER NOT NULL, "
                   "COUNTER INTEGER NOT NULL, "
                   "CONSTRAINT SYMBOLS_PK PRIMARY KEY (SITE_ID, COUNTER)"
                   ")");
        if (!done) qDebug() << query.lastError();
        symbols_db.commit();
    }
    symbols_db.close();
    QSqlDatabase::removeDatabase(symbols_db_path);
}

void NetworkServer::signUpNewUser(QTcpSocket* clientConnection, QString username, QString password, QString nickname) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    // Verifying if the chosen username is already in use.
    bool username_found = this->isThisUsernameInDatabase(username);
    if (username_found) {
        out << Response(Response::USERNAME_ALREADY_IN_USE, QMap<QString, QString>());
        clientConnection->write(block);
        return;
    }

    // Verifying if the chosen nickname is already in use.
    bool nickname_found = this->isThisNicknameInDatabase(nickname);
    if (nickname_found) {
        out << Response(Response::NICKNAME_ALREADY_IN_USE, QMap<QString, QString>());
        clientConnection->write(block);
        return;
    }

    // Storing the new credentials.
    QSqlDatabase users_db = QSqlDatabase::database("users_db_conn");
    users_db.open(); // If it doesn't exists, it will be created.
    {
        users_db.transaction();
        QSqlQuery query(users_db);
        bool done = query.exec("INSERT INTO USERS(USERNAME, PASSWORD, NICKNAME) VALUES('" + username + "', '" + password + "', '" + nickname + "')");
        if (!done) qDebug() << query.lastError();
        users_db.commit();
    }
    users_db.close();

    // Creating the user's documents folder.
    QString new_path = "documents/" + username;
    QDir().mkdir(QDir::toNativeSeparators(new_path));

    // Creating the user's permissions file.
    QString permissions_db_path = QDir::toNativeSeparators("documents/" + username + "/permissions.db");
    QSqlDatabase permissions_db = QSqlDatabase::addDatabase("QSQLITE", permissions_db_path);
    permissions_db.setDatabaseName(permissions_db_path);
    permissions_db.open(); // If it doesn't exists, it will be created.
    {
        permissions_db.transaction();
        QSqlQuery query = QSqlQuery(permissions_db);
        bool done = query.exec("CREATE TABLE PERMISSIONS("
                   "OWNER VARCHAR(30) NOT NULL, "
                   "DOCUMENT VARCHAR(30) NOT NULL, "
                   "CONSTRAINT PERMISSIONS_PK PRIMARY KEY (OWNER, DOCUMENT))");
        if (!done) qDebug() << query.lastError();
        permissions_db.commit();
    }
    permissions_db.close();
    QSqlDatabase::removeDatabase(permissions_db_path);

    // Telling the client that SignUp went well.
    out << Response(Response::USERNAME_ACCEPTED, QMap<QString, QString>());
    clientConnection->write(block);
}

bool NetworkServer::isThisUsernameInDatabase(QString username, QString* password, QString* nickname) {
    bool found;

    QSqlDatabase users_db = QSqlDatabase::database("users_db_conn");
    users_db.open();
    {
        QSqlQuery query(users_db);
        bool done = query.exec("SELECT * FROM USERS WHERE USERNAME='" + username + "'");
        if (!done) qDebug() << query.lastError();

        if (query.first()) {
            if (password != nullptr) {
                QVariant val = query.value(1); // PASSWORD
                *password = val.toString();
            }
            if (nickname != nullptr) {
                QVariant val = query.value(2); // NICKNAME
                *nickname = val.toString();
            }
            found = true;
        } else found = false;
    }
    users_db.close();

    return found;
}

bool NetworkServer::isThisNicknameInDatabase(QString nickname, QString* username) {
    bool found;

    QSqlDatabase users_db = QSqlDatabase::database("users_db_conn");
    users_db.open();
    {
        QSqlQuery query(users_db);
        bool done = query.exec("SELECT USERNAME, NICKNAME FROM USERS WHERE NICKNAME='" + nickname + "'");
        if (!done) qDebug() << query.lastError();

        if (query.first()) {
            if (username != nullptr) {
                QVariant val = query.value(0); // USERNAME
                *username = val.toString();
            }
            found = true;
        }
        else found = false;
    }
    users_db.close();

    return found;
}

void NetworkServer::addCollaborator(QString nickname, QString username, QString filename) {
    QString folder_path = QDir::toNativeSeparators("documents/" + username + "/" + filename);

    // Adding data to collaborators.db.
    QString collaborators_db_path = QDir::toNativeSeparators(folder_path + "/collaborators.db");
    QSqlDatabase collaborators_db = QSqlDatabase::addDatabase("QSQLITE", collaborators_db_path);
    collaborators_db.setDatabaseName(collaborators_db_path);
    collaborators_db.open();
    {
        collaborators_db.transaction();
        QSqlQuery query(collaborators_db);
        bool done = query.exec("INSERT INTO COLLABORATORS(NICKNAME, COUNTER) VALUES('" + nickname + "', " + QString::number(0) + ")");
        if (!done) qDebug() << query.lastError();
        collaborators_db.commit();
    }
    collaborators_db.close();
    QSqlDatabase::removeDatabase(collaborators_db_path);
}

void NetworkServer::addPermission(QString collaborator_username, QString owner_nickname, QString file_path) {
    QFile file(file_path);
    QFileInfo fileInfo(file);
    QString filename(fileInfo.fileName());
    filename.replace(QString(".html"), QString(""), Qt::CaseInsensitive);

    QString permissions_db_path = QDir::toNativeSeparators("documents/" + collaborator_username + "/permissions.db");
    QSqlDatabase permissions_db = QSqlDatabase::addDatabase("QSQLITE", permissions_db_path);
    permissions_db.setDatabaseName(permissions_db_path);
    permissions_db.open(); // If it doesn't exists, it will be created.
    {
        permissions_db.transaction();
        QSqlQuery query = QSqlQuery(permissions_db);
        bool done = query.exec("INSERT INTO PERMISSIONS(OWNER, DOCUMENT) VALUES('" + owner_nickname + "', '" + filename + "')");
        if (!done) qDebug() << query.lastError();
        permissions_db.commit();
    }
    permissions_db.close();
    QSqlDatabase::removeDatabase(permissions_db_path);
}