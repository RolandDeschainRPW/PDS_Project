//
// Created by giamb on 06/05/2019.
//

#include "NetworkServer.h"

NetworkServer::NetworkServer() { /* empty */ }

NetworkServer::NetworkServer(const NetworkServer &ns) {
        this->messages = ns.messages;
        this->sharedEditors = ns.sharedEditors;
}

NetworkServer& NetworkServer::operator=(const NetworkServer& ns) {
    if (this != &ns){
        this->messages = ns.messages;
        this->sharedEditors = ns.sharedEditors;
    }
    return *this;
}

int NetworkServer::connect(SharedEditor* sharedEditor){
    uint16_t cnt = 0;

    for (SharedEditor* se : sharedEditors) {
        if (se == nullptr) {
            sharedEditors.insert(sharedEditors.begin() + cnt, sharedEditor);
            return cnt;
        }
        cnt++;
    }

    sharedEditors.push_back(sharedEditor);
    return cnt;
}

void NetworkServer::disconnect(SharedEditor* sharedEditor) {
    uint16_t cnt = 0;

    for(SharedEditor* se : sharedEditors) {
        if (se == sharedEditor)
            sharedEditors.insert(sharedEditors.begin() + cnt, nullptr);
        cnt++;
    }
}

void NetworkServer::send(const Message& m) {
    messages.push(m);
}

void NetworkServer::dispatchMessages() {
    Message tmp;
    while (!messages.empty()) {
        tmp = messages.front();
        messages.pop();
        for (SharedEditor* se : sharedEditors) {
            if (tmp.getSiteId() != se->getSiteId())
                se->process(tmp);
        }
    }
}
