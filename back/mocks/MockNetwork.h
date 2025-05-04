#ifndef MOCKNETWORK_H
#define MOCKNETWORK_H

#include <string>
#include <iostream>

class MockNetwork {
public:
    void sendMessage(const std::string& message);
    std::string receiveMessage();
};

#endif
