//
// Created by raffa on 28/08/2019.
//

#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QImageWriter>

#include "../include/SignUpDialog.h"
#include "../include/Request.h"
#include "../include/Response.h"
#include "../include/SerialSize.h"
#include "ui_signupdialog.h"

SignUpDialog::SignUpDialog(QTcpSocket* clientConnection,
                        QWidget* parent) : QDialog(parent),
                                        ui(new Ui::SignUpDialog),
                                        clientConnection(clientConnection) {
    ui->setupUi(this);

    profile_pic_path = QDir::toNativeSeparators(":/images/default_profile_pic.png");

    connect(this, &QDialog::finished, this, &QObject::deleteLater);
    connect(clientConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &SignUpDialog::displayError);
    connect(clientConnection, &QIODevice::readyRead, this, &SignUpDialog::getSignUpResult);

    in.setDevice(clientConnection);
    in.setVersion(QDataStream::Qt_5_13);

    connect(ui->cancelButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(ui->okButton, &QAbstractButton::clicked, this, &SignUpDialog::signUp);
    connect(ui->newImageButton, &QAbstractButton::clicked, this, &SignUpDialog::changeProfilePic);
    connect(ui->newUsernameLineEdit, &QLineEdit::textChanged, this, &SignUpDialog::enableOkButton);
    connect(ui->newPasswordLineEdit, &QLineEdit::textChanged, this, &SignUpDialog::enableOkButton);
    connect(ui->repeatUsernameLineEdit, &QLineEdit::textChanged, this, &SignUpDialog::enableOkButton);
    connect(ui->repeatPasswordLineEdit, &QLineEdit::textChanged, this, &SignUpDialog::enableOkButton);
    connect(ui->newNicknameLineEdit, &QLineEdit::textChanged, this, &SignUpDialog::enableOkButton);
}

SignUpDialog::~SignUpDialog() {
    delete ui;
}

void SignUpDialog::changeProfilePic() {
    profile_pic_path = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg)"));
    QImage tmp_img = QImage(profile_pic_path);
    QPixmap tmp_pixmap = QPixmap::fromImage(tmp_img);
    if (tmp_pixmap.height() > 150 ||
        tmp_pixmap.width() > 150) /* Resolution too high */ {
        QMessageBox::information(this, tr("Resolution too high!"), tr("Choose a compatible profile picture (max. 150x150)"));
        profile_pic_path = QDir::toNativeSeparators("images/default_profile_pic.png");
        return;
    }

    ui->imageLabel->setPixmap(tmp_pixmap);
}

void SignUpDialog::getSignUpResult() {
    in.startTransaction();

    qint64 size;
    in >> size;

    if(size > clientConnection->bytesAvailable()) {
        in.rollbackTransaction();
        in.abortTransaction();
        return;
    }

    Response res;
    in >> res;

    if (!in.commitTransaction()) {
        qDebug() << "Something went wrong!\n\t-> I could not complete SignUp process!";
        ui->okButton->setEnabled(true);
        return;
    }
    if (res.getResult() == Response::USERNAME_ALREADY_IN_USE) {
        QMessageBox::information(this, tr("Sign Up Information"), tr("Username already in use!\n"
                                                                     "Choose another username."));
        ui->okButton->setEnabled(true);
    } else if (res.getResult() == Response::NICKNAME_ALREADY_IN_USE) {
        QMessageBox::information(this, tr("Sign Up Information"), tr("Nickname already in use!\n"
                                                                     "Choose another nickname."));
        ui->okButton->setEnabled(true);
    } else /* Request::USERNAME_ACCEPTED */ {
        if (QMessageBox::information(this, tr("Sign Up Information"), tr("Username accepted!")) == QMessageBox::Ok)
            this->close();
    }
}

void SignUpDialog::signUp() {
    ui->okButton->setEnabled(false);

    // Verifying is the user's input is correct.
    if (QString::compare(ui->newUsernameLineEdit->text(), ui->repeatUsernameLineEdit->text(), Qt::CaseSensitive) == 0 &&
            QString::compare(ui->newPasswordLineEdit->text(), ui->repeatPasswordLineEdit->text(), Qt::CaseSensitive) == 0) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_13);

        QFileInfo fileInfo = QFileInfo(profile_pic_path);
        QString image_format = fileInfo.suffix();

        QImage profile_pic = QImage(profile_pic_path, image_format.toStdString().c_str());

        Request req_to_send = Request(Request::SITE_ID_UNASSIGNED,
                Request::SIGN_UP_TYPE, std::nullopt,
                ui->newUsernameLineEdit->text(),
                ui->newPasswordLineEdit->text(),
                "", 0, ui->newNicknameLineEdit->text(),
                profile_pic, image_format);

        // Calculating the size of the request in bytes.
        SerialSize size;
        qint64 req_size = size(req_to_send);

        out << req_size << req_to_send;
        clientConnection->write(block);
    } else {
        QMessageBox::information(this, tr("Sign Up Information"), tr("Be sure that New Username and Repeat Username are equal.\n"
                                                                     "Also New Password and Repeat Password must be equal.\n"
                                                                     "Reinsert the data."));
        ui->okButton->setEnabled(true);
    }
}

void SignUpDialog::enableOkButton() {
    ui->okButton->setEnabled(!ui->newPasswordLineEdit->text().isEmpty() &&
                            !ui->repeatPasswordLineEdit->text().isEmpty() &&
                            !ui->newUsernameLineEdit->text().isEmpty() &&
                            !ui->repeatUsernameLineEdit->text().isEmpty() &&
                            !ui->newNicknameLineEdit->text().isEmpty());
}

// Overriding the closeEvent() function to correctly disconnect from the Server.
void SignUpDialog::closeEvent(QCloseEvent* event) {
    clientConnection->disconnectFromHost();
    QDialog::closeEvent(event);
}

void SignUpDialog::displayError(QAbstractSocket::SocketError socketError) {
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
