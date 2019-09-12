//
// Created by raffa on 30/08/2019.
//

#include <QMessageBox>
#include <QInputDialog>

#include "../include/Request.h"
#include "../include/Response.h"
#include "../include/SharedDocumentsExplorer.h"
#include "../include/DocumentsModel.h"
#include "../include/NetworkingData.h"
#include "../include/StartDialog.h"
#include "../include/SerialSize.h"
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
    ui->documentsView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->documentsView->show();

    connect(ui->cancelButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(ui->newDocumentButton, &QAbstractButton::clicked, this, &SharedDocumentsExplorer::askNewDocumentFilename);
    connect(ui->openDocumentButton, &QAbstractButton::clicked, this, &SharedDocumentsExplorer::openDocument);
    connect(ui->addCollaboratorButton, &QAbstractButton::clicked, this, &SharedDocumentsExplorer::addCollaborator);
    connect(ui->modifyProfileButton, &QAbstractButton::clicked, this, &SharedDocumentsExplorer::openModifyProfileDialog);
}

SharedDocumentsExplorer::~SharedDocumentsExplorer() {
    delete ui;
}

void SharedDocumentsExplorer::openModifyProfileDialog() {
    ui->modifyProfileButton->setEnabled(false);

    // Temporarily disconnecting these two signals
    disconnect(clientConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &SharedDocumentsExplorer::displayError);
    disconnect(clientConnection, &QIODevice::readyRead, this, &SharedDocumentsExplorer::readStartDataFromServer);

    mod_profile_dialog = new ModifyProfileDialog(clientConnection, networkSession, username, this);
    connect(mod_profile_dialog, &QDialog::finished, this, [this] {
        // Reconnecting signals.
        connect(clientConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &SharedDocumentsExplorer::displayError);
        connect(clientConnection, &QIODevice::readyRead, this, &SharedDocumentsExplorer::readStartDataFromServer);
    });

    mod_profile_dialog->open();
    ui->modifyProfileButton->setEnabled(true);
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
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_13);
            if (isThisFilenameAlreadyInUse(new_filename)) continue;
            Request req_to_send = Request(Request::SITE_ID_UNASSIGNED, Request::NEW_DOCUMENT_TYPE, std::nullopt, username, "", new_filename);

            // Calculating the size of the request in bytes.
            SerialSize size;
            qint64 req_size = size(req_to_send);

            out << req_size << req_to_send;
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
    QItemSelectionModel* select = ui->documentsView->selectionModel();

    // Checking if has selection.
    if (!select->hasSelection()) {
        QMessageBox::information(this, tr("No document selected!"),
                                 tr("Please select a document to open it."));
        return;
    }

    int row = select->selectedRows()[0].row();
    DocumentsModel* model = dynamic_cast<DocumentsModel*>(ui->documentsView->model());

    QModelIndex index = model->index(row, 0, QModelIndex());
    QString filename = ui->documentsView->model()->data(index).toString();
    filename = filename.replace(QString(".html"), QString(""), Qt::CaseInsensitive);

    index = model->index(row, 1, QModelIndex());
    QString owner_nickname = ui->documentsView->model()->data(index).toString();

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(Request::SITE_ID_UNASSIGNED, Request::OPEN_DOCUMENT_TYPE, std::nullopt, username, "", filename, 0, owner_nickname);

    // Calculating the size of the request in bytes.
    SerialSize size;
    qint64 req_size = size(req_to_send);

    out << req_size << req_to_send;
    clientConnection->write(block);
    selected = true;
}

