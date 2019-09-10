//
// Created by raffa on 25/08/2019.
//

#include <iostream>

#include <QtWidgets>
#include <QtNetwork>

#include "../include/StartDialog.h"
#include "../include/NetworkingData.h"
#include "../include/Request.h"
#include "../include/Response.h"
#include "../include/SerialSize.h"
#include "ui_startdialog.h"

StartDialog::StartDialog(QWidget* parent) : QDialog(parent),
                                        ui(new Ui::StartDialog),
                                        tcpSocket(new QTcpSocket(this)) {
    ui->setupUi(this);
    qDebug() << "Client started.";

    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        ui->hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            ui->hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        ui->hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            ui->hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            ui->hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    ui->portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    ui->portLineEdit->setFocus();

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_5_13);

    connect(ui->hostCombo, &QComboBox::editTextChanged, this, &StartDialog::enableConnectToServerButton);
    connect(ui->portLineEdit, &QLineEdit::textChanged, this, &StartDialog::enableConnectToServerButton);
    connect(ui->usernameLineEdit, &QLineEdit::textChanged, this, &StartDialog::enableConnectToServerButton);
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, this, &StartDialog::enableConnectToServerButton);

    connect(ui->hostCombo, &QComboBox::editTextChanged, this, &StartDialog::enableSignUpButton);
    connect(ui->portLineEdit, &QLineEdit::textChanged, this, &StartDialog::enableSignUpButton);

    connect(ui->connectToServerButton, &QAbstractButton::clicked, this, &StartDialog::connectToServer);
    connect(ui->quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(ui->signUpButton, &QAbstractButton::clicked, this, &StartDialog::openSignUpDialog);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &StartDialog::displayError);
    connect(tcpSocket, &QIODevice::readyRead, this, &StartDialog::openSharedDocumentsExplorer);

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
        connect(networkSession, &QNetworkSession::opened, this, &StartDialog::sessionOpened);

        ui->connectToServerButton->setEnabled(false);
        ui->signUpButton->setEnabled(false);
        ui->statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
}

StartDialog::~StartDialog() {
    delete ui;
}

void StartDialog::sessionOpened() {
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

    enableConnectToServerButton();
}

void StartDialog::displayError(QAbstractSocket::SocketError socketError) {
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
                                        "Make sure the server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("SharedEditor"),
                                     tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
    }
    ui->signUpButton->setEnabled(true);
    ui->connectToServerButton->setEnabled(true);
}

void StartDialog::connectToServer() {
    ui->connectToServerButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->hostCombo->currentText(), ui->portLineEdit->text().toInt());

    // We must consider also username and password!
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(Request::SITE_ID_UNASSIGNED, Request::CONNECT_TYPE, std::nullopt, ui->usernameLineEdit->text(), ui->passwordLineEdit->text());

    // Calculating the size of the request in bytes.
    SerialSize size;
    qint64 req_size = size(req_to_send);

    out << req_size << req_to_send;
    tcpSocket->write(block);
}

void StartDialog::enableConnectToServerButton() {
    ui->connectToServerButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                        !ui->hostCombo->currentText().isEmpty() &&
                                        !ui->portLineEdit->text().isEmpty() &&
                                        !ui->usernameLineEdit->text().isEmpty() &&
                                        !ui->passwordLineEdit->text().isEmpty());
}

void StartDialog::openSignUpDialog() {
    ui->signUpButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->hostCombo->currentText(), ui->portLineEdit->text().toInt());

    // Temporarily disconnecting these two signals
    disconnect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &StartDialog::displayError);
    disconnect(tcpSocket, &QIODevice::readyRead, this, &StartDialog::openSharedDocumentsExplorer);

    signUpDialog = new SignUpDialog(tcpSocket, this);
    connect(signUpDialog, &QDialog::finished, this, [this] {
        // Reconnecting signals.
        connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &StartDialog::displayError);
        connect(tcpSocket, &QIODevice::readyRead, this, &StartDialog::openSharedDocumentsExplorer);
    });

    signUpDialog->open();
    ui->signUpButton->setEnabled(true);
}

void StartDialog::openSharedDocumentsExplorer() {
    ui->connectToServerButton->setEnabled(false);
    in.startTransaction();
    Response res;
    in >> res;
    if (!in.commitTransaction()) {
        qDebug() << "Something went wrong!\n\t-> I could not complete Login process!";
        tcpSocket->abort();
        ui->connectToServerButton->setEnabled(true);
        return;
    }
    if (res.getResult() == Response::WRONG_CREDENTIALS) {
        QMessageBox::information(this, tr("Login Information"), tr("Wrong credentials!\n"
                                                                   "Please insert correct username and password."));
        tcpSocket->disconnectFromHost();
    } else if (res.getResult() == Response::USERNAME_ACTIVE) {
        QMessageBox::information(this, tr("Login Information"), tr("Username already connected to Server!"));
        tcpSocket->disconnectFromHost();
    } else /* Response::SUCCESSFUL_LOGIN */ {
        disconnect(tcpSocket, &QIODevice::readyRead, this, &StartDialog::openSharedDocumentsExplorer);
        explorer = new SharedDocumentsExplorer(tcpSocket, networkSession, res.getDocsList(), res.getUsername(), this);
        explorer->open();
        ui->connectToServerButton->setEnabled(true);
    }
}

void StartDialog::enableSignUpButton() {
    ui->signUpButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                !ui->hostCombo->currentText().isEmpty() &&
                                !ui->portLineEdit->text().isEmpty());
}