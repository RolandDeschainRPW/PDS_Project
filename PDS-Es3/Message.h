//
// Created by giamb on 06/05/2019.
//

#ifndef LAB3_MESSAGE_H
#define LAB3_MESSAGE_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include "Symbol.h"

class Message {
private:
    int _siteId;
    int type;
    Symbol s;

public:
    static const int INSERT_TYPE = 0;
    static const int ERASE_TYPE = 1;

    Message();
    Message(int _siteId, int type, Symbol s);
    Message(const Message& m);
    ~Message();

    Message& operator=(const Message& m);
    int getSiteId();
    int getType() const;
    Symbol getSymbol() const;
};


#endif //LAB3_MESSAGE_H
