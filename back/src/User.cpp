#include "User.h"
#include "PasswordHasher.h"

User::User(const std::string &username, const std::string &password)
    : username(username),
      hashedPassword(PasswordHasher::hash(password)) {
}

std::string User::getUsername() const {
    return username;
}

const std::string& User::getHashedPassword() const {
    return hashedPassword;
}
