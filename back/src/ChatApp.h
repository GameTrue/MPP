#ifndef CHATAPP_H
#define CHATAPP_H

#include <vector>
#include <string>
#include "User.h"
#include "Message.h"
#include "Chat.h"
#include "UserManager.h"
#include <algorithm>

class ChatApp {
public:
    ChatApp();
    void addUser(const User& user);
    void removeUser(const std::string& username);
    // Изменённый метод отправки личных сообщений:
    void sendMessage(const std::string& token, const std::string& to, const std::string& content);
    void receiveMessage(const Message& message);

    // Новые методы для работы с чатами
    void createChat(const std::string& chatName);
    void joinChat(const std::string& chatName, const User& user);
    void leaveChat(const std::string& chatName, const std::string& username);
    void sendMessageInChat(const std::string& token, const std::string& chatName, const std::string& content);

    // Новые методы для регистрации и авторизации
    bool registerUser(const User& user);
    std::string login(const std::string& username, const std::string& password);
    
    // Методы для работы с токенами
    std::string getUsernameByToken(const std::string& token) const;
    bool verifyToken(const std::string& token) const;

    // Тестовые геттеры
    const std::vector<User>& getUsers() const { return users; }
    const std::vector<Message>& getMessages() const { return messages; }
    const std::vector<Chat>& getChats() const { return chats; }

private:
    std::vector<User> users;
    std::vector<Message> messages;
    std::vector<Chat> chats;
    UserManager userManager;
};

#endif
