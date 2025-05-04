#include "ChatApp.h"
#include "User.h"
#include "Message.h"
#include "../mocks/MockNetwork.h"
#include "../mocks/MockDatabase.h"

int main() {
    // Создание приложения и пользователей
    ChatApp chatApp;
    User user1("Alice");
    User user2("Bob");

    chatApp.addUser(user1);
    chatApp.addUser(user2);

    // Отправка и получение сообщений
    chatApp.sendMessage("Alice", "Bob", "Hello, Bob!");
    chatApp.sendMessage("Bob", "Alice", "Hi, Alice!");

    return 0;
}
