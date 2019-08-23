//
// Created by raffa on 21/08/2019.
//

#ifndef PDS_CLIENT_SYMBOL_H
#define PDS_CLIENT_SYMBOL_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>

#include <QDataStream>

class Symbol {
private:
    QChar c;
    qint32 siteId;
    qint32 counter;
    qint32 id;
    QVector<qint32> pos;

public:
    Symbol();
    Symbol(QChar c, qint32 siteId, qint32 counter, QVector<qint32> pos);
    Symbol(const Symbol& s);
    ~Symbol();

    Symbol& operator=(const Symbol&);
    QVector<qint32> getPos() const;
    QChar getChar() const;
    qint32 getSiteId() const;
    qint32 getCounter() const;
    qint32 getId() const;
};

QDataStream &operator<<(QDataStream &out, const Symbol& msg);
QDataStream &operator>>(QDataStream &in, Symbol& msg);

#endif //PDS_CLIENT_SYMBOL_H
