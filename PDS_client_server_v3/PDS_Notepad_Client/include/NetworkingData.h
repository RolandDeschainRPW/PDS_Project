//
// Created by raffa on 25/08/2019.
//

#ifndef NOTEPAD_NETWORKINGDATA_H
#define NOTEPAD_NETWORKINGDATA_H

#include <QObject>
#include <QtNetwork>
#include <QVector>

#include "../include/Symbol.h"

class NetworkingData : public QObject {
    Q_OBJECT

public:
    NetworkingData(qint32 _siteId, quint32 _counter, QVector<Symbol> _symbols, QTcpSocket* tcpSocket, QNetworkSession* networkSession, QObject* parent = 0);

    QTcpSocket* getTcpSocket();
    QVector<Symbol>& getSymbols();
    QNetworkSession* getNetworkSession();
    qint32 getSiteId();
    quint32 getCounter();
    void incrementCounter();

private:
    qint32 _siteId;
    quint32 _counter;
    QVector<Symbol> _symbols;
    QTcpSocket* tcpSocket = nullptr;
    QNetworkSession* networkSession = nullptr;
};


#endif //NOTEPAD_NETWORKINGDATA_H
