//
// Created by raffa on 21/08/2019.
//

#include <QtWidgets>
#include <QtNetwork>
#include <QtMath>
#include <QVector>
#include <QByteArray>

#include "SharedEditor.h"
#include "Request.h"

SharedEditor::SharedEditor(QWidget *parent, qint32 base, qint32 boundary, qint32 strategy) : QDialog(parent),
                                            hostCombo(new QComboBox),
                                            portLineEdit(new QLineEdit),
                                            connectToServerButton(new QPushButton(tr("Connect to Server"))),
                                            tcpSocket(new QTcpSocket(this)),
                                            base(base),
                                            boundary(boundary),
                                            strategy(strategy) {

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    hostCombo->setEditable(true);

    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));

    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    auto hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);
    auto portLabel = new QLabel(tr("&Server port:"));
    portLabel->setBuddy(portLineEdit);

    statusLabel = new QLabel(tr("This examples requires that you run the "
                                "Fortune Server example as well."));

    connectToServerButton->setDefault(true);
    connectToServerButton->setEnabled(false);

    auto quitButton = new QPushButton(tr("Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(connectToServerButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_5_13);

    connect(hostCombo, &QComboBox::editTextChanged, this, &SharedEditor::enableConnectToServerButton);
    connect(portLineEdit, &QLineEdit::textChanged, this, &SharedEditor::enableConnectToServerButton);
    connect(connectToServerButton, &QAbstractButton::clicked, this, &SharedEditor::connectToServer);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket, &QIODevice::readyRead, this, [this] {
        while (tcpSocket->bytesAvailable()) this->readDataFromSocket();
    });
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &SharedEditor::displayError);

    QGridLayout *mainLayout = nullptr;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QGridLayout(this);
    }
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &SharedEditor::sessionOpened);

        connectToServerButton->setEnabled(false);
        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
}

void SharedEditor::readDataFromSocket() {
    if (_siteId != SITE_ID_UNASSIGNED) {
        std::cout << "There is a message to read!" << std::endl;
        this->readMessage();
    }
    else { // Here the client receives the Site Id from the Server!
        in.startTransaction();
        in >> this->_siteId >> this->_symbols;
        if (!in.commitTransaction()) {
            std::cout << "Something went wrong!\n\t-> I could not have Site Id and Symbols from Server!" << std::endl;
            return;
        }
        std::cout << "I received the following Site Id: " << this->_siteId << std::endl;
        // Showing the Site Id in the GUI.
        std::string site_id_str = "Site Id: " + std::to_string(this->_siteId);
        this->statusLabel->setText(tr(site_id_str.c_str()));

        // Trying some editing for debugging purposes.
        // BE CAREFUL! Works only if the clients are exactly 2!

        if (this->_siteId == 0) {
            this->localInsert(0, 'c');
            this->localInsert(1, 'a');
            this->localInsert(2, 't');
        } else if (this->_siteId == 1) {
            this->localInsert(0, 'w');
            this->localInsert(1, 'h');
            this->localErase(2);
            this->localInsert(2, 'e');
        }
        this->statusLabel->setText(tr(this->symbols_to_string().toLocal8Bit().data()));

    }
}

void SharedEditor::connectToServer() {
    connectToServerButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(), portLineEdit->text().toInt());
}

void SharedEditor::readMessage() {
    in.startTransaction();

    Message next_msg;
    in >> next_msg;

    if (!in.commitTransaction()) {
        std::cout << "Something went wrong!\n\t-> I couldn't read the incoming message!" << std::endl;
        return;
    }

    std::cout << "I am going to process the message!" << std::endl;
    this->processSymbol(next_msg);

    // Showing the current symbols on standard output.
    std::string str = this->symbols_to_string().toStdString();
    std::cout << "Current symbols on this Client with Site Id " << std::to_string(this->_siteId) << ": " << str << std::endl;

    // Showing the current symbols on GUI.
    this->statusLabel->setText(tr(this->symbols_to_string().toLocal8Bit().data()));

    //connectToServerButton->setEnabled(true);
}

void SharedEditor::displayError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("SharedEditor"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("SharedEditor"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the fortune server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("SharedEditor"),
                                     tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
    }

    connectToServerButton->setEnabled(true);
}

void SharedEditor::enableConnectToServerButton() {
    connectToServerButton->setEnabled((!networkSession || networkSession->isOpen()) && !hostCombo->currentText().isEmpty() && !portLineEdit->text().isEmpty());
}

void SharedEditor::sessionOpened() {
    // Save the used configuration
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

    statusLabel->setText(tr("This examples requires that you run the Server example as well."));

    enableConnectToServerButton();
}

qint32 SharedEditor::getSiteId() {
    return _siteId;
}

qint32 SharedEditor::getCounter() {
    return _counter;
}

void SharedEditor::localInsert(qint32 index, QChar value) {
    Symbol s = this->generateSymbol(value, index);

    if (_symbols.empty())  _symbols.push_back(s);
    else if (index > static_cast<qint32>(_symbols.size() - 1)) _symbols.push_back(s);
    else if (index == 0) _symbols.insert(_symbols.begin(), s);
    else _symbols.insert(_symbols.begin() + index, s);

    _counter++;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    Request req_to_send = Request(this->_siteId, Request::MESSAGE_TYPE, Message(this->_siteId, Message::INSERT_TYPE, s));
    out << req_to_send;
    tcpSocket->write(block);
    std::cout << "I have sent an INSERT MESSAGE!" << std::endl;
}

void SharedEditor::localErase(qint32 index) {
    if (index >= static_cast<qint32>(_symbols.size())) return;

    Symbol tmp = _symbols.at(index);
    _symbols.erase(_symbols.begin() + index);

    _counter++;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << Request(this->_siteId, Request::MESSAGE_TYPE, Message(this->_siteId, Message::ERASE_TYPE, tmp));
    tcpSocket->write(block);
    std::cout << "I have sent an ERASE MESSAGE!" << std::endl;
}

void SharedEditor::processSymbol(const Message &m) {
    qint32 index = 0;
    if (m.getType() == Message::INSERT_TYPE) {
        if (_symbols.empty()) _symbols.push_back(m.getSymbol());
        else {
            for (Symbol s : _symbols) {
                if (this->comparePositions(s.getPos(), m.getSymbol().getPos())) {
                    _symbols.insert(_symbols.begin() + index, m.getSymbol());
                    return;
                }
                index++;
            }
            _symbols.push_back(m.getSymbol());
        }
    } else /* Message::ERASE_TYPE */ {
        for (Symbol s : _symbols){
            if (s.getChar() == m.getSymbol().getChar() && s.getId() == m.getSymbol().getId()) {
                _symbols.erase(_symbols.begin() + index);
                return;
            }
            index++;
        }
    }
}

QString SharedEditor::symbols_to_string() {
    QString str;
    for (Symbol s : _symbols) {
        str += s.getChar();
    }
    return str;
}

std::optional<Symbol> SharedEditor::getSymbol(qint32 index) {
    if (!_symbols.empty() && index >= 0 && index <= static_cast<qint32>(_symbols.size() - 1)) return _symbols.at(index);
    else return std::nullopt;
}

