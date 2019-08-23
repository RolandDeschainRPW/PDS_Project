//
// Created by giamb on 06/05/2019.
//

#include "Symbol.h"

Symbol::Symbol() { /* empty */ }

Symbol::Symbol(char c, int siteId, int counter, std::vector<int> pos) {
    this->c = c;
    this->siteId = siteId;
    this->counter = counter;
    this->id = siteId + counter;
    this->pos = pos;
}

Symbol::Symbol(const Symbol &s) {
    this->c = s.c;
    this->siteId = s.siteId;
    this->counter = s.counter;
    this->id = s.id;
    this->pos = s.pos;
}

Symbol::~Symbol() { /* empty */ }

Symbol& Symbol::operator=(const Symbol& s) {
    if(this != &s){
        this->c = s.c;
        this->siteId = s.siteId;
        this->counter = s.counter;
        this->id = s.id;
        this->pos = s.pos;
    }
    return *this;
}

std::vector<int> Symbol::getPos() {
    return pos;
}

int Symbol::getSiteId() {
    return siteId;
}

int Symbol::getCounter() {
    return counter;
}

int Symbol::getId() {
    return id;
}

char Symbol::getChar() {
    return c;
}
