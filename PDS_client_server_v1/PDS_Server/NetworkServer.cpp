//
// Created by raffa on 21/08/2019.
//

#include "Request.h"
#include "NetworkServer.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

NetworkServer::NetworkServer(QWidget *parent) : QDialog(parent), statusLabel(new QLabel) {
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
    connect(tcpServer, &QTcpServer::newConnection, this, &NetworkServer::connectClient);

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

void NetworkServer::connectClient() {
    QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    qint32 cnt = 0;

    foreach (bool free, this->free_site_ids) {
        if (free) {
            SharedEditor* new_se = new SharedEditor(clientConnection, cnt);
            sharedEditors.insert(sharedEditors.begin() + cnt, new_se);
            out << cnt << this->_symbols;
            clientConnection->write(block);
            connect(clientConnection, &QIODevice::readyRead, this, [this, clientConnection] {
                while (clientConnection->bytesAvailable()) {
                    std::cout << "There is a request to read!" << std::endl;
                    this->readRequest(clientConnection);
                }
            });
            this->free_site_ids[cnt] = false;
            std::cout << "I assigned the following Site Id to the incoming client: " << cnt << std::endl;
            //connect(clientConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::displayError);
            return;
        }
        cnt++;
    }

    // No Site Ids available.
    std::cout << "I cannot host the incoming client!\n\t-> No more Site Ids available!" << std::endl;
    clientConnection->abort();
}

void NetworkServer::readRequest(QTcpSocket* clientConnection) {
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_5_13);
    in.startTransaction();

    Request next_req;
    in >> next_req;
    if (!in.commitTransaction()) {
        std::cout << "Something went wrong!\n\t-> I could not read the incoming request!" << std::endl;
        return;
    }
    std::cout << "I am going to process the request!" << std::endl;
    if (next_req.getRequestType() == Request::MESSAGE_TYPE) {
        Message msg = next_req.getMessage();
        std::cout << "I am going to process the message!" << std::endl;
        this->readMessage(msg);
    } else /* Request::DISCONNECT_TYPE */ {
        this->disconnectClient(next_req.getSiteId());
    }
}

void NetworkServer::disconnectClient(qint32 siteId) {
    qint32 cnt = 0;
    foreach (SharedEditor* se, sharedEditors) {
        if (se->getSiteId() == siteId) {
            this->sharedEditors[cnt]->getClientConnection()->disconnectFromHost();
            this->sharedEditors.erase(sharedEditors.begin() + cnt);
            this->free_site_ids[siteId] = true;
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
                std::cout << "Message sent!" << std::endl;
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
            for (Symbol s : _symbols) {
                if (this->comparePositions(s.getPos(), msg.getSymbol().getPos())) {
                    _symbols.insert(_symbols.begin() + index, msg.getSymbol());
                    std::string str = this->symbols_to_string().toStdString();
                    std::cout << "Current symbols on Server: " << str << std::endl;
                    return;
                }
                index++;
            }
            _symbols.push_back(msg.getSymbol());
        }
        std::cout << "I have INSERTED a symbol!" << std::endl;
    } else /* Message::ERASE_TYPE */ {
        for (Symbol s : _symbols){
            if (s.getChar() == msg.getSymbol().getChar() && s.getId() == msg.getSymbol().getId()) {
                _symbols.erase(_symbols.begin() + index);
                std::cout << "I have ERASED a symbol!" << std::endl;
                std::string str = this->symbols_to_string().toStdString();
                std::cout << "Current symbols on Server: " << str << std::endl;
                return;
            }
            index++;
        }
        std::cout << "ERASING already done!" << std::endl;
    }
    std::string str = this->symbols_to_string().toStdString();
    std::cout << "Current symbols on Server: " << str << std::endl;
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
    for (Symbol s : _symbols) {
        str += s.getChar();
    }
    return str;
}