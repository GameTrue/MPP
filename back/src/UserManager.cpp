#include "UserManager.h"
#include "PasswordHasher.h"
#include <random>
#include <sstream>

bool UserManager::registerUser(const User& user) {
    if(users.find(user.getUsername()) != users.end())
        return false;
    users[user.getUsername()] = user;
    return true;
}

std::string UserManager::generateToken(const std::string& username) const {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1000, 9999);
    std::ostringstream oss;
    oss << username << "_" << dist(mt);
    return oss.str();
}

std::string UserManager::login(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if(it != users.end() && it->second.getHashedPassword() == PasswordHasher::hash(password)) {
        std::string token = generateToken(username);
        tokens[token] = username;
        return token;
    }
    return "";
}

bool UserManager::verifyToken(const std::string& token) const {
    return tokens.find(token) != tokens.end();
}

std::string UserManager::getUsernameByToken(const std::string& token) const {
    auto it = tokens.find(token);
    if(it != tokens.end())
        return it->second;
    return "";
}