void SharedDocumentsExplorer::closeEvent(QCloseEvent* event) {
    StartDialog* parent = dynamic_cast<StartDialog*>(this->parent());
    connect(clientConnection, &QIODevice::readyRead, parent, &StartDialog::openSharedDocumentsExplorer);
    if (!selected) {
        qDebug().noquote() << "No document selected.\n\t-> Disconnecting from Server.";
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
        qDebug() << "Something went wrong!\n\t-> I could not read start data from Server!";
        return;
    }

    disconnect(clientConnection, &QIODevice::readyRead, this, &SharedDocumentsExplorer::readStartDataFromServer);
    NetworkingData* startData = new NetworkingData(res.getSiteId(),
                                                   res.getCounter(),
                                                   res.getFilePath(),
                                                   res.getSymbols(),
                                                   clientConnection,
                                                   networkSession,
                                                   this);
    showEditor(startData, res.getProfilePic(), res.getNickname(), res.getConnectedEditors());
}


void SharedDocumentsExplorer::showEditor(NetworkingData* startData, QImage profile_pic, QString nickname, QVector<Collaborator> connected_editors) {
    notepad = new Notepad(startData, profile_pic, nickname, connected_editors, this);
    notepad->setWindowModality(Qt::ApplicationModal);
    notepad->show();
}

void SharedDocumentsExplorer::addCollaborator() {
    QItemSelectionModel* select = ui->documentsView->selectionModel();

    // Checking if has selection.
    if (!select->hasSelection()) {
        QMessageBox::information(this, tr("No document selected!"),
                                 tr("Please select a document before adding a collaborator."));
        return;
    }

    int row = select->selectedRows()[0].row();
    DocumentsModel* model = dynamic_cast<DocumentsModel*>(ui->documentsView->model());
    QModelIndex index = model->index(row, 0, QModelIndex());
    QString filename = ui->documentsView->model()->data(index).toString();
    filename = filename.replace(QString(".html"), QString(""), Qt::CaseInsensitive);

    disconnect(clientConnection, &QIODevice::readyRead, this, &SharedDocumentsExplorer::readStartDataFromServer);
    connect(clientConnection, &QIODevice::readyRead, this, [this] {
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
            qDebug() << "Something went wrong!\n\t-> I could not add the collaborator as requested!";
            return;
        }

        if (res.getResult() == Response::NICKNAME_NON_EXISTENT) {
            QMessageBox::information(this, tr("Nickname non existent!"),
                                     tr("Please insert the nickname of an existent user."));
        } else if (res.getResult() == Response::TOO_MANY_COLLABORATORS) {
            QMessageBox::information(this, tr("Too many collaborators!"),
                                     tr("Collaborators limit was reached (max. 4 collaborators per document)."));
        } else /* Response::NICKNAME_ACTIVE */ {
            QMessageBox::information(this, tr("Success!"),
                                     tr("Collaborator added successfully."));
        }

        disconnect(clientConnection, &QIODevice::readyRead, this, 0);
        connect(clientConnection, &QIODevice::readyRead, this, &SharedDocumentsExplorer::readStartDataFromServer);
    });

    bool ok;
    QString nickname;
    while (true) {
        nickname = QInputDialog::getText(this,
                                         tr("Add a new collaborator"),
                                         tr("Insert new collaborator's nickname:"),
                                         QLineEdit::Normal,
                                         QDir::home().dirName(),
                                         &ok);
        if (ok && !nickname.isEmpty()) {
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_13);

            Request req_to_send = Request(Request::SITE_ID_UNASSIGNED, Request::ADD_COLLABORATOR_TYPE, std::nullopt, username, "", filename, 0, nickname);

            // Calculating the size of the request in bytes.
            SerialSize size;
            qint64 req_size = size(req_to_send);

            out << req_size << req_to_send;
            clientConnection->write(block);
            return;
        } else if (ok && nickname.isEmpty()) {
            QMessageBox::information(this, tr("Missing nickname!"),
                                     tr("Please insert the nickname of the new collaborator."));
        } else {
            disconnect(clientConnection, &QIODevice::readyRead, this, 0);
            connect(clientConnection, &QIODevice::readyRead, this, &SharedDocumentsExplorer::readStartDataFromServer);
            return;
        }
    }
}
