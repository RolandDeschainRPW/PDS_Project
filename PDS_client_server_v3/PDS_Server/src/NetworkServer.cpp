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

#include "../include/Response.h"

NetworkServer::NetworkServer(QString usersDbFileName,
                        QWidget *parent) : QDialog(parent),
                                        statusLabel(new QLabel),
                                        usersDbFileName(usersDbFileName) {
    // Initialiting directories.
    if (!QDir("documents").exists()) QDir().mkdir("documents");
    if (!QDir("profiles").exists()) QDir().mkdir("profiles");

    // Connecting to users database
    bool newly_created = !QFile(this->usersDbFileName).exists();
    QString users_db_path = QDir::toNativeSeparators(QDir().currentPath() + "/" + usersDbFileName);
    users_db = QSqlDatabase::addDatabase("QSQLITE");
    users_db.setDatabaseName(users_db_path);
    users_db.open(); // If it doesn't exists, it will be created.
    if (newly_created) {
        QSqlQuery query(users_db);
        query.exec("CREATE TABLE USERS("
                   "USERNAME VARCHAR(30) PRIMARY KEY,"
                   "PASSWORD VARCHAR(30) NOT NULL"
                   ")");
    }

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
    bool found = isThisUsernameInDatabase(username, &real_password);
    if (!found || QString::compare(password, real_password, Qt::CaseSensitive) == 0) {
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
        all_docs.insertMulti(filename, username);
    }

    // Listing all docs the user has access to.
    QString permissions_db_path = QDir::toNativeSeparators(QDir().currentPath() + "/documents/" + username + "/permissions.db");
    QSqlDatabase permissions_db = QSqlDatabase::addDatabase("QSQLITE");
    permissions_db.setDatabaseName(permissions_db_path);
    permissions_db.open();

    QSqlQuery query(permissions_db);
    query.exec("SELECT * FROM PERMISSIONS");
    QSqlRecord record = query.record();

    while(query.next()) {
        QString owner =  query.value(record.indexOf("OWNER")).toString();
        QString document =  query.value(record.indexOf("DOCUMENT")).toString();
        all_docs.insertMulti(document, owner);
    }

    // Telling the client that Login went well.
    out << Response(Response::SUCCESSFUL_LOGIN, all_docs, username);
    clientConnection->write(block);

    // Adding current user to active users.
    activeUsers.push_back(username);
}

// DA RIVEDERE!!!
void NetworkServer::readFromExistingConnection(QTcpSocket* clientConnection) {
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_5_13);
    in.startTransaction();

    Request next_req;
    in >> next_req;
    qint32 site_id = next_req.getSiteId();
    QString filename = next_req.getFilename();
    if (!in.commitTransaction()) {
        qDebug().noquote() << "Something went wrong!\n\t-> I could not read the incoming request!";
        return;
    }
    qDebug() << "I am going to process the request!";
    if (next_req.getRequestType() == Request::MESSAGE_TYPE) {
        Message msg = next_req.getMessage();
        qDebug() << "I am going to process the message!";
        this->getDocument(filename)->readMessage(msg);
    } else if (next_req.getRequestType() == Request::DISCONNECT_TYPE) {
        this->getDocument(filename)->disconnectClient(next_req.getSiteId());
    }
}

SharedDocument* NetworkServer::getDocument(QString filename) {
    foreach (SharedDocument* sd, openDocuments) {
        if (QString::compare(filename, sd->getFilename(), Qt::CaseInsensitive) == 0)
            return sd;
    }
    return nullptr;
}

void NetworkServer::readFromNewConnection() {
    QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    // I use this cycle to be sure to receive the data from Client.
    for (int retry = 2; retry > 0; retry--) {
        if (clientConnection->bytesAvailable()) {
            this->processNewConnections(clientConnection);
            break;
        } else if (retry == 2) {
            connect(clientConnection, &QIODevice::readyRead, this, [this, clientConnection] {
                this->processNewConnections(clientConnection);
            });
        }
    }
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
    qDebug() << "I am going to process the request!";
    if (next_req.getRequestType() == Request::CONNECT_TYPE) {
        qDebug() << "Welcoming the incoming client.";
        this->connectClient(clientConnection, next_req.getUsername(), next_req.getPassword());
    } else if (next_req.getRequestType() == Request::SIGN_UP_TYPE) {
        qDebug() << "Starting the SignUp procedure.";
        this->signUpNewUser(clientConnection, next_req.getUsername(), next_req.getPassword());
    } else if (next_req.getRequestType() == Request::OPEN_DOCUMENT_TYPE) {
        // do some stuff!
    } else if (next_req.getRequestType() == Request::NEW_DOCUMENT_TYPE) {
        // do some stuff!
        qDebug() << "Creating a directory for the new document.";
        this->createNewDocumentDirectory(next_req.getUsername(), next_req.getFilename());
        this->writeStartDataToClient(clientConnection, true, next_req.getFilename());
    }
}

// DA RIVEDERE!!
void NetworkServer::writeStartDataToClient(QTcpSocket* clientConnection, bool new_document, QString filename) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    if (new_document) {
        SharedDocument* doc = new SharedDocument(filename, this);
        qint32 site_id_assigned = doc->addSharedEditor(clientConnection);

        /* Useful for opening an existing document (so, to use later in development).
        if (site_id_assigned == SharedDocument::SITE_ID_UNASSIGNED) {
            // No Site Ids available.
            qDebug().noquote() << "I cannot host the incoming client!\n\t-> No more Site Ids available!";
            clientConnection->abort();
        }
        */

        out << site_id_assigned << doc->getSymbols();
        clientConnection->write(block);
        connect(clientConnection, &QIODevice::readyRead, this, [this, clientConnection] {
            while (clientConnection->bytesAvailable()) {
                qDebug() << "There is a request to read!";
                this->readFromExistingConnection(clientConnection);
            }
        });
        openDocuments.push_back(doc);
    } else /* Opening an existing document. */ {
        // do some stuff!
    }
}

