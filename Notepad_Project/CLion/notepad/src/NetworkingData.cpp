//
// Created by raffa on 25/08/2019.
//

#include "../include/NetworkingData.h"

NetworkingData::NetworkingData(qint32 _siteId,
        QVector<Symbol> _symbols,
        QTcpSocket* tcpSocket,
        QNetworkSession* networkSession,
        QObject* parent) : QObject(parent),
                        _siteId(_siteId),
                        _symbols(_symbols),
                        tcpSocket(tcpSocket),
                        networkSession(networkSession) {
    // I am going to change the parents of the following raw pointers
    // to avoid Memory Leakage!
    tcpSocket->setParent(this);
    networkSession->setParent(this);
}

QTcpSocket* NetworkingData::getTcpSocket() {
    return tcpSocket;
}