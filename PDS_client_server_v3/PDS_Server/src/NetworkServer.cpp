//
// Created by raffa on 21/08/2019.
//

#include "../include/Request.h"
#include "../include/NetworkServer.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <Response.h>

NetworkServer::NetworkServer(QString usersListFileName,
                        QWidget *parent) : QDialog(parent),
                                        statusLabel(new QLabel),
                                        usersListFileName(usersListFileName) {
    // Initialiting directories.
    if (!QDir("documents").exists()) QDir().mkdir("documents");
    if (!QDir("profiles").exists()) QDir().mkdir("profiles");
    QFile("users.txt").open(QIODevice::ReadWrite); // If it doesn't exists, it will be created.

    // Initializing a map for free Site Ids.
    for (int i = 0; i < MAX_SITE_IDS; i++) free_site_ids.push_back(true);

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
    int found = searchUsername(username);
    if (found == NetworkServer::SEARCH_FAILED) {
        clientConnection->abort();
        return;
    } else if (found == NetworkServer::USERNAME_NOT_FOUND) {
        out << Response(Response::WRONG_CREDENTIALS, QUrl());
        clientConnection->write(block);
        return;
    } else /* found >= 0 */ {
        if (checkPassword(found, password) == NetworkServer::WRONG_PASSWORD) {
            out << Response(Response::WRONG_CREDENTIALS, QUrl());
            clientConnection->write(block);
            return;
        } else if (checkPassword(found, password) == NetworkServer::SEARCH_FAILED) {
            clientConnection->abort();
            return;
        }
    }

    // Telling the client that Login went well.
    out << Response(Response::SUCCESSFUL_LOGIN, QUrl(QDir::currentPath() + "/documents/" + username));
    clientConnection->write(block);

    // DA COMPLETARE!

    // DO NOT DELETE THE FOLLOWING CODE!!!
    // IT WILL BE USEFUL AT A LATER STAGE OF DEVELOPMENT!
    /*
    qint32 cnt = 0;

    foreach (bool free, this->free_site_ids) {
        if (free) {
            SharedEditor* new_se = new SharedEditor(clientConnection, cnt, this);
            sharedEditors.insert(sharedEditors.begin() + cnt, new_se);
            out << cnt << this->_symbols;
            clientConnection->write(block);
            connect(clientConnection, &QIODevice::readyRead, this, [this, clientConnection] {
                while (clientConnection->bytesAvailable()) {
                    qDebug() << "There is a request to read!";
                    this->readFromExistingConnection(clientConnection);
                }
            });
            this->free_site_ids[cnt] = false;
            qDebug() << "I assigned the following Site Id to the incoming client: " << cnt;
            return;
        }
        cnt++;
    }

    // No Site Ids available.
    qDebug().noquote() << "I cannot host the incoming client!\n\t-> No more Site Ids available!";
    clientConnection->abort();
     */
}

int NetworkServer::checkPassword(int lineIndex, QString password) {
    QFile file(usersListFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug().noquote() << "Something went wrong!\n\t-> I could not open the users data!";
        return NetworkServer::SEARCH_FAILED;
    }

    QTextStream in(&file);
    QString line;

    for (int i = 0; i <= lineIndex; i += 2) {
        in.readLine();
        line = in.readLine();
    }

    if (QString::compare(password, line, Qt::CaseSensitive) == 0)
        return lineIndex + 1; // index line of password.
    return NetworkServer::WRONG_PASSWORD;
}

void NetworkServer::readFromExistingConnection(QTcpSocket* clientConnection) {
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_5_13);
    in.startTransaction();

    Request next_req;
    in >> next_req;
    if (!in.commitTransaction()) {
        qDebug().noquote() << "Something went wrong!\n\t-> I could not read the incoming request!";
        return;
    }
    qDebug() << "I am going to process the request!";
    if (next_req.getRequestType() == Request::MESSAGE_TYPE) {
        Message msg = next_req.getMessage();
        qDebug() << "I am going to process the message!";
        this->readMessage(msg);
    } else if (next_req.getRequestType() == Request::DISCONNECT_TYPE) {
        this->disconnectClient(next_req.getSiteId());
    }
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
    }
}

void NetworkServer::signUpNewUser(QTcpSocket* clientConnection, QString username, QString password) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    // Verifying if the chosen username is already in use.
    int found = searchUsername(username);
    if (found == NetworkServer::SEARCH_FAILED) {
        clientConnection->abort();
        return;
    } else if (found >= 0) {
        out << Response(Response::USERNAME_ALREADY_IN_USE, QUrl());
        clientConnection->write(block);
        return;
    }

    // Storing the new credentials.
    QFile file(usersListFileName);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug().noquote() << "Something went wrong!\n\t-> I could not open the users database!";
        clientConnection->abort();
        return;
    }
    QTextStream to_file(&file);
    to_file << username << "\n" << password << "\n";

    // Telling the client that SignUp went well.
    out << Response(Response::USERNAME_ACCEPTED, QUrl());
    QString new_path = "documents/" + username;
    QDir().mkdir(QDir::toNativeSeparators(new_path));
    QFile(new_path + "/permissions.txt").open(QIODevice::ReadWrite); // Creating the permissions file.
    clientConnection->write(block);
}

int NetworkServer::searchUsername(QString username) {
    QFile file(usersListFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug().noquote() << "Something went wrong!\n\t-> I could not open the users data!";
        return NetworkServer::SEARCH_FAILED;
    }

    int lineIndex = 0;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        if (QString::compare(line, username, Qt::CaseInsensitive) == 0) {
            return lineIndex;
        }
        in.readLine(); // Ignoring the password line.
        line = in.readLine();
        lineIndex += 2;
    }
    return NetworkServer::USERNAME_NOT_FOUND;
}

void NetworkServer::disconnectClient(qint32 siteId) {
    qint32 cnt = 0;
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

void NetworkServer::dispatchMessages() {
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

void NetworkServer::readMessage(Message& msg) {
    this->processSymbol(msg);
    messages.push(msg);
    this->dispatchMessages();
}

void NetworkServer::processSymbol(const Message& msg) {
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
bool NetworkServer::comparePositions(std::optional<QVector<qint32>> pos1_opt,
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

QString NetworkServer::symbols_to_string() {
    QString str;
    foreach (Symbol s, _symbols) {
        str += s.getChar();
    }
    return str;
}

QString NetworkServer::positions_to_string() {
    QString pos_str = "";
    qint32 cnt = 0;
    foreach (Symbol s, _symbols) {
        pos_str.append(QString("[%1] ->").arg(cnt));
        foreach (qint32 digit, s.getPos()) pos_str.append(QString(" %1 ").arg(digit));
        pos_str.append("\n");
        cnt++;
    }
    return pos_str;
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