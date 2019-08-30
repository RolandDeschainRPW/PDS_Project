//
// Created by raffa on 30/08/2019.
//

#ifndef NOTEPAD_SHAREDDOCUMENTSEXPLORER_H
#define NOTEPAD_SHAREDDOCUMENTSEXPLORER_H

#include <QDialog>
#include <QtNetwork>

#include "../include/Notepad.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SharedDocumentsExplorer;
}
QT_END_NAMESPACE

class SharedDocumentsExplorer : public QDialog {
    Q_OBJECT
    
public:
    explicit SharedDocumentsExplorer(QTcpSocket* clientConnection, QNetworkSession* networkSession, QMap<QString, QString> files_map, QString username, QWidget* parent = 0);
    ~SharedDocumentsExplorer();

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void askNewDocumentFilename();
    void openDocument();
    void readStartDataFromServer();

private:
    Ui::SharedDocumentsExplorer* ui = nullptr;
    Notepad* notepad = nullptr;
    QTcpSocket* clientConnection = nullptr;
    QNetworkSession* networkSession = nullptr;
    QMap<QString, QString> files_map;
    QString username;
    bool selected = false;
    QDataStream in;

    bool isThisFilenameAlreadyInUse(QString new_filename);
    void closeEvent(QCloseEvent* event) override;
    void showEditor(NetworkingData* startData);
};


#endif //NOTEPAD_SHAREDDOCUMENTSEXPLORER_H
