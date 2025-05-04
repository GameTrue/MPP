#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User() : username(""), hashedPassword("") {}
    User(const std::string& username, const std::string& password);
    std::string getUsername() const;
    const std::string& getHashedPassword() const;

private:
    std::string username;
    std::string hashedPassword;
};

#endif
