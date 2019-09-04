//
// Created by raffa on 25/08/2019.
//

#include "../include/NetworkingData.h"

NetworkingData::NetworkingData(qint32 _siteId,
        quint32 _counter,
        QVector<Symbol> _symbols,
        QTcpSocket* tcpSocket,
        QNetworkSession* networkSession,
        QObject* parent) : QObject(parent),
                        _siteId(_siteId),
                        _counter(_counter),
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

void NetworkingData::incrementCounter() {
    _counter++;
}