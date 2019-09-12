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
#include "../include/SerialSize.h"

NetworkServer::NetworkServer(QString usersDbFileName,
                        QWidget *parent) : QDialog(parent),
                                        statusLabel(new QLabel) {
    // Initialiting directories.
    if (!QDir("documents").exists()) QDir().mkdir("documents");
    if (!QDir("profiles_pics").exists()) QDir().mkdir("profiles_pics");

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
        Response res = Response(Response::WRONG_CREDENTIALS);

        // Calculating response size.
        SerialSize size;
        qint64 res_size = size(res);

        out << res_size << res;
        clientConnection->write(block);
        return;
    }

    // Verifying if the username is already active.
    foreach (QString usr, activeUsers) {
        if (QString::compare(usr, username, Qt::CaseInsensitive) == 0) {
            Response res = Response(Response::USERNAME_ACTIVE);

            // Calculating response size.
            SerialSize size;
            qint64 res_size = size(res);

            out << res_size << res;
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
    Response res = Response(Response::SUCCESSFUL_LOGIN, all_docs, username);

    // Calculating response size.
    SerialSize size;
    qint64 res_size = size(res);

    out << res_size << res;
    clientConnection->write(block);
}

// It returns false when the socket has been disconnected.
bool NetworkServer::readFromExistingConnection(Request& req) {
    qint32 site_id = req.getSiteId();
    quint32 counter = req.getCounter();
    QString file_path = req.getFilename();

    if (req.getRequestType() == Request::MESSAGE_TYPE) {
        Message msg = req.getMessage();
        this->getDocument(file_path)->readMessage(msg);
        return true;
    } else if (req.getRequestType() == Request::DISCONNECT_TYPE) {
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
        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_5_13);
        in.startTransaction();

        qint64 size;
        in >> size;

        if(size > clientConnection->bytesAvailable()) {
            in.rollbackTransaction();
            in.abortTransaction();
            return;
        }

        Request next_req;
        in >> next_req;

        if (!in.commitTransaction()) {
            qDebug().noquote() << "Something went wrong!\n\t-> I could not read the incoming request!";
            clientConnection->abort();
            return;
        }

        this->processNewConnections(clientConnection, next_req);
    });
}

void NetworkServer::processNewConnections(QTcpSocket* clientConnection, Request& req) {
    if (req.getRequestType() == Request::CONNECT_TYPE) {
        this->connectClient(clientConnection, req.getUsername(), req.getPassword());
    } else if (req.getRequestType() == Request::SIGN_UP_TYPE) {
        this->signUpNewUser(clientConnection, req.getUsername(), req.getPassword(), req.getNickname(), req.getProfilePic(), req.getImageFormat());
    } else if (req.getRequestType() == Request::OPEN_DOCUMENT_TYPE) {
        // Retrieving the document's owner username by its nickname.
        QString owner_username;
        this->isThisNicknameInDatabase(req.getNickname(), &owner_username);

        // Retrieving the opener's nickname.
        QString opener_nickname;
        this->isThisUsernameInDatabase(req.getUsername(), nullptr, &opener_nickname);

        this->writeStartDataToClient(clientConnection, false, req.getFilename(), owner_username, opener_nickname);
    } else if (req.getRequestType() == Request::NEW_DOCUMENT_TYPE) {
        this->createNewDocumentDirectory(req.getUsername(), req.getFilename());
        this->writeStartDataToClient(clientConnection, true, req.getFilename(), req.getUsername());
    } else if (req.getRequestType() == Request::ADD_COLLABORATOR_TYPE) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_13);
        qint32 result;
        QString collaborator_username;
        QString owner_nickname;

        if (!this->isThisNicknameInDatabase(req.getNickname(), &collaborator_username))
            result = Response::NICKNAME_NON_EXISTENT;
        else {
            this->isThisUsernameInDatabase(req.getUsername(), nullptr, &owner_nickname);
            bool done = this->addCollaborator(req.getNickname(), req.getUsername(), req.getFilename());
            if (done) {
                this->addPermission(collaborator_username, owner_nickname, req.getFilename());
                result = Response::NICKNAME_ACTIVE;
            }
            else result = Response::TOO_MANY_COLLABORATORS;
        }

        Response res = Response(result);

        // Calculating response size.
        SerialSize size;
        qint64 res_size = size(res);

        out << res_size << res;
        clientConnection->write(block);
    } else if (req.getRequestType() == Request::GET_PROFILE_PIC_TYPE) {
        this->sendProfilePic(clientConnection, req.getUsername());
    } else if (req.getRequestType() == Request::MODIFY_PASSWORD_TYPE) {
        this->modifyProfile(clientConnection,
                            req.getUsername(),
                            req.getImageFormat(),
                            std::nullopt,
                            req.getPassword());
    } else if (req.getRequestType() == Request::MODIFY_PROFILE_PIC_TYPE) {
        this->modifyProfile(clientConnection,
                            req.getUsername(),
                            req.getImageFormat(),
                            req.getProfilePic(),
                            req.getPassword());
    }
}

