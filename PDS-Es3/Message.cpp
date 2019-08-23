//
// Created by giamb on 06/05/2019.
//

#include "Message.h"

Message::Message() { /* empty */ }

Message::Message(int _siteId, int type, Symbol s) {
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

int Message::getSiteId(){
    return _siteId;
}

int Message::getType() const {
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
