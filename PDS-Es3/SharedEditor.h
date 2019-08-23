//
// Created by giamb on 06/05/2019.
//

#ifndef LAB3_SHAREDEDITOR_H
#define LAB3_SHAREDEDITOR_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <optional>
#include "NetworkServer.h"
#include "Symbol.h"
#include "Message.h"

class NetworkServer;

class SharedEditor {
private:
    int _siteId;
    int _counter = 0;
    int boundary;
    int base;
    int strategy;
    std::vector<int> strategyCache;
    NetworkServer& _server;
    std::vector<Symbol> _symbols;

    Symbol generateSymbol(char value, int index);
    std::vector<int> generatePosBetween(int siteId1,
                                        int siteId2,
                                        std::vector<int> before,
                                        std::optional<std::vector<int>> after_opt,
                                        std::optional<std::vector<int>> newPos_opt,
                                        int level = 0);
    int generateIdBetween(int min, int max, int myStrategy);
    int retrieveStrategy(int level);
    bool comparePositions(std::optional<std::vector<int>> pos1_opt,
            std::optional<std::vector<int>> pos2_opt);

public:
    static const int BOUNDARY_PLUS = 0;
    static const int BOUNDARY_MINUS = 1;
    static const int RANDOM = 2;

    SharedEditor(NetworkServer& ns, int base = 32, int boundary = 10, int strategy = RANDOM);
    ~SharedEditor();

    int getSiteId();
    int getCounter();
    std::optional<Symbol> getSymbol(int index);
    void localInsert(int index, char value);
    void localErase(int index);
    void process(const Message& m);
    std::string to_string();
};

#endif //LAB3_SHAREDEDITOR_H
