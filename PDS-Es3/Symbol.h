//
// Created by giamb on 06/05/2019.
//

#ifndef LAB3_SYMBOL_H
#define LAB3_SYMBOL_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>

class Symbol {
private:
    char c;
    int siteId;
    int counter;
    int id;
    std::vector<int> pos;

public:
    Symbol();
    Symbol(char c, int siteId, int counter, std::vector<int> pos);
    Symbol(const Symbol&);
    ~Symbol();

    Symbol& operator=(const Symbol&);
    std::vector<int> getPos();
    char getChar();
    int getSiteId();
    int getCounter();
    int getId();
};

#endif //LAB3_SYMBOL_H
