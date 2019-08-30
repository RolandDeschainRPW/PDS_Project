//
// Created by raffa on 25/08/2019.
//

#ifndef NOTEPAD_STARTDIALOG_H
#define NOTEPAD_STARTDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include "../include/Notepad.h"
#include "../include/NetworkingData.h"
#include "../include/SignUpDialog.h"
#include "../include/SharedDocumentsExplorer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class StartDialog;
}
QT_END_NAMESPACE

class StartDialog : public QDialog {
    Q_OBJECT

public:
    explicit StartDialog(QWidget* parent = 0);
    ~StartDialog();

private slots:
    void connectToServer();
    void enableConnectToServerButton();
    void enableSignUpButton();
    void sessionOpened();
    void displayError(QAbstractSocket::SocketError socketError);
    void readStartDataFromServer();
    void openSignUpDialog();
    void openSharedDocumentsExplorer();

private:
    Ui::StartDialog* ui = nullptr;
    Notepad* notepad = nullptr;
    SignUpDialog* signUpDialog = nullptr;
    SharedDocumentsExplorer* explorer = nullptr;

    // Vars for networking.
    QTcpSocket* tcpSocket = nullptr;
    QNetworkSession* networkSession = nullptr;
    QDataStream in;

    void showEditor(NetworkingData* startData);
};

#endif //NOTEPAD_STARTDIALOG_H
