//
// Created by raffa on 21/08/2019.
//

#include "SharedEditor.h"

SharedEditor::SharedEditor(QObject* parent) : QObject(parent) { /* empty */ }

SharedEditor::SharedEditor(SharedEditor& se, QObject* parent) : QObject(parent) {
    this->clientConnection = se.clientConnection;
    this->_siteId = se._siteId;
}

SharedEditor::SharedEditor(QTcpSocket* clientConnection, qint32 _siteId, QObject* parent) : QObject(parent) {
    this->clientConnection = clientConnection;
    this->_siteId = _siteId;
}

QTcpSocket* SharedEditor::getClientConnection() {
    return clientConnection;
}

qint32 SharedEditor::getSiteId() {
    return  _siteId;
}
