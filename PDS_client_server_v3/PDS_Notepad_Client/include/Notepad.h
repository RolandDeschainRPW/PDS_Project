#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <optional>

#include <QMainWindow>
#include <QTextEdit>
#include <QTextCursor>
#include <QVector>
#include <QtNetwork>

#include "../include/Symbol.h"
#include "../include/Message.h"
#include "../include/NetworkingData.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Notepad;
}
QT_END_NAMESPACE

class Notepad : public QMainWindow {
    Q_OBJECT

public:
    static const qint32 BOUNDARY_PLUS = 0;
    static const qint32 BOUNDARY_MINUS = 1;
    static const qint32 RANDOM = 2;

    explicit Notepad(NetworkingData* net_data, QWidget *parent = 0, qint32 base = 32, qint32 boundary = 10, qint32 strategy = RANDOM);
    ~Notepad();

    qint32 getSiteId();
    std::optional<Symbol> getSymbol(qint32 index);
    void localInsert(qint32 index, QChar value);
    void localErase(qint32 index);
    void processSymbol(const Message &m);
    QString symbols_to_string();

private slots:
    void print();
    void exit();
    void copy();
    void cut();
    void paste();
    void undo();
    void redo();

    // Slot for text changes.
    void interceptUserInput(int pos, int del, int add);

    // Slot for networking error.
    void displayError(QAbstractSocket::SocketError socketError);

private:
    Ui::Notepad* ui;

    // Vars for symbols managing.
    qint32 boundary;
    qint32 base;
    qint32 strategy;
    QVector<qint32> strategyCache;

    // Vars for networking.
    NetworkingData* net_data = nullptr;
    QDataStream in;

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
    void closeEvent(QCloseEvent* event) override;
    void updateDocument(qint32 index, qint32 updateType, QString text);
};

#endif // NOTEPAD_H
