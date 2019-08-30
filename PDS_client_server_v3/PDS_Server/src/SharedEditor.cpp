//
// Created by raffa on 21/08/2019.
//

#include "../include/SharedEditor.h"

SharedEditor::SharedEditor(QObject* parent) : QObject(parent) { /* empty */ }

SharedEditor::SharedEditor(QTcpSocket* clientConnection, qint32 _siteId, QObject* parent) : QObject(parent) {
    this->clientConnection = clientConnection;
    this->_siteId = _siteId;

    if (this->clientConnection != nullptr) this->clientConnection->setParent(this); // To avoid Memory Leakage!
}

QTcpSocket* SharedEditor::getClientConnection() {
    return clientConnection;
}

qint32 SharedEditor::getSiteId() {
    return  _siteId;
}