// Returns true if pos1 > pos2
bool SharedEditor::comparePositions(std::optional<QVector<qint32>> pos1_opt,
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

Symbol SharedEditor::generateSymbol(QChar value, qint32 index) {
    QVector<qint32> before = (this->getSymbol(index - 1).has_value()) ? this->getSymbol(index - 1).value().getPos() : QVector<qint32>();
    qint32 siteIdBefore = (this->getSymbol(index - 1).has_value()) ? this->getSymbol(index - 1).value().getSiteId() : this->getSiteId();

    QVector<qint32> after = (this->getSymbol(index).has_value()) ? this->getSymbol(index).value().getPos() : QVector<qint32>();
    qint32 siteIdAfter = (this->getSymbol(index).has_value()) ? this->getSymbol(index).value().getSiteId() : this->getSiteId();

    QVector<qint32> newPos = this->generatePosBetween(siteIdBefore, siteIdAfter, before, after, std::nullopt);
    return Symbol(value, this->getSiteId(), this->getCounter(), newPos);
}

QVector<qint32> SharedEditor::generatePosBetween(qint32 siteId1,
                                                  qint32 siteId2,
                                                  QVector<qint32> before,
                                                  std::optional<QVector<qint32>> after_opt,
                                                  std::optional<QVector<qint32>> newPos_opt,
                                                  qint32 level) {
    qint32 myBase = static_cast<qint32>(pow(2, level) * this->base);
    qint32 myStrategy = this->retrieveStrategy(level);

    QVector<qint32> after = (after_opt.has_value()) ? after_opt.value() : QVector<qint32>();
    QVector<qint32> newPos = (newPos_opt.has_value()) ? newPos_opt.value() : QVector<qint32>();

    qint32 id1 = (!before.empty()) ? before[0] : 0;
    qint32 id2 = (!after.empty()) ? after[0] : myBase;

    if (id2 - id1 > 1) {
        qint32 newDigit = this->generateIdBetween(id1, id2, myStrategy);
        newPos.push_back(newDigit);
        return newPos;
    } else if (id2 - id1 == 1) {
        newPos.push_back(id1);
        QVector<qint32> slice = (before.begin() + 1 < before.end()) ? before.mid(1) : QVector<qint32>();
        return this->generatePosBetween(siteId1, siteId2, slice, std::nullopt, newPos, level + 1);
    } else if (id1 == id2) {
        if (siteId1 < siteId2) {
            newPos.push_back(id1);
            QVector<qint32> slice = (before.begin() + 1 < before.end()) ? before.mid(1) : QVector<qint32>();
            return this->generatePosBetween(siteId1, siteId2, slice, std::nullopt, newPos, level + 1);
        } else if (siteId1 == siteId2) {
            newPos.push_back(id1);
            QVector<qint32> slice1 = (before.begin() + 1 < before.end()) ? before.mid(1) : QVector<qint32>();
            QVector<qint32> slice2 = (after.begin() + 1 < after.end()) ? after.mid(1) : QVector<qint32>();
            return this->generatePosBetween(siteId1, siteId2, slice1, slice2, newPos, level + 1);
        } else {
            // Da rivedere!
            // "Fix Position Sorting"
            throw std::exception();
        }
    }
}

qint32 SharedEditor::generateIdBetween(qint32 min, qint32 max, qint32 myStrategy) {
    if ((max - min) < boundary) {
        min++;
    } else {
        if (myStrategy == BOUNDARY_MINUS) {
            min = max - boundary;
        } else {
            min++;
            max = min + boundary;
        }
    }

    // Random number between 0.0 (inclusive) and 1.0 (exclusive).
    qreal tmp = static_cast<qreal>(QRandomGenerator::global()->bounded(1.0));
    return static_cast<qint32>(qFloor(tmp * (max - min)) + min);
}

qint32 SharedEditor::retrieveStrategy(qint32 level) {
    if (!strategyCache.empty() && level <= static_cast<qint32>(strategyCache.size() - 1)) return strategyCache[level];

    qint32 myStrategy;

    switch (this->strategy) {
        case BOUNDARY_PLUS: {
            myStrategy = BOUNDARY_PLUS;
            break;
        }
        case BOUNDARY_MINUS: {
            myStrategy = BOUNDARY_MINUS;
            break;
        }
        case RANDOM: {
            // Random number between 0.0 (inclusive) and 1.0 (exclusive).
            double tmp = QRandomGenerator::global()->bounded(1.0);
            myStrategy = round(tmp) == 0 ? BOUNDARY_PLUS : BOUNDARY_MINUS;
            break;
        }
        default: {
            myStrategy = (level % 2) == 0 ? BOUNDARY_PLUS : BOUNDARY_MINUS;
            break;
        }
    }

    strategyCache.push_back(myStrategy);
    return myStrategy;
}

// Overriding the QDialog::closeEvent() function to correctly disconnect from the Server.
void SharedEditor::closeEvent(QCloseEvent* event) {
    // Knowing how to handle the disconnection from Server.
    connect(tcpSocket, &QAbstractSocket::disconnected, tcpSocket, [this, event] {
        tcpSocket->abort();
        std::cout << "DISCONNECTED!" << std::endl;
        QDialog::closeEvent(event);
    });

    // I am going to warn the Server that I want to disconnect.
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(this->_siteId, Request::DISCONNECT_TYPE, std::nullopt);
    out << req_to_send;
    tcpSocket->write(block);
    std::cout << "I am going to DISCONNECT!" << std::endl;
}
