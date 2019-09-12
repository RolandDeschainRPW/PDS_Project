//
// Created by raffa on 30/08/2019.
//

#ifndef NOTEPAD_SHAREDDOCUMENTSEXPLORER_H
#define NOTEPAD_SHAREDDOCUMENTSEXPLORER_H

#include <QDialog>
#include <QtNetwork>

#include "../include/Notepad.h"
#include "../include/ModifyProfileDialog.h"
#include "../include/Collaborator.h"

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
    void addCollaborator();
    void openModifyProfileDialog();

private:
    Ui::SharedDocumentsExplorer* ui = nullptr;
    Notepad* notepad = nullptr;
    ModifyProfileDialog* mod_profile_dialog = nullptr;
    QTcpSocket* clientConnection = nullptr;
    QNetworkSession* networkSession = nullptr;
    QMap<QString, QString> files_map;
    QString username;
    bool selected = false;
    QDataStream in;

    bool isThisFilenameAlreadyInUse(QString new_filename);
    void closeEvent(QCloseEvent* event) override;
    void showEditor(NetworkingData* startData, QImage profile_pic, QString nickname, QVector<Collaborator> connected_editors);
};


#endif //NOTEPAD_SHAREDDOCUMENTSEXPLORER_H
