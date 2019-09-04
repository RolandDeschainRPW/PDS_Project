//
// Created by raffa on 30/08/2019.
//

#include <QMessageBox>
#include <QInputDialog>

#include "../include/Request.h"
#include "../include/SharedDocumentsExplorer.h"
#include "../include/DocumentsModel.h"
#include "../include/NetworkingData.h"
#include "../include/StartDialog.h"
#include "ui_shareddocumentsexplorer.h"

SharedDocumentsExplorer::SharedDocumentsExplorer(QTcpSocket* clientConnection,
                                            QNetworkSession* networkSession,
                                            QMap<QString, QString> files_map,
                                            QString username,
                                            QWidget* parent) : QDialog(parent),
                                                            ui(new Ui::SharedDocumentsExplorer),
                                                            files_map(files_map),
                                                            username(username),
                                                            clientConnection(clientConnection),
                                                            networkSession(networkSession) {
    ui->setupUi(this);
    connect(clientConnection, &QIODevice::readyRead, this, &SharedDocumentsExplorer::readStartDataFromServer);
    connect(clientConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &SharedDocumentsExplorer::displayError);
    connect(this, &QDialog::finished, this, &QObject::deleteLater);

    in.setDevice(clientConnection);
    in.setVersion(QDataStream::Qt_5_13);

    // Create some data that is tabular in nature:
    QList<QString> documents_filenames;
    QList<QString> documents_owners;
    QMapIterator<QString, QString> it(files_map);
    while (it.hasNext()) {
        it.next();
        documents_filenames.append(it.key()); // filename
        documents_owners.append(it.value()); // owner
    }

    // Create model:
    DocumentsModel* docs_model = new DocumentsModel(this);

    // Populate model with data:
    docs_model->populateData(documents_filenames, documents_owners);

    // Connect model to table view:
    ui->documentsView->setModel(docs_model);

    // Make table header visible and display table:
    ui->documentsView->horizontalHeader()->setVisible(true);
    ui->documentsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->documentsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->documentsView->show();

    connect(ui->cancelButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(ui->newDocumentButton, &QAbstractButton::clicked, this, &SharedDocumentsExplorer::askNewDocumentFilename);
    connect(ui->openDocumentButton, &QAbstractButton::clicked, this, &SharedDocumentsExplorer::openDocument);
}

SharedDocumentsExplorer::~SharedDocumentsExplorer() {
    delete ui;
}

void SharedDocumentsExplorer::askNewDocumentFilename() {
    bool ok;
    QString new_filename;
    while (true) {
        new_filename = QInputDialog::getText(this,
                                             tr("Create new document"),
                                             tr("Insert new document's name:"),
                                             QLineEdit::Normal,
                                             QDir::home().dirName(),
                                             &ok);
        if (ok && !new_filename.isEmpty()) {
            // We must consider also username and password!
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_13);
            if (isThisFilenameAlreadyInUse(new_filename)) continue;
            Request req_to_send = Request(Request::SITE_ID_UNASSIGNED, Request::NEW_DOCUMENT_TYPE, std::nullopt, username, "", new_filename);
            out << req_to_send;
            clientConnection->write(block);
            selected = true;
            return;
        } else if (ok && new_filename.isEmpty()) {
            QMessageBox::information(this, tr("Missing name!"),
                                     tr("Please insert a name for the new document."));
        } else return;
    }
}

bool SharedDocumentsExplorer::isThisFilenameAlreadyInUse(QString new_filename) {
    DocumentsModel* model = dynamic_cast<DocumentsModel*>(ui->documentsView->model());
    int rows = model->rowCount();
    for (int i = 0; i < rows; i++) {
        QModelIndex index = model->index(i, 0, QModelIndex());
        QString filename = ui->documentsView->model()->data(index).toString();
        index = model->index(i, 1, QModelIndex());
        QString owner = ui->documentsView->model()->data(index).toString();

        if (QString::compare(new_filename, filename, Qt::CaseInsensitive) == 0 &&
            QString::compare(username, owner, Qt::CaseInsensitive) == 0) {
            QMessageBox::information(this, tr("Name already in use!"),
                                     tr("Please choose another name for the new document."));
            return true;
        }
    }
    return false;
}

void SharedDocumentsExplorer::openDocument() {

}

void SharedDocumentsExplorer::closeEvent(QCloseEvent* event) {
    if (!selected) {
        qDebug().noquote() << "No document selected.\n\t-> Disconnecting from Server.";
        StartDialog* parent = dynamic_cast<StartDialog*>(this->parent());
        connect(clientConnection, &QIODevice::readyRead, parent, &StartDialog::openSharedDocumentsExplorer);
        clientConnection->disconnectFromHost();
    }
    QDialog::closeEvent(event);
}

void SharedDocumentsExplorer::displayError(QAbstractSocket::SocketError socketError) {
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

void SharedDocumentsExplorer::readStartDataFromServer() {
    in.startTransaction();
    qint32 siteId;
    quint32 counter;
    QVector<Symbol> symbols;
    in >> siteId >> counter >> symbols;
    if (!in.commitTransaction()) {
        qDebug() << "Something went wrong!\n\t-> I could not have Site Id and Symbols from Server!";
        return;
    }
    disconnect(clientConnection, &QIODevice::readyRead, this, &SharedDocumentsExplorer::readStartDataFromServer);
    NetworkingData* startData = new NetworkingData(siteId, counter, symbols, clientConnection, networkSession, this);
    showEditor(startData);
}


void SharedDocumentsExplorer::showEditor(NetworkingData* startData) {
    notepad = new Notepad(startData, this);
    notepad->setWindowModality(Qt::ApplicationModal);
    notepad->show();
}