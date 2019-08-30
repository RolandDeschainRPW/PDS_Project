//
// Created by raffa on 30/08/2019.
//

#ifndef NOTEPAD_SHAREDDOCUMENTSEXPLORER_H
#define NOTEPAD_SHAREDDOCUMENTSEXPLORER_H

#include <QDialog>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SharedDocumentsExplorer;
}
QT_END_NAMESPACE

class SharedDocumentsExplorer : public QDialog {
    Q_OBJECT
    
public:
    explicit SharedDocumentsExplorer(QTcpSocket* clientConnection, QWidget* parent = 0);
    ~SharedDocumentsExplorer();

private slots:
    void displayError(QAbstractSocket::SocketError socketError);

private:
    Ui::SharedDocumentsExplorer* ui = nullptr;
    QTcpSocket* clientConnection;
    bool selected = false;

    void closeEvent(QCloseEvent* event) override;
};


#endif //NOTEPAD_SHAREDDOCUMENTSEXPLORER_H
