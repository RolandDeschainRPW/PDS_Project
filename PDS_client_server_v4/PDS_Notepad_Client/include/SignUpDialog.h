//
// Created by raffa on 28/08/2019.
//

#ifndef NOTEPAD_SIGNUPDIALOG_H
#define NOTEPAD_SIGNUPDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QImage>

#include "../include/NetworkingData.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SignUpDialog;
}
QT_END_NAMESPACE

class SignUpDialog : public QDialog {
    Q_OBJECT

public:
    explicit SignUpDialog(QTcpSocket* clientConnection, QWidget* parent = 0);
    ~SignUpDialog();

private slots:
    void signUp();
    void enableOkButton();
    void displayError(QAbstractSocket::SocketError socketError);
    void getSignUpResult();
    void changeProfilePic();

private:
    Ui::SignUpDialog* ui = nullptr;
    QTcpSocket* clientConnection;
    QDataStream in;
    QString profile_pic_path;

    void closeEvent(QCloseEvent* event) override;
};


#endif //NOTEPAD_SIGNUPDIALOG_H
