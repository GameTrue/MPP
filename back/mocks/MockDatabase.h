#ifndef MOCKDATABASE_H
#define MOCKDATABASE_H

#include <string>
#include <iostream>

class MockDatabase {
public:
    void saveMessage(const std::string& message);
    std::string loadMessage();
};

#endif
