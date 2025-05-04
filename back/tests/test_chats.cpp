#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "../src/ChatApp.h"
#include "../src/User.h"
#include "../src/Message.h"
#include "../src/Chat.h"

// Фикстура для тестов чатов
struct ChatTestFixture {
    ChatTestFixture() {
        // Настройка тестового окружения
        chatApp = std::make_unique<ChatApp>();
        
        // Регистрируем тестовых пользователей
        user1 = User("ChatUser1", "password1");
        user2 = User("ChatUser2", "password2");
        user3 = User("ChatUser3", "password3");
        
        chatApp->registerUser(user1);
        chatApp->registerUser(user2);
        chatApp->registerUser(user3);
        
        // Входим от имени пользователей
        token1 = chatApp->login("ChatUser1", "password1");
        token2 = chatApp->login("ChatUser2", "password2");
        token3 = chatApp->login("ChatUser3", "password3");
    }
    
    ~ChatTestFixture() {}
    
    std::unique_ptr<ChatApp> chatApp;
    User user1, user2, user3;
    std::string token1, token2, token3;
};

BOOST_FIXTURE_TEST_SUITE(ChatTests, ChatTestFixture)

// Тест создания чата
BOOST_AUTO_TEST_CASE(TestCreateChat) {
    // Создание чата
    BOOST_CHECK_NO_THROW(chatApp->createChat("TestRoom"));
    
    // Проверяем, что чат был создан
    const auto& chats = chatApp->getChats();
    BOOST_CHECK(!chats.empty());
    
    if (!chats.empty()) {
        bool found = false;
        for (const auto& chat : chats) {
            if (chat.getChatName() == "TestRoom") {
                found = true;
                break;
            }
        }
        BOOST_CHECK(found);
    }
}

// Тест присоединения к чату
BOOST_AUTO_TEST_CASE(TestJoinChat) {
    // Создание чата
    chatApp->createChat("JoinChatTest");
    
    // Присоединение пользователей к чату
    BOOST_CHECK_NO_THROW(chatApp->joinChat("JoinChatTest", user1));
    BOOST_CHECK_NO_THROW(chatApp->joinChat("JoinChatTest", user2));
    
    // Проверяем, что пользователи присоединились к чату
    const auto& chats = chatApp->getChats();
    for (const auto& chat : chats) {
        if (chat.getChatName() == "JoinChatTest") {
            const auto& users = chat.getUsers();
            BOOST_CHECK_EQUAL(users.size(), 2);
            
            bool user1Found = false, user2Found = false;
            for (const auto& user : users) {
                if (user.getUsername() == "ChatUser1") user1Found = true;
                if (user.getUsername() == "ChatUser2") user2Found = true;
            }
            
            BOOST_CHECK(user1Found);
            BOOST_CHECK(user2Found);
            break;
        }
    }
}

// Тест отправки сообщения в чат
BOOST_AUTO_TEST_CASE(TestSendMessageInChat) {
    // Создание чата и присоединение пользователей
    chatApp->createChat("ChatMessageTest");
    chatApp->joinChat("ChatMessageTest", user1);
    chatApp->joinChat("ChatMessageTest", user2);
    
    // Отправка сообщения в чат
    BOOST_CHECK_NO_THROW(chatApp->sendMessageInChat(token1, "ChatMessageTest", "Привет всем в чате!"));
    
    // Проверяем, что сообщение добавилось в чат
    const auto& chats = chatApp->getChats();
    for (const auto& chat : chats) {
        if (chat.getChatName() == "ChatMessageTest") {
            const auto& messages = chat.getMessages();
            BOOST_CHECK(!messages.empty());
            
            if (!messages.empty()) {
                BOOST_CHECK_EQUAL(messages.back().getFrom(), "ChatUser1");
                BOOST_CHECK_EQUAL(messages.back().getTo(), "ChatMessageTest");
                BOOST_CHECK_EQUAL(messages.back().getContent(), "Привет всем в чате!");
            }
            break;
        }
    }
}

// Тест выхода из чата
BOOST_AUTO_TEST_CASE(TestLeaveChat) {
    // Создание чата и присоединение пользователей
    chatApp->createChat("LeaveChatTest");
    chatApp->joinChat("LeaveChatTest", user1);
    chatApp->joinChat("LeaveChatTest", user2);
    chatApp->joinChat("LeaveChatTest", user3);
    
    // Выход из чата
    BOOST_CHECK_NO_THROW(chatApp->leaveChat("LeaveChatTest", "ChatUser2"));
    
    // Проверяем, что пользователь вышел из чата
    const auto& chats = chatApp->getChats();
    for (const auto& chat : chats) {
        if (chat.getChatName() == "LeaveChatTest") {
            const auto& users = chat.getUsers();
            BOOST_CHECK_EQUAL(users.size(), 2); // осталось 2 пользователя
            
            bool user2Found = false;
            for (const auto& user : users) {
                if (user.getUsername() == "ChatUser2") user2Found = true;
            }
            
            BOOST_CHECK(!user2Found); // user2 не должен быть в чате
            break;
        }
    }
}

// Тест отправки сообщения в несуществующий чат
BOOST_AUTO_TEST_CASE(TestSendMessageToNonExistentChat) {
    // Попытка отправки сообщения в несуществующий чат (должно выбросить исключение)
    BOOST_CHECK_THROW(chatApp->sendMessageInChat(token1, "NonExistentChat", "Это сообщение не должно отправиться"), std::exception);
}

BOOST_AUTO_TEST_SUITE_END()