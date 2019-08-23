#ifndef PDS_CLIENT_SHAREDEDITOR_H
#define PDS_CLIENT_SHAREDEDITOR_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QCloseEvent>

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <optional>

#include "Symbol.h"
#include "Message.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class SharedEditor : public QDialog {
Q_OBJECT

public:
    static const qint32 SITE_ID_UNASSIGNED = -1;
    static const qint32 BOUNDARY_PLUS = 0;
    static const qint32 BOUNDARY_MINUS = 1;
    static const qint32 RANDOM = 2;

    explicit SharedEditor(QWidget *parent = nullptr, qint32 base = 32, qint32 boundary = 10, qint32 strategy = RANDOM);
    qint32 getSiteId();
    qint32 getCounter();
    std::optional<Symbol> getSymbol(qint32 index);
    void localInsert(qint32 index, QChar value);
    void localErase(qint32 index);
    void processSymbol(const Message &m);
    QString symbols_to_string();

private slots:
    void connectToServer();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableConnectToServerButton();
    void sessionOpened();

private:
    qint32 _siteId = SITE_ID_UNASSIGNED;
    qint32 _counter = 0;
    qint32 boundary;
    qint32 base;
    qint32 strategy;
    QVector<qint32> strategyCache;
    QVector<Symbol> _symbols;

    Symbol generateSymbol(QChar value, qint32 index);
    QVector<qint32> generatePosBetween(qint32 siteId1,
                                        qint32 siteId2,
                                        QVector<qint32> before,
                                        std::optional<QVector<qint32>> after_opt,
                                        std::optional<QVector<qint32>> newPos_opt,
                                        qint32 level = 0);
    qint32 generateIdBetween(qint32 min, qint32 max, qint32 myStrategy);
    qint32 retrieveStrategy(qint32 level);
    bool comparePositions(std::optional<QVector<qint32>> pos1_opt, std::optional<QVector<qint32>> pos2_opt);
    void readMessage();
    void readDataFromSocket();
    void closeEvent(QCloseEvent* event);

    QComboBox* hostCombo = nullptr;
    QLineEdit* portLineEdit = nullptr;
    QLabel* statusLabel = nullptr;
    QPushButton* connectToServerButton = nullptr;
    QTcpSocket* tcpSocket = nullptr;
    QDataStream in;
    QNetworkSession* networkSession = nullptr;
};

#endif //PDS_CLIENT_SHAREDEDITOR_H
