//
// Created by raffa on 21/08/2019.
//

#include "SharedEditor.h"

SharedEditor::SharedEditor() { /* empty */ }

SharedEditor::SharedEditor(SharedEditor& se) {
    this->clientConnection = se.clientConnection;
    this->_siteId = se._siteId;
}

SharedEditor::SharedEditor(QTcpSocket* clientConnection, qint32 _siteId) {
    this->clientConnection = clientConnection;
    this->_siteId = _siteId;
}

QTcpSocket* SharedEditor::getClientConnection() {
    return clientConnection;
}

qint32 SharedEditor::getSiteId() {
    return  _siteId;
}
