//
// Created by raffa on 21/08/2019.
//

#ifndef PDS_SERVER_SHAREDEDITOR_H
#define PDS_SERVER_SHAREDEDITOR_H

#include <QtNetwork>
#include <QtCore>

class SharedEditor : public QObject {
    Q_OBJECT

public:
    SharedEditor();
    SharedEditor(SharedEditor& se);
    SharedEditor(QTcpSocket* clientConnection, qint32 _siteId);

    QTcpSocket* getClientConnection();
    qint32 getSiteId();

private:
    QTcpSocket* clientConnection;
    qint32 _siteId;
};


#endif //PDS_SERVER_SHAREDEDITOR_H
