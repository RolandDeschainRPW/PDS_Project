//
// Created by giamb on 06/05/2019.
//

#include "SharedEditor.h"

SharedEditor::SharedEditor(NetworkServer& ns,
        int base,
        int boundary,
        int strategy) : _server(ns),  base(base), boundary(boundary), strategy(strategy) {
    this->_siteId = _server.connect(this);
}

SharedEditor::~SharedEditor() {
    _server.disconnect(this);
}

int SharedEditor::getSiteId() {
    return _siteId;
}

int SharedEditor::getCounter() {
    return _counter;
}

void SharedEditor::localInsert(int index, char value) {
    Symbol s = this->generateSymbol(value, index);

    if (_symbols.empty())  _symbols.push_back(s);
    else if (index > _symbols.size() - 1) _symbols.push_back(s);
    else if (index == 0) _symbols.insert(_symbols.begin(), s);
    else _symbols.insert(_symbols.begin() + index, s);

    _counter++;
    _server.send(Message(_siteId, Message::INSERT_TYPE, s));
    // Allineo tutto.
    _server.dispatchMessages();
}

void SharedEditor::localErase(int index) {
    if (index >= _symbols.size()) return;

    Symbol tmp = _symbols.at(index);
    _symbols.erase(_symbols.begin() + index);

    Message m = Message(_siteId, Message::ERASE_TYPE, tmp);
    _counter++;
    _server.send(m);
    // Allineo tutto.
    _server.dispatchMessages();
}

void SharedEditor::process(const Message& m) {
    int index = 0;
    if (m.getType() == Message::INSERT_TYPE) {
        if (_symbols.empty()) _symbols.push_back(m.getSymbol());
        else {
            for (Symbol s : _symbols) {
                if (this->comparePositions(s.getPos(), m.getSymbol().getPos())) {
                    _symbols.insert(_symbols.begin() + index, m.getSymbol());
                    return;
                }
                index++;
            }
            _symbols.push_back(m.getSymbol());
        }
    } else /* Message::ERASE_TYPE */ {
        for (Symbol s : _symbols){
            if (s.getChar() == m.getSymbol().getChar() && s.getId() == m.getSymbol().getId()) {
                _symbols.erase(_symbols.begin() + index);
                return;
            }
            index++;
        }
    }
}

std::string SharedEditor::to_string() {
    std::string str;
    for (Symbol s : _symbols) {
        str += s.getChar();
    }
    return str;
}

std::optional<Symbol> SharedEditor::getSymbol(int index) {
    if (!_symbols.empty() && index >= 0 && index <= _symbols.size() - 1) return _symbols.at(index);
    else return std::nullopt;
}

// Returns true if pos1 > pos2
bool SharedEditor::comparePositions(std::optional<std::vector<int>> pos1_opt,
        std::optional<std::vector<int>> pos2_opt) {
    std::vector<int> pos1 = (pos1_opt.has_value()) ? pos1_opt.value() : std::vector<int>();
    std::vector<int> pos2 = (pos2_opt.has_value()) ? pos2_opt.value() : std::vector<int>();

    int digit1 = (!pos1.empty()) ? pos1[0] : 0;
    int digit2 = (!pos2.empty()) ? pos2[0] : 0;

    if (digit1 > digit2) return true;
    else if (digit1 < digit2) return false;
    else if (digit1 == digit2 && pos1.size() == 1 && pos2.size() == 1) return false;
    else {
        std::vector<int> slice1 = (pos1.begin() + 1 < pos1.end()) ? std::vector<int>(pos1.begin() + 1, pos1.end()) : std::vector<int>();
        std::vector<int> slice2 = (pos2.begin() + 1 < pos2.end()) ? std::vector<int>(pos2.begin() + 1, pos2.end()) : std::vector<int>();
        return this->comparePositions(slice1, slice2);
    }
}

