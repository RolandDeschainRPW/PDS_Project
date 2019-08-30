//
// Created by raffa on 30/08/2019.
//

#include <QMessageBox>

#include "../include/SharedDocumentsExplorer.h"
#include "../include/DocumentsModel.h"
#include "ui_shareddocumentsexplorer.h"

SharedDocumentsExplorer::SharedDocumentsExplorer(QTcpSocket* clientConnection, QWidget* parent) : QDialog(parent),
                                                                                            ui(new Ui::SharedDocumentsExplorer),
                                                                                            clientConnection(clientConnection) {
    ui->setupUi(this);
    connect(clientConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &SharedDocumentsExplorer::displayError);
    connect(this, &QDialog::finished, this, &QObject::deleteLater);

    QList<QString> documents_filenames;
    QList<QString> documents_owners;

    // Create some data that is tabular in nature:
    documents_filenames.append("apple orders.rtf");
    documents_filenames.append("sport article.rtf");
    documents_filenames.append("birthday presents.rtf");

    documents_filenames.append("apple orders.rtf");
    documents_filenames.append("sport article.rtf");
    documents_filenames.append("birthday presents.rtf");

    documents_filenames.append("apple orders.rtf");
    documents_filenames.append("sport article.rtf");
    documents_filenames.append("birthday presents.rtf");

    documents_owners.append("Richard");
    documents_owners.append("Thomas");
    documents_owners.append("Harry");

    documents_owners.append("Richard");
    documents_owners.append("Thomas");
    documents_owners.append("Harry");

    documents_owners.append("Richard");
    documents_owners.append("Thomas");
    documents_owners.append("Harry");

    // Create model:
    DocumentsModel* docs_model = new DocumentsModel(this);

    // Populate model with data:
    docs_model->populateData(documents_filenames,documents_owners);

    // Connect model to table view:
    ui->documentsView->setModel(docs_model);

    // Make table header visible and display table:
    ui->documentsView->horizontalHeader()->setVisible(true);
    ui->documentsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->documentsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->documentsView->show();

    connect(ui->cancelButton, &QAbstractButton::clicked, this, &QWidget::close);
}

SharedDocumentsExplorer::~SharedDocumentsExplorer() {
    delete ui;
}

void SharedDocumentsExplorer::closeEvent(QCloseEvent* event) {
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
