//
// Created by raffa on 21/08/2019.
//

#ifndef PDS_SERVER_SYMBOL_H
#define PDS_SERVER_SYMBOL_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>

#include <QDataStream>
#include <QVector>

class Symbol {
private:
    QChar c;
    qint32 siteId;
    quint32 counter;
    quint32 id;
    QVector<qint32> pos;

public:
    Symbol();
    Symbol(QChar c, qint32 siteId, quint32 counter, QVector<qint32> pos);
    Symbol(const Symbol& s);
    ~Symbol();

    Symbol& operator=(const Symbol&);
    QVector<qint32> getPos() const;
    QChar getChar() const;
    qint32 getSiteId() const;
    qint32 getCounter() const;
    quint32 getId() const;
};

QDataStream &operator<<(QDataStream &out, const Symbol& msg);
QDataStream &operator>>(QDataStream &in, Symbol& msg);

#endif //PDS_SERVER_SYMBOL_H
