//
// Created by raffa on 21/08/2019.
//

#include "../include/Message.h"

Message::Message() { /* empty */ }

Message::Message(qint32 _siteId, qint32 type, Symbol s) {
    this->type = type;
    this->_siteId = _siteId;
    this->s = s;
}

Message::Message(const Message& m) {
    this->_siteId = m._siteId;
    this->type = m.type;
    this->s = m.s;
}

Message::~Message() { /* empty */ }

qint32 Message::getSiteId() const {
    return _siteId;
}

qint32 Message::getType() const {
    return type;
}

Symbol Message::getSymbol() const {
    return s;
}

Message& Message::operator=(const Message &m) {
    if (this != &m){
        this->_siteId = m._siteId;
        this->type = m.type;
        this->s = m.s;
    }
    return *this;
}

QDataStream &operator<<(QDataStream &out, const Message& msg) {
    out << msg.getType() << msg.getSiteId() << msg.getSymbol();
    return out;
}

QDataStream &operator>>(QDataStream &in, Message& msg) {
    int type;
    int _siteId;
    Symbol s;

    in >> type >> _siteId >> s;
    msg = Message(_siteId, type, s);
    return in;
}