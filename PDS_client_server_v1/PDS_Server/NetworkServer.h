//
// Created by raffa on 21/08/2019.
//

#ifndef PDS_SERVER_NETWORKSERVER_H
#define PDS_SERVER_NETWORKSERVER_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include <optional>

#include <QDialog>
#include <QString>
#include <QVector>

#include "Symbol.h"
#include "Message.h"
#include "SharedEditor.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
class QNetworkSession;
class QTcpSocket;
class SharedEditor;
QT_END_NAMESPACE

class NetworkServer : public QDialog {
Q_OBJECT

public:
    static const int MAX_SITE_IDS = 100;

    explicit NetworkServer(QWidget *parent = nullptr);

private slots:
    void sessionOpened();
    void connectClient();

private:
    std::queue<Message> messages;
    QVector<SharedEditor*> sharedEditors;
    QVector<Symbol> _symbols;
    QLabel* statusLabel = nullptr;
    QTcpServer* tcpServer = nullptr;
    QNetworkSession* networkSession = nullptr;
    QVector<bool> free_site_ids;

    void readMessage(Message& msg);
    bool comparePositions(std::optional<QVector<qint32>> pos1_opt, std::optional<QVector<qint32>> pos2_opt);
    void dispatchMessages();
    void disconnectClient(qint32 siteId);
    void readRequest(QTcpSocket* clientConnection);
    void processSymbol(const Message& msg);
    QString symbols_to_string();
};

#endif //PDS_SERVER_NETWORKSERVER_H