QImage NetworkServer::searchProfilePic(QString username, QString* image_format) {
    QString profile_pic_path = "";

    QDirIterator it("profiles_pics", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if (QFileInfo(it.filePath()).isFile())
            if (QFileInfo(it.filePath()).fileName().contains(username, Qt::CaseInsensitive)) {
                profile_pic_path = it.filePath();
                if (image_format != nullptr) *image_format = QFileInfo(it.filePath()).suffix();
                break;
            }
    }

    return QImage(QDir::toNativeSeparators(profile_pic_path));
}

void NetworkServer::sendProfilePic(QTcpSocket* clientConnection, QString username) {
    QImage profile_pic = this->searchProfilePic(username);

    // Calculating the size of the image.
    SerialSize size;
    qint64 img_size = size(profile_pic);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    out << img_size << profile_pic;
    clientConnection->write(block);
}

void NetworkServer::modifyProfile(QTcpSocket* clientConnection, QString username, QString image_format, std::optional<QImage> opt_profile_pic, QString new_password) {
    qint32 result;
    QImage tmp_img;
    QString tmp_img_format;
    bool failed = false;

    if (opt_profile_pic.has_value()) {
        QImage profile_pic = opt_profile_pic.value();

        // Deleting old profile pic.
        QDirIterator it("profiles_pics", QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            if (QFileInfo(it.filePath()).isFile())
                if (QFileInfo(it.filePath()).fileName().contains(username, Qt::CaseInsensitive)) {
                    tmp_img_format = QFileInfo(it.filePath()).suffix();
                    tmp_img.load(it.filePath(), tmp_img_format.toStdString().c_str());
                    failed = !QFile::remove(it.filePath());
                    if (failed) {
                        qDebug().noquote() << "Something went wrong!\n\t-> I could not delete old profile picture!";
                        result = Response::PROFILE_PIC_NOT_STORED;
                    }
                    break;
                }
        }

        // Storing the new profile picture.
        QString profile_pic_path = QDir::toNativeSeparators("profiles_pics/" + username + "." + image_format);
        failed = !profile_pic.save(profile_pic_path, image_format.toStdString().c_str());
        if (failed) {
            qDebug().noquote() << "Something went wrong!\n\t-> I could not save the profile picture!";
            QString old_path = QDir::toNativeSeparators("profiles_pics/" + username + "." + tmp_img_format);
            tmp_img.save(old_path, tmp_img_format.toStdString().c_str());
            result = Response::PROFILE_PIC_NOT_STORED;
        }
    }

    if (QString::compare(new_password, "") != 0 && !failed) /* Updating the password. */ {
        QSqlDatabase users_db = QSqlDatabase::database("users_db_conn");
        users_db.open();
        {
            users_db.transaction();
            QSqlQuery query(users_db);
            bool done = query.exec("UPDATE USERS SET PASSWORD='" + new_password + "' WHERE USERNAME='" + username + "'");
            if (!done) {
                qDebug() << query.lastError();
                result = Response::PASSWORD_NOT_STORED;
                failed = true;
            }
            users_db.commit();
        }
        users_db.close();
    }

    if (!failed) result = Response::PROFILE_MODIFIED;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Response res = Response(result);

    // Calculating response size.
    SerialSize size;
    qint64 res_size = size(res);

    out << res_size << res;
    clientConnection->write(block);
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
    QString opener_username;
    QImage profile_pic;
    QString nickname_to_send;
    QVector<Collaborator> connected_editors;

    // Retrieving the nickname corresponding to the owner username.
    this->isThisUsernameInDatabase(owner_username, nullptr, &owner_nickname);

    if (new_document) {
        doc = new SharedDocument(file_path, folder_path, this);
        this->addCollaborator(owner_nickname, owner_username, filename);
        doc->addSharedEditor(clientConnection, owner_nickname, &site_id_assigned, &counter);
        connected_editors = this->getConnectedCollaborators(doc);
        profile_pic = this->searchProfilePic(owner_username);
        nickname_to_send = owner_nickname;
        openDocuments.push_back(doc);
    } else /* Opening an existing document. */ {
        doc = this->getDocument(file_path);

        if (doc == nullptr) { /* Document not already opened. */
            doc = new SharedDocument(file_path, folder_path, this);
            openDocuments.push_back(doc);
        }

        // Retrieving the username corresponding to the opener nickname.
        this->isThisNicknameInDatabase(opener_nickname, &opener_username);

        doc->addSharedEditor(clientConnection, opener_nickname, &site_id_assigned, &counter);
        connected_editors = this->getConnectedCollaborators(doc);
        profile_pic = this->searchProfilePic(opener_username);
        nickname_to_send = opener_nickname;
        doc->notifyIncomingUser(site_id_assigned, profile_pic, nickname_to_send);
        if (site_id_assigned == SharedDocument::SITE_ID_UNASSIGNED) /* No more Site Ids available. */ {
            qDebug().noquote() << "I cannot host the incoming client!\n\t-> No more Site Ids available!";
            clientConnection->abort();
            return;
        }
    }

    Response res = Response(Response::START_DATA,
                            std::nullopt, "",
                            site_id_assigned,
                            counter,
                            file_path,
                            doc->getSymbols(),
                            nickname_to_send,
                            profile_pic,
                            connected_editors);

    // Calculating the size of the image.
    SerialSize size;
    qint64 res_size = size(res);

    out << res_size << res;
    clientConnection->write(block);

    disconnect(clientConnection, &QIODevice::readyRead, this, 0);
    connect(clientConnection, &QIODevice::readyRead, this, [this, clientConnection] {
        while (clientConnection->bytesAvailable()) {
            QDataStream in(clientConnection);
            in.setVersion(QDataStream::Qt_5_13);
            in.startTransaction();

            qint64 size;
            in >> size;

            if (size > clientConnection->bytesAvailable()) {
                in.rollbackTransaction();
                in.abortTransaction();
                return;
            }

            Request next_req;
            in >> next_req;

            if (!in.commitTransaction()) {
                qDebug().noquote() << "Something went wrong!\n\t-> I could not read the incoming request!";
                return;
            }

            // When the socket has been disconnected, the cycle gets broken.
            bool still_connected = this->readFromExistingConnection(next_req);
            if (!still_connected) break;
        }
    });
}

