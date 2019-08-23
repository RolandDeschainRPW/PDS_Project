//
// Created by giamb on 06/05/2019.
//

#ifndef LAB3_NETWORKSERVER_H
#define LAB3_NETWORKSERVER_H

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include "Message.h"
#include "SharedEditor.h"

class SharedEditor;

class NetworkServer {
private:
    std::queue<Message> messages;
    std::vector<SharedEditor*> sharedEditors;

public:
    NetworkServer();
    NetworkServer(const NetworkServer& ns);

    NetworkServer& operator=(const NetworkServer& ns);
    int connect(SharedEditor* sharedEditor);
    void disconnect(SharedEditor* sharedEditor);
    void send(const Message& m);
    void dispatchMessages();
};

#endif //LAB3_NETWORKSERVER_H
