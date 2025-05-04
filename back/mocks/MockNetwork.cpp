#include "MockNetwork.h"

void MockNetwork::sendMessage(const std::string& message) {
    std::cout << "Sending message: " << message << std::endl;
}

std::string MockNetwork::receiveMessage() {
    return "Mock message received!";
}
