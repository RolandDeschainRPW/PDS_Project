//
// Created by raffa on 10/09/2019.
//

#include <QMessageBox>
#include <QFileDialog>

#include "../include/ModifyProfileDialog.h"
#include "../include/Request.h"
#include "../include/Response.h"
#include "../include/SerialSize.h"

#include "ui_modifyprofiledialog.h"

ModifyProfileDialog::ModifyProfileDialog(QTcpSocket* clientConnection,
                                        QNetworkSession* networkSession,
                                        QString username,
                                        QWidget* parent) : QDialog(parent),
                                                        ui(new Ui::ModifyProfileDialog),
                                                        clientConnection(clientConnection),
                                                        networkSession(networkSession),
                                                        username(username) {
    ui->setupUi(this);
    connect(clientConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ModifyProfileDialog::displayError);
    connect(this, &QDialog::finished, this, &QObject::deleteLater);

    in.setDevice(clientConnection);
    in.setVersion(QDataStream::Qt_5_13);

    connect(clientConnection, &QIODevice::readyRead, this, [this, clientConnection] {
        in.startTransaction();

        qint64 size;
        in >> size;

        if(size > clientConnection->bytesAvailable()) {
            in.rollbackTransaction();
            in.abortTransaction();
            return;
        }

        QImage profile_pic;
        in >> profile_pic;

        if (!in.commitTransaction()) {
            qDebug().noquote() << "Something went wrong!\n\t-> I could not read the profile picture!";
            return;
        }

        QPixmap pixmap = QPixmap::fromImage(profile_pic);
        ui->imageLabel->setPixmap(pixmap);

        disconnect(clientConnection, &QIODevice::readyRead, this, 0);
        connect(clientConnection, &QIODevice::readyRead, this, &ModifyProfileDialog::getModificationResult);
    });

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(Request::SITE_ID_UNASSIGNED, Request::GET_PROFILE_PIC_TYPE, std::nullopt, username);

    // Calculating the size of the request in bytes.
    SerialSize size;
    qint64 req_size = size(req_to_send);

    out << req_size << req_to_send;
    clientConnection->write(block);

    connect(ui->cancelButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(ui->okButton, &QAbstractButton::clicked, this, &ModifyProfileDialog::modifyProfile);
    connect(ui->newImageButton, &QAbstractButton::clicked, this, &ModifyProfileDialog::changeProfilePic);
    connect(ui->newPasswordLineEdit, &QLineEdit::textChanged, this, &ModifyProfileDialog::enableOkButton);
    connect(ui->repeatPasswordLineEdit, &QLineEdit::textChanged, this, &ModifyProfileDialog::enableOkButton);
    connect(this, &ModifyProfileDialog::profilePicChanged, this, &ModifyProfileDialog::enableOkButton);
}

ModifyProfileDialog::~ModifyProfileDialog() {
    delete ui;
}

void ModifyProfileDialog::closeEvent(QCloseEvent* event) {
    disconnect(clientConnection, &QIODevice::readyRead, this, &ModifyProfileDialog::getModificationResult);
    disconnect(clientConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ModifyProfileDialog::displayError);
    QDialog::closeEvent(event);
}

void ModifyProfileDialog::getModificationResult() {
    in.startTransaction();

    Response res;
    in >> res;

    if (!in.commitTransaction()) {
        qDebug().noquote() << "Something went wrong!\n\t-> I could not read response from Server!";
        return;
    }

    if (res.getResult() == Response::PROFILE_MODIFIED) {
        QMessageBox::information(this, tr("Success!"), tr("Profile successfully modified."));
        this->close();
    } else if (res.getResult() == Response::PROFILE_PIC_NOT_STORED) {
        QMessageBox::information(this, tr("Failed!"), tr("Something went wrong!\nUpdate failed!"));
    } else if (res.getResult() == Response::PASSWORD_NOT_STORED) {
        QMessageBox::information(this, tr("Failed!"), tr("ONLY profile picture has been updated!\nPassword remained unchanged!"));
    }
}

void ModifyProfileDialog::modifyProfile() {
    bool newProfilePicChosen = QString::compare(profile_pic_path, "") != 0;

    QString new_password = "";
    Request req_to_send;

    // Verifying is the user's input is correct.
    if (QString::compare(ui->newPasswordLineEdit->text(), ui->repeatPasswordLineEdit->text(), Qt::CaseSensitive) != 0) {
        QMessageBox::information(this, tr("Reinsert the new password!"), tr("Be sure that New Password and Repeat Password are equal.\n"
                                                                            "Reinsert the data."));
        ui->okButton->setEnabled(true);
        return;
    }

    new_password = ui->newPasswordLineEdit->text();

    if (newProfilePicChosen) {
        QFileInfo fileInfo = QFileInfo(profile_pic_path);
        QString image_format = fileInfo.suffix();

        QImage profile_pic = QImage(profile_pic_path, image_format.toStdString().c_str());
        req_to_send = Request(Request::SITE_ID_UNASSIGNED,
                              Request::MODIFY_PROFILE_PIC_TYPE,
                              std::nullopt,
                              username,
                              new_password,
                              "", 0, "",
                              profile_pic,
                              image_format);
    } else {
        req_to_send = Request(Request::SITE_ID_UNASSIGNED,
                              Request::MODIFY_PASSWORD_TYPE,
                              std::nullopt,
                              username,
                              new_password,
                              "", 0, "");
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    // Calculating the size of the request in bytes.
    SerialSize size;
    qint64 req_size = size(req_to_send);

    out << req_size << req_to_send;
    clientConnection->write(block);

    ui->okButton->setEnabled(true);
}

void ModifyProfileDialog::changeProfilePic() {
    profile_pic_path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg)"));
    QImage profile_pic = QImage(profile_pic_path);
    QPixmap pixmap = QPixmap::fromImage(profile_pic);
    if (pixmap.height() > 150 ||
        pixmap.width() > 150) /* Resolution too high */ {
        QMessageBox::information(this, tr("Resolution too high!"), tr("Choose a compatible profile picture (max. 150x150)"));
        return;
    }

    ui->imageLabel->setPixmap(pixmap);
    emit profilePicChanged();
}

void ModifyProfileDialog::displayError(QAbstractSocket::SocketError socketError) {
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
                                     tr("The following error occurred: %1.").arg(clientConnection->errorString()));
    }
    this->close();
}

void ModifyProfileDialog::enableOkButton() {
    bool newPasswordInserted = !ui->newPasswordLineEdit->text().isEmpty() &&
                                !ui->repeatPasswordLineEdit->text().isEmpty();
    bool newProfilePicChosen = QString::compare(profile_pic_path, "") != 0;

    // Changing text on screen.
    if (!newPasswordInserted && !newProfilePicChosen)
        ui->statusLabel->setText(tr("Here you can modify your profile picture and/or your password."));
    else if (newPasswordInserted && !newProfilePicChosen)
        ui->statusLabel->setText(tr("The password will be changed."));
    else if (!newPasswordInserted && newProfilePicChosen)
        ui->statusLabel->setText(tr("The profile picture will be changed."));
    else if (newPasswordInserted && newProfilePicChosen)
        ui->statusLabel->setText(tr("Profile picture & password will be changed."));

    ui->okButton->setEnabled(newPasswordInserted || newProfilePicChosen);
}
