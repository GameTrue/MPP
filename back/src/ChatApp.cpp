#include "ChatApp.h"
#include "Chat.h"
#include "UserManager.h"
#include <iostream>
#include <algorithm>

ChatApp::ChatApp() {}

void ChatApp::addUser(const User& user) {
    users.push_back(user);
    std::cout << "User " << user.getUsername() << " added to the chat." << std::endl;
}

void ChatApp::removeUser(const std::string& username) {
    auto it = std::remove_if(users.begin(), users.end(), [&username](const User& user) {
        return user.getUsername() == username;
    });
    users.erase(it, users.end());
    std::cout << "User " << username << " removed from the chat." << std::endl;
}

void ChatApp::sendMessage(const std::string& token, const std::string& to, const std::string& content) {
    if(!userManager.verifyToken(token)) {
        std::cout << "Invalid token. Personal message not sent." << std::endl;
        throw std::runtime_error("Invalid token");
    }
    
    // Check if recipient exists
    bool recipientExists = false;
    for(const auto& user : users) {
        if(user.getUsername() == to) {
            recipientExists = true;
            break;
        }
    }
    
    if(!recipientExists) {
        std::cout << "Recipient " << to << " does not exist." << std::endl;
        throw std::runtime_error("Recipient does not exist");
    }
    
    std::string username = userManager.getUsernameByToken(token);
    Message message(username, to, content);
    receiveMessage(message);
    std::cout << "Personal message sent from " << username << " to " << to << std::endl;
}

void ChatApp::receiveMessage(const Message& message) {
    messages.push_back(message);
    std::cout << message.getFrom() << " to " << message.getTo() << ": " << message.getContent() << std::endl;
}

void ChatApp::createChat(const std::string& chatName) {
    chats.push_back(Chat(chatName));
    std::cout << "Chat " << chatName << " created." << std::endl;
}

void ChatApp::joinChat(const std::string& chatName, const User& user) {
    for(auto & chat : chats) {
        if(chat.getChatName() == chatName) {
            chat.addUser(user);
            return;
        }
    }
    std::cout << "Chat " << chatName << " not found." << std::endl;
}

void ChatApp::leaveChat(const std::string& chatName, const std::string& username) {
    for(auto & chat : chats) {
        if(chat.getChatName() == chatName) {
            chat.removeUser(username);
            return;
        }
    }
    std::cout << "Chat " << chatName << " not found." << std::endl;
}

void ChatApp::sendMessageInChat(const std::string& token, const std::string& chatName, const std::string& content) {
    if(!userManager.verifyToken(token)) {
        std::cout << "Invalid token. Message not sent." << std::endl;
        throw std::runtime_error("Invalid token");
    }
    
    std::string username = userManager.getUsernameByToken(token);
    
    bool chatExists = false;
    for(auto & chat : chats) {
        if(chat.getChatName() == chatName) {
            chatExists = true;
            Message msg(username, chatName, content);
            chat.addMessage(msg);
            return;
        }
    }
    
    if(!chatExists) {
        std::cout << "Chat " << chatName << " not found." << std::endl;
        throw std::runtime_error("Chat not found");
    }
}

bool ChatApp::registerUser(const User& user) {
    bool result = userManager.registerUser(user);
    std::cout << (result ? "User registered." : "User already exists.") << std::endl;
    return result;
}

std::string ChatApp::login(const std::string& username, const std::string& password) {
    std::string token = userManager.login(username, password);
    if(!token.empty())
        std::cout << "User " << username << " logged in. Token: " << token << std::endl;
    else
        std::cout << "Login failed for " << username << std::endl;
    return token;
}

std::string ChatApp::getUsernameByToken(const std::string& token) const {
    return userManager.getUsernameByToken(token);
}

bool ChatApp::verifyToken(const std::string& token) const {
    return userManager.verifyToken(token);
}