Symbol SharedEditor::generateSymbol(char value, int index) {
    std::vector<int> before = (this->getSymbol(index - 1).has_value()) ? this->getSymbol(index - 1).value().getPos() : std::vector<int>();
    int siteIdBefore = (this->getSymbol(index - 1).has_value()) ? this->getSymbol(index - 1).value().getSiteId() : this->getSiteId();

    std::vector<int> after = (this->getSymbol(index).has_value()) ? this->getSymbol(index).value().getPos() : std::vector<int>();
    int siteIdAfter = (this->getSymbol(index).has_value()) ? this->getSymbol(index).value().getSiteId() : this->getSiteId();

    std::vector<int> newPos = this->generatePosBetween(siteIdBefore, siteIdAfter, before, after, std::nullopt);
    return Symbol(value, this->getSiteId(), this->getCounter(), newPos);
}

std::vector<int> SharedEditor::generatePosBetween(int siteId1,
                                          int siteId2,
                                          std::vector<int> before,
                                          std::optional<std::vector<int>> after_opt,
                                          std::optional<std::vector<int>> newPos_opt,
                                          int level) {
    int myBase = static_cast<int>(pow(2, level) * this->base);
    int myStrategy = this->retrieveStrategy(level);

    std::vector<int> after = (after_opt.has_value()) ? after_opt.value() : std::vector<int>();
    std::vector<int> newPos = (newPos_opt.has_value()) ? newPos_opt.value() : std::vector<int>();

    int id1 = (!before.empty()) ? before[0] : 0;
    int id2 = (!after.empty()) ? after[0] : myBase;

    if (id2 - id1 > 1) {
        int newDigit = this->generateIdBetween(id1, id2, myStrategy);
        newPos.push_back(newDigit);
        return newPos;
    } else if (id2 - id1 == 1) {
        newPos.push_back(id1);
        std::vector<int> slice = (before.begin() + 1 < before.end()) ? std::vector<int>(before.begin() + 1, before.end()) : std::vector<int>();
        return this->generatePosBetween(siteId1, siteId2, slice, std::nullopt, newPos, level + 1);
    } else if (id1 == id2) {
        if (siteId1 < siteId2) {
            newPos.push_back(id1);
            std::vector<int> slice = (before.begin() + 1 < before.end()) ? std::vector<int>(before.begin() + 1, before.end()) : std::vector<int>();
            return this->generatePosBetween(siteId1, siteId2, slice, std::nullopt, newPos, level + 1);
        } else if (siteId1 == siteId2) {
            newPos.push_back(id1);
            std::vector<int> slice1 = (before.begin() + 1 < before.end()) ? std::vector<int>(before.begin() + 1, before.end()) : std::vector<int>();
            std::vector<int> slice2 = (after.begin() + 1 < after.end()) ? std::vector<int>(after.begin() + 1, after.end()) : std::vector<int>();
            return this->generatePosBetween(siteId1, siteId2, slice1, slice2, newPos, level + 1);
        } else {
            // Da rivedere!
            // "Fix Position Sorting"
            throw std::exception();
        }
    }
}

int SharedEditor::generateIdBetween(int min, int max, int myStrategy) {
    if ((max - min) < boundary) {
        min++;
    } else {
        if (myStrategy == BOUNDARY_MINUS) {
            min = max - boundary;
        } else {
            min++;
            max = min + boundary;
        }
    }

    // Random Number Generator.
    std::default_random_engine rd;
    std::mt19937 gen(rd());

    // Uniform distribution between 0 and 1.
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return static_cast<int>(floor(dis(gen) * (max - min)) + min);
}

int SharedEditor::retrieveStrategy(int level) {
    if (!strategyCache.empty() && level <= strategyCache.size() - 1) return strategyCache[level];

    int myStrategy;

    switch (this->strategy) {
        case BOUNDARY_PLUS: {
            myStrategy = BOUNDARY_PLUS;
            break;
        }
        case BOUNDARY_MINUS: {
            myStrategy = BOUNDARY_MINUS;
            break;
        }
        case RANDOM: {
            // Random Number Generator.
            std::default_random_engine rd;
            std::mt19937 gen(rd());

            // Uniform distribution between 0 and 1.
            std::uniform_real_distribution<double> dis(0.0, 1.0);

            myStrategy = round(dis(gen)) == 0 ? BOUNDARY_PLUS : BOUNDARY_MINUS;
            break;
        }
        default: {
            myStrategy = (level % 2) == 0 ? BOUNDARY_PLUS : BOUNDARY_MINUS;
            break;
        }
    }

    strategyCache.push_back(myStrategy);
    return myStrategy;
}
