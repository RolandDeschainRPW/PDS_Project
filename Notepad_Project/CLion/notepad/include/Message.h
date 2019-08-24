//
// Created by raffa on 24/08/2019.
//

#ifndef NOTEPAD_MESSAGE_H
#define NOTEPAD_MESSAGE_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include <QDataStream>

#include "Symbol.h"

class Message {
private:
    qint32 _siteId;
    qint32 type;
    Symbol s;

public:
    static const qint32 INSERT_TYPE = -1;
    static const qint32 ERASE_TYPE = -2;

    Message();
    Message(qint32 _siteId, qint32 type, Symbol s);
    Message(const Message& m);
    ~Message();

    Message& operator=(const Message& m);
    qint32 getSiteId() const;
    qint32 getType() const;
    Symbol getSymbol() const;
};

QDataStream &operator<<(QDataStream &out, const Message& msg);
QDataStream &operator>>(QDataStream &in, Message& msg);

#endif //NOTEPAD_MESSAGE_H
