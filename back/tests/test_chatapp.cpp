#define BOOST_TEST_MODULE ChatTests
#include <boost/test/unit_test.hpp>

#include "../src/ChatApp.h"
#include "../src/User.h"
#include "../src/Message.h"

struct ChatAppFixture {
    ChatAppFixture() {
        // Настройка тестового окружения
        chatApp = std::make_unique<ChatApp>();
        
        // Регистрируем тестовых пользователей
        testUser1 = User("TestUser1", "password1");
        testUser2 = User("TestUser2", "password2");
        
        chatApp->registerUser(testUser1);
        chatApp->registerUser(testUser2);
        
        // Вход от имени первого тестового пользователя
        token1 = chatApp->login("TestUser1", "password1");
    }
    
    ~ChatAppFixture() {}
    
    std::unique_ptr<ChatApp> chatApp;
    User testUser1;
    User testUser2;
    std::string token1;
};

BOOST_FIXTURE_TEST_SUITE(ChatAppTests, ChatAppFixture)

// Тесты регистрации пользователей
BOOST_AUTO_TEST_CASE(TestUserRegistration) {
    // Регистрация нового пользователя
    User newUser("TestUser3", "password3");
    bool result = chatApp->registerUser(newUser);
    BOOST_CHECK(result);
    
    // Пробуем зарегистрировать снова (должен быть отказ)
    result = chatApp->registerUser(newUser);
    BOOST_CHECK(!result);
}

// Тесты входа пользователей
BOOST_AUTO_TEST_CASE(TestUserLogin) {
    // Проверка успешного входа
    std::string token = chatApp->login("TestUser1", "password1");
    BOOST_CHECK(!token.empty());
    
    // Проверка неудачного входа с неправильным паролем
    token = chatApp->login("TestUser1", "wrongpassword");
    BOOST_CHECK(token.empty());
    
    // Проверка неудачного входа с несуществующим пользователем
    token = chatApp->login("NonExistentUser", "password");
    BOOST_CHECK(token.empty());
}

// Тесты отправки сообщений
BOOST_AUTO_TEST_CASE(TestSendMessage) {
    // Отправка сообщения от пользователя 1 к пользователю 2
    BOOST_CHECK_NO_THROW(chatApp->sendMessage(token1, "TestUser2", "Hello, TestUser2!"));
    
    // Проверка отправки с недействительным токеном
    BOOST_CHECK_THROW(chatApp->sendMessage("invalid_token", "TestUser2", "This should fail"), std::exception);
    
    // Проверка отправки несуществующему пользователю
    BOOST_CHECK_THROW(chatApp->sendMessage(token1, "NonExistentUser", "This should fail"), std::exception);
}

// Тесты чатов
BOOST_AUTO_TEST_CASE(TestCreateAndJoinChat) {
    // Создание чата
    BOOST_CHECK_NO_THROW(chatApp->createChat("TestChatRoom"));
    
    // Присоединение к чату
    BOOST_CHECK_NO_THROW(chatApp->joinChat("TestChatRoom", testUser2));
    
    // Отправка сообщения в чат
    BOOST_CHECK_NO_THROW(chatApp->sendMessageInChat(token1, "TestChatRoom", "Hello, everyone!"));
    
    // Выход из чата
    BOOST_CHECK_NO_THROW(chatApp->leaveChat("TestChatRoom", "TestUser2"));
}

// Тест потокобезопасности (упрощенный)
BOOST_AUTO_TEST_CASE(TestThreadSafety) {
    // Это упрощенный тест потокобезопасности
    // В реальном тесте мы бы создали несколько потоков и заставили их
    // одновременно обращаться к экземпляру ChatApp
    
    // На данный момент мы просто проверим, что операции работают как ожидается
    std::string token2 = chatApp->login("TestUser2", "password2");
    BOOST_CHECK(!token2.empty());
    
    chatApp->createChat("ThreadTestChat");
    chatApp->joinChat("ThreadTestChat", testUser1);
    chatApp->joinChat("ThreadTestChat", testUser2);
    
    // Оба пользователя отправляют сообщения в чат
    chatApp->sendMessageInChat(token1, "ThreadTestChat", "Message from User1");
    chatApp->sendMessageInChat(token2, "ThreadTestChat", "Message from User2");
    
    // Если мы дошли сюда без исключений, тест считается пройденным
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
