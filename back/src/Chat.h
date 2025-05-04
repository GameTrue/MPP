#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <vector>
#include "User.h"
#include "Message.h"

class Chat {
public:
    Chat(const std::string &chatName);
    void addUser(const User& user);
    void removeUser(const std::string& username);
    const std::vector<User>& getUsers() const;
    void addMessage(const Message& message);
    const std::vector<Message>& getMessages() const;
    const std::string& getChatName() const;
private:
    std::string chatName;
    std::vector<User> participants;
    std::vector<Message> messages;
};

#endif
