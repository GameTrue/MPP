#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <unordered_map>
#include "User.h"

class UserManager {
public:
    bool registerUser(const User& user);
    std::string login(const std::string& username, const std::string& password);
    bool verifyToken(const std::string& token) const;
    std::string getUsernameByToken(const std::string& token) const;
private:
    std::unordered_map<std::string, User> users; // username -> User
    std::unordered_map<std::string, std::string> tokens; // token -> username
    std::string generateToken(const std::string& username) const;
};

#endif
