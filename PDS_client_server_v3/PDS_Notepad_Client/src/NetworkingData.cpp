//
// Created by raffa on 25/08/2019.
//

#include "../include/NetworkingData.h"

NetworkingData::NetworkingData(qint32 _siteId,
        quint32 _counter,
        QString remote_file_path,
        QVector<Symbol> _symbols,
        QTcpSocket* tcpSocket,
        QNetworkSession* networkSession,
        QObject* parent) : QObject(parent),
                        _siteId(_siteId),
                        _counter(_counter),
                        remote_file_path(remote_file_path),
                        _symbols(_symbols),
                        tcpSocket(tcpSocket),
                        networkSession(networkSession) {
    // I am going to change the parents of the following raw pointers
    // to avoid Memory Leakage!
    if (networkSession != nullptr) tcpSocket->setParent(this);
    if (networkSession != nullptr) networkSession->setParent(this);
}

QTcpSocket* NetworkingData::getTcpSocket() {
    return tcpSocket;
}

QVector<Symbol>& NetworkingData::getSymbols() {
    return _symbols;
}

QNetworkSession* NetworkingData::getNetworkSession() {
    return networkSession;
}

quint32 NetworkingData::getCounter() {
    return _counter;
}

qint32 NetworkingData::getSiteId() {
    return _siteId;
}

QString NetworkingData::getRemoteFilePath() {
    return remote_file_path;
}

void NetworkingData::incrementCounter() {
    _counter++;
}