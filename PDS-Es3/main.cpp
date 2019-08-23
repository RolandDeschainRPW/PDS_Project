#include <iostream>
#include "NetworkServer.h"
#include "Message.h"
#include "SharedEditor.h"
#include "Symbol.h"
#include "Message.h"

int main() {
    NetworkServer network;
    SharedEditor ed1(network);
    SharedEditor ed2(network);

    ed1.localInsert(0, 'c');
    ed1.localInsert(1, 'a');
    ed1.localInsert(2, 't');
    //network.dispatchMessages();

    std::cout << " 1 -> ed1: " << ed1.to_string() << std::endl;
    std::cout << " 1 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed1.localInsert(1, 'h');
    //network.dispatchMessages();

    std::cout<< " 2 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< " 2 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    // Ulteriori test.
    ed1.localInsert(1, 'z');
    //network.dispatchMessages();
    std::cout<< " 3 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< " 3 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed1.localErase(2);
    //network.dispatchMessages();
    std::cout<< " 4 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< " 4 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed2.localErase(0);
    //network.dispatchMessages();
    std::cout<< " 5 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< " 5 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed2.localInsert(1, 'g');
    //network.dispatchMessages();
    std::cout<< " 6 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< " 6 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed2.localInsert(1, 'f');
    //network.dispatchMessages();
    std::cout<< " 7 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< " 7 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed2.localInsert(2, 'k');
    //network.dispatchMessages();
    std::cout<< " 8 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< " 8 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed1.localErase(1);
    //network.dispatchMessages();
    std::cout<< " 9 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< " 9 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed2.localErase(0);
    //network.dispatchMessages();
    std::cout<< "10 -> ed1: " << ed1.to_string() << std::endl;
    std::cout<< "10 -> ed2: " << ed2.to_string() << std::endl << std::endl;

    ed1.localInsert(2, 'r');
    //network.dispatchMessages();
    // Fine ulteriori test.

    std::cout<<"11 -> ed1: "<<ed1.to_string()<<std::endl;
    std::cout<<"11 -> ed2: "<<ed2.to_string()<<std::endl << std::endl;

    return 0;
}