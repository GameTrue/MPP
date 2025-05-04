#include "MockDatabase.h"

void MockDatabase::saveMessage(const std::string& message) {
    std::cout << "Saving message: " << message << std::endl;
}

std::string MockDatabase::loadMessage() {
    return "Mock message loaded!";
}
