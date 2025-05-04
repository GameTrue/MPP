#include "Chat.h"
#include <algorithm>
#include <iostream>

Chat::Chat(const std::string &chatName) : chatName(chatName) {}

void Chat::addUser(const User& user) {
    participants.push_back(user);
    std::cout << "User " << user.getUsername() << " joined chat " << chatName << std::endl;
}

void Chat::removeUser(const std::string& username) {
    auto it = std::remove_if(participants.begin(), participants.end(), [&username](const User &user) {
        return user.getUsername() == username;
    });
    participants.erase(it, participants.end());
    std::cout << "User " << username << " left chat " << chatName << std::endl;
}

const std::vector<User>& Chat::getUsers() const {
    return participants;
}

void Chat::addMessage(const Message& message) {
    messages.push_back(message);
    std::cout << message.getFrom() << " to " << message.getTo() << " in chat " << chatName << ": " << message.getContent() << std::endl;
}

const std::vector<Message>& Chat::getMessages() const {
    return messages;
}

const std::string& Chat::getChatName() const {
    return chatName;
}
