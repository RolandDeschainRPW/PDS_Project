//
// Created by raffa on 25/08/2019.
//

#ifndef NOTEPAD_NETWORKINGDATA_H
#define NOTEPAD_NETWORKINGDATA_H

#include <QObject>
#include <QtNetwork>

#include "../include/Symbol.h"

class NetworkingData : public QObject {
    Q_OBJECT

public:
    NetworkingData(QObject* parent = 0);

private:
    qint32 _siteId;
    QVector<Symbol> _symbols;
    QTcpSocket* tcpSocket = nullptr;
    QNetworkSession* networkSession = nullptr;
};


#endif //NOTEPAD_NETWORKINGDATA_H