QVector<Collaborator> NetworkServer::getConnectedCollaborators(SharedDocument* document) {
    QVector<Collaborator> collaborators;

    // typedef for using foreach macro.
    typedef QPair<qint32, QString> CollaboratorPair;

    QVector<CollaboratorPair> connected_editors = document->getConnectedCollaboratorsNicknames();

    foreach (CollaboratorPair pair, connected_editors) {
        QString username;

        // Searching profile pic.
        this->isThisNicknameInDatabase(pair.second, &username);
        QImage profile_pic = this->searchProfilePic(username);

        collaborators.push_back(Collaborator(pair.second, profile_pic, pair.first));
    }

    return collaborators;
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

void NetworkServer::signUpNewUser(QTcpSocket* clientConnection, QString username, QString password, QString nickname, QImage profile_pic, QString image_format) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    // Verifying if the chosen username is already in use.
    bool username_found = this->isThisUsernameInDatabase(username);
    if (username_found) {
        Response res = Response(Response::USERNAME_ALREADY_IN_USE);

        // Calculating response size.
        SerialSize size;
        qint64 res_size = size(res);

        out << res_size << res;
        clientConnection->write(block);
        return;
    }

    // Verifying if the chosen nickname is already in use.
    bool nickname_found = this->isThisNicknameInDatabase(nickname);
    if (nickname_found) {
        Response res = Response(Response::NICKNAME_ALREADY_IN_USE);

        // Calculating response size.
        SerialSize size;
        qint64 res_size = size(res);

        out << res_size << res;
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

    // Storing the profile picture.
    QString profile_pic_path = QDir::toNativeSeparators("profiles_pics/" + username + "." + image_format);
    profile_pic.save(profile_pic_path, image_format.toStdString().c_str());

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
    Response res = Response(Response::USERNAME_ACCEPTED);

    // Calculating response size.
    SerialSize size;
    qint64 res_size = size(res);

    out << res_size << res;
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

bool NetworkServer::addCollaborator(QString nickname, QString username, QString filename) {
    QString folder_path = QDir::toNativeSeparators("documents/" + username + "/" + filename);

    bool done;

    // Adding data to collaborators.db.
    QString collaborators_db_path = QDir::toNativeSeparators(folder_path + "/collaborators.db");
    QSqlDatabase collaborators_db = QSqlDatabase::addDatabase("QSQLITE", collaborators_db_path);
    collaborators_db.setDatabaseName(collaborators_db_path);
    collaborators_db.open();

    {
        QSqlQuery query(collaborators_db);
        done = query.exec("SELECT COUNT(*) FROM COLLABORATORS");
        if (!done) qDebug() << query.lastError();
        else {
            QVariant val = query.value(0);
            qint32 count = val.toInt();
            if (count > 4) done = false; // Max. 4 collaborators per document.
        }
    }

    if (done) {
        collaborators_db.transaction();
        QSqlQuery query(collaborators_db);
        done = query.exec("INSERT INTO COLLABORATORS(NICKNAME, COUNTER) VALUES('" + nickname + "', " + QString::number(0) + ")");
        if (!done) qDebug() << query.lastError();
        collaborators_db.commit();
    }

    collaborators_db.close();
    QSqlDatabase::removeDatabase(collaborators_db_path);
    return done;
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