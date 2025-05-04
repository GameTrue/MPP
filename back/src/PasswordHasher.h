#ifndef PASSWORDHASHER_H
#define PASSWORDHASHER_H

#include <string>

class PasswordHasher {
public:
    static std::string hash(const std::string &password);
};

#endif
