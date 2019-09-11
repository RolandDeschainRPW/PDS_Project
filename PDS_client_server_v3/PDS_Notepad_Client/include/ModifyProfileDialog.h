//
// Created by raffa on 10/09/2019.
//

#ifndef NOTEPAD_MODIFYPROFILEDIALOG_H
#define NOTEPAD_MODIFYPROFILEDIALOG_H

#include <QDialog>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui {
    class ModifyProfileDialog;
}
QT_END_NAMESPACE


class ModifyProfileDialog : public QDialog {
    Q_OBJECT

public:
    explicit ModifyProfileDialog(QTcpSocket* clientConnection, QNetworkSession* networkSession, QString username, QWidget* parent = 0);
    ~ModifyProfileDialog();

private slots:
    void enableOkButton();
    void displayError(QAbstractSocket::SocketError socketError);
    void changeProfilePic();
    void modifyProfile();
    void getModificationResult();

signals:
    void profilePicChanged();

private:
    Ui::ModifyProfileDialog* ui = nullptr;
    QTcpSocket* clientConnection = nullptr;
    QNetworkSession* networkSession = nullptr;
    QDataStream in;
    QString profile_pic_path = "";
    QString username;

    void closeEvent(QCloseEvent* event) override;
};


#endif //NOTEPAD_MODIFYPROFILEDIALOG_H
