//
// Created by raffa on 21/08/2019.
//

#include "../include/SharedEditor.h"

SharedEditor::SharedEditor(QObject* parent) : QObject(parent) { /* empty */ }

SharedEditor::SharedEditor(QTcpSocket* clientConnection,
                           qint32 _siteId,
                           qint32 cursor_pos,
                           QObject* parent) : QObject(parent),
                                              clientConnection(clientConnection),
                                              _siteId(_siteId),
                                              cursor_pos(cursor_pos) { /* empty */ }

QTcpSocket* SharedEditor::getClientConnection() {
    return clientConnection;
}

qint32 SharedEditor::getSiteId() {
    return  _siteId;
}

qint32 SharedEditor::getCursorPos() {
    return cursor_pos;
}

void SharedEditor::setCursorPos(qint32 pos) {
    this->cursor_pos = pos;
}