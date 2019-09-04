//
// Created by raffa on 21/08/2019.
//

#include "../include/Symbol.h"

Symbol::Symbol() { /* empty */ }

Symbol::Symbol(QChar c, qint32 siteId, quint32 counter, QVector<qint32> pos) {
    this->c = c;
    this->siteId = siteId;
    this->counter = counter;

    if (siteId == 0) this->id = counter;
    else this->id = static_cast<quint32>(siteId) + counter;

    this->pos = pos;
}

Symbol::Symbol(const Symbol& s) {
    this->c = s.c;
    this->siteId = s.siteId;
    this->counter = s.counter;
    this->id = s.id;
    this->pos = s.pos;
}

Symbol::~Symbol() { /* empty */ }

Symbol& Symbol::operator=(const Symbol& s) {
    if(this != &s) {
        this->c = s.c;
        this->siteId = s.siteId;
        this->counter = s.counter;
        this->id = s.id;
        this->pos = s.pos;
    }
    return *this;
}

QVector<qint32> Symbol::getPos() const {
    return pos;
}

qint32 Symbol::getSiteId() const {
    return siteId;
}

qint32 Symbol::getCounter() const {
    return counter;
}

quint32 Symbol::getId() const {
    return id;
}

QChar Symbol::getChar() const {
    return c;
}

QDataStream &operator<<(QDataStream &out, const Symbol& s) {
    out << s.getChar() << s.getSiteId() << s.getCounter() << s.getPos();
    return out;
}

QDataStream &operator>>(QDataStream &in, Symbol& s) {
    QChar c;
    qint32 siteId;
    quint32 counter;
    QVector<qint32> pos;

    in >> c >> siteId >> counter >> pos;
    s = Symbol(c, siteId, counter, pos);
    return in;
}