void NetworkServer::createNewDocumentDirectory(QString username, QString filename) {
    QDir().mkdir(QDir::toNativeSeparators("documents/" + username + "/" + filename));
    QString new_file = QDir::toNativeSeparators("documents/" + username + "/" + filename + "/" + filename + ".html");
    QFile(new_file).open(QIODevice::ReadWrite);

    QString symbols_pos_data_path = QDir::toNativeSeparators("documents/" + username + "/" + filename + "/" + "symbols.txt");
    QFile(symbols_pos_data_path).open(QIODevice::ReadWrite);

    QString collaborators_db_path = QDir::toNativeSeparators(QDir().currentPath() + "/documents/" + username + "/" + filename + "/collaborators.db");
    QSqlDatabase collaborators_db = QSqlDatabase::addDatabase("QSQLITE");
    collaborators_db.setDatabaseName(collaborators_db_path);
    collaborators_db.open();
    QSqlQuery query(collaborators_db);
    query.exec("CREATE TABLE COLLABORATORS("
               "USERNAME VARCHAR(30) PRIMARY KEY,"
               "SITE_ID INTEGER NOT NULL,"
               "COUNTER INTEGER NOT NULL"
               ")");

    QString symbols_db_path = QDir::toNativeSeparators(QDir().currentPath() + "/documents/" + username + "/" + filename + "/symbols.db");
    QSqlDatabase symbols_db = QSqlDatabase::addDatabase("QSQLITE");
    symbols_db.setDatabaseName(symbols_db_path);
    symbols_db.open();
    query = QSqlQuery(symbols_db);
    query.exec("CREATE SYMBOLS("
               "CHARACTER VARCHAR(1) NOT NULL,"
               "SITE_ID INTEGER NOT NULL,"
               "COUNTER INTEGER NOT NULL,"
               "CONSTRAINT SYMBOLS_PK PRIMARY KEY (SITE_ID, COUNTER)"
               ")");

    // Database for those who no longer have access to the document
    // because the owner revoke the permission to access to it.
    QString old_collaborators_db_path = QDir::toNativeSeparators(QDir().currentPath() + "/documents/" + username + "/" + filename + "/old_collaborators.db");
    QSqlDatabase old_collaborators_db = QSqlDatabase::addDatabase("QSQLITE");
    old_collaborators_db.setDatabaseName(symbols_db_path);
    old_collaborators_db.open();
    query = QSqlQuery(old_collaborators_db);
    query.exec("CREATE TABLE OLD_COLLABORATORS("
               "USERNAME VARCHAR(30) PRIMARY KEY,"
               "SITE_ID INTEGER NOT NULL,"
               "COUNTER INTEGER NOT NULL"
               ")");
}

void NetworkServer::signUpNewUser(QTcpSocket* clientConnection, QString username, QString password) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    // Verifying if the chosen username is already in use.
    bool found = isThisUsernameInDatabase(username);
    if (found) {
        out << Response(Response::USERNAME_ALREADY_IN_USE, QMap<QString, QString>());
        clientConnection->write(block);
        return;
    }

    // Storing the new credentials.
    QSqlQuery query(users_db);
    query.exec("INSERT INTO USERS(USERNAME, PASSWORD) VALUES(" + username + ", " + password + ")");

    // Creating the user's documents folder.
    QString new_path = "documents/" + username;
    QDir().mkdir(QDir::toNativeSeparators(new_path));

    // Creating the user's permissions file.
    QString permissions_db_path = QDir::toNativeSeparators(QDir().currentPath() + "/documents/" + username + "/permissions.db");
    QSqlDatabase permissions_db = QSqlDatabase::addDatabase("QSQLITE");
    permissions_db.setDatabaseName(permissions_db_path);
    permissions_db.open(); // If it doesn't exists, it will be created.
    query = QSqlQuery(permissions_db);
    query.exec("CREATE TABLE PERMISSIONS("
               "OWNER VARCHAR(30) NOT NULL,"
               "DOCUMENT VARCHAR(30) NOT NULL"
               "CONSTRAINT PERMISSIONS_PK PRIMARY KEY (OWNER, DOCUMENT))");

    // Telling the client that SignUp went well.
    out << Response(Response::USERNAME_ACCEPTED, QMap<QString, QString>());
    clientConnection->write(block);
}

bool NetworkServer::isThisUsernameInDatabase(QString username, QString* password) {
    QSqlQuery query(users_db);
    query.exec("SELECT * FROM USERS WHERE USERNAME='" + username + "' COLLATE NOCASE");
    QSqlRecord record = query.record();

    if (query.next()) {
        if (password != nullptr) {
            QVariant val = query.value(record.indexOf("PASSWORD"));
            *password = QString(val.toString());
        }
        return true;
    } else return false;
}


/*
void NetworkServer::saveDocument() {
    //QFile file(this->fileName);
    QFile file("temp.rtf");
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString text = this->symbols_to_string();
    out << text;
    file.close();
}*/