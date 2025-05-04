#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <unordered_map>
#include <set>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include "../back/src/ChatApp.h"
#include "../back/src/User.h"

using boost::asio::ip::tcp;

// Структура для управления подписками на чаты
class ChatSubscriptionManager {
private:
    std::unordered_map<std::string, std::set<std::shared_ptr<tcp::socket>>> chatSubscribers;
    std::unordered_map<std::string, std::string> userTokens; // username -> token
    std::unordered_map<std::string, std::set<std::string>> userChats; // username -> set of chat names
    std::mutex mtx;

public:
    void subscribeToChat(const std::string& chatName, std::shared_ptr<tcp::socket> socket, const std::string& username) {
        std::lock_guard<std::mutex> lock(mtx);
        chatSubscribers[chatName].insert(socket);
        userChats[username].insert(chatName);
        BOOST_LOG_TRIVIAL(info) << "User " << username << " subscribed to chat " << chatName;
    }

    void unsubscribeFromChat(const std::string& chatName, const std::string& username) {
        std::lock_guard<std::mutex> lock(mtx);
        // Удаляем чат из списка пользователя
        if (userChats.find(username) != userChats.end()) {
            userChats[username].erase(chatName);
            BOOST_LOG_TRIVIAL(info) << "User " << username << " unsubscribed from chat " << chatName;
        }
    }

    void unsubscribeUser(const std::string& username, std::shared_ptr<tcp::socket> socket) {
        std::lock_guard<std::mutex> lock(mtx);
        // Удаляем пользователя из всех чатов
        if (userChats.find(username) != userChats.end()) {
            for (const auto& chatName : userChats[username]) {
                if (chatSubscribers.find(chatName) != chatSubscribers.end()) {
                    chatSubscribers[chatName].erase(socket);
                    BOOST_LOG_TRIVIAL(info) << "User " << username << " removed from chat " << chatName;
                }
            }
            userChats.erase(username);
        }
    }

    void broadcastMessage(const std::string& chatName, const std::string& from, const std::string& message,
                        std::shared_ptr<tcp::socket> senderSocket) {
        std::lock_guard<std::mutex> lock(mtx);
        if (chatSubscribers.find(chatName) != chatSubscribers.end()) {
            std::string formattedMessage = "CHAT_MESSAGE:" + chatName + ":" + from + ":" + message + "\n";
            
            for (auto& socket : chatSubscribers[chatName]) {
                // Не отправляем сообщение отправителю
                if (socket != senderSocket) {
                    try {
                        boost::asio::write(*socket, boost::asio::buffer(formattedMessage));
                        BOOST_LOG_TRIVIAL(debug) << "Message broadcasted to a user in chat " << chatName;
                    } catch (std::exception& e) {
                        BOOST_LOG_TRIVIAL(error) << "Error broadcasting message: " << e.what();
                    }
                }
            }
            BOOST_LOG_TRIVIAL(info) << "Message from " << from << " broadcasted to " << 
                                   chatSubscribers[chatName].size() - 1 << " users in chat " << chatName;
        }
    }

    void registerUserToken(const std::string& username, const std::string& token) {
        std::lock_guard<std::mutex> lock(mtx);
        userTokens[username] = token;
    }

    std::string getUsernameByToken(const std::string& token) {
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& pair : userTokens) {
            if (pair.second == token) {
                return pair.first;
            }
        }
        return "";
    }
};

// Глобальный менеджер подписок
ChatSubscriptionManager subscriptionManager;

// Потокобезопасный ChatApp с защитой мьютексом
class ThreadSafeChatApp {
private:
    ChatApp chatApp;
    mutable std::mutex mtx;

public:
    ThreadSafeChatApp() = default;

    bool registerUser(const User& user) {
        std::lock_guard<std::mutex> lock(mtx);
        return chatApp.registerUser(user);
    }

    std::string login(const std::string& username, const std::string& password) {
        std::lock_guard<std::mutex> lock(mtx);
        std::string token = chatApp.login(username, password);
        if (!token.empty()) {
            subscriptionManager.registerUserToken(username, token);
        }
        return token;
    }

    void sendMessage(const std::string& token, const std::string& to, const std::string& content) {
        std::lock_guard<std::mutex> lock(mtx);
        chatApp.sendMessage(token, to, content);
    }

    void createChat(const std::string& chatName) {
        std::lock_guard<std::mutex> lock(mtx);
        chatApp.createChat(chatName);
    }

    void joinChat(const std::string& chatName, const User& user) {
        std::lock_guard<std::mutex> lock(mtx);
        chatApp.joinChat(chatName, user);
    }

    void sendMessageInChat(const std::string& token, const std::string& chatName, const std::string& content) {
        std::lock_guard<std::mutex> lock(mtx);
        chatApp.sendMessageInChat(token, chatName, content);
    }

    void leaveChat(const std::string& chatName, const std::string& username) {
        std::lock_guard<std::mutex> lock(mtx);
        chatApp.leaveChat(chatName, username);
        subscriptionManager.unsubscribeFromChat(chatName, username);
    }

    std::vector<Message> getChatMessages(const std::string& chatName) {
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& chat : chatApp.getChats()) {
            if (chat.getChatName() == chatName) {
                return chat.getMessages();
            }
        }
        return std::vector<Message>();
    }
    
    std::vector<std::string> getChatUsers(const std::string& chatName) {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<std::string> result;
        for (const auto& chat : chatApp.getChats()) {
            if (chat.getChatName() == chatName) {
                for (const auto& user : chat.getUsers()) {
                    result.push_back(user.getUsername());
                }
                break;
            }
        }
        return result;
    }
    
    std::vector<std::string> getAllUsers() {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<std::string> result;
        for (const auto& user : chatApp.getUsers()) {
            result.push_back(user.getUsername());
        }
        return result;
    }
};

// Глобальное потокобезопасное приложение чата
ThreadSafeChatApp safeChatApp;

// Инициализация логирования
void init_logging() {
    namespace logging = boost::log;
    namespace keywords = boost::log::keywords;
    namespace expr = boost::log::expressions;

    logging::add_file_log(
        keywords::file_name = "server_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = logging::sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = (
            expr::stream 
                << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] "
                << "[Thread " << expr::attr<logging::attributes::current_thread_id::value_type>("ThreadID") << "] "
                << "[" << logging::trivial::severity << "] "
                << expr::smessage
        )
    );

    logging::add_common_attributes();
    
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );
}

// Обработка команд от клиентов
void process_command(const std::string& command, std::string& response, std::shared_ptr<tcp::socket> clientSocket) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    
    BOOST_LOG_TRIVIAL(info) << "Processing command: " << cmd;
    
    if(cmd == "register") {
        std::string username, password;
        iss >> username >> password;
        User user(username, password);
        bool result = safeChatApp.registerUser(user);
        response = result ? "User registered successfully" : "User already exists";
        BOOST_LOG_TRIVIAL(info) << "Register attempt for user " << username << ": " << response;
    }
    else if(cmd == "login") {
        std::string username, password;
        iss >> username >> password;
        std::string token = safeChatApp.login(username, password);
        if (!token.empty()) {
            // Автоматически подписываем на чат General при успешном входе
            try {
                User user(username, password);
                safeChatApp.joinChat("General", user);
                subscriptionManager.subscribeToChat("General", clientSocket, username);
                BOOST_LOG_TRIVIAL(info) << "User " << username << " automatically joined General chat";
            } catch (std::exception& e) {
                BOOST_LOG_TRIVIAL(error) << "Error auto-joining General chat: " << e.what();
            }
        }
        response = !token.empty() ? "Token: " + token : "Login failed: incorrect credentials";
        BOOST_LOG_TRIVIAL(info) << "Login attempt for user " << username << ": " << (token.empty() ? "Failed" : "Success");
    }
    else if(cmd == "sendMessage") {
        std::string token, to;
        iss >> token >> to;
        std::string content;
        std::getline(iss, content);
        if (content.size() > 0 && content[0] == ' ')
            content = content.substr(1);
        
        try {
            safeChatApp.sendMessage(token, to, content);
            response = "Message sent to " + to;
            BOOST_LOG_TRIVIAL(info) << "Message sent to user " << to;
        } catch (const std::exception& e) {
            response = "Error: " + std::string(e.what());
            BOOST_LOG_TRIVIAL(error) << "Error sending message: " << e.what();
        }
    }
    else if(cmd == "createChat") {
        std::string chatName;
        iss >> chatName;
        try {
            safeChatApp.createChat(chatName);
            response = "Chat created: " + chatName;
            BOOST_LOG_TRIVIAL(info) << "Chat created: " << chatName;
        } catch (const std::exception& e) {
            response = "Error: " + std::string(e.what());
            BOOST_LOG_TRIVIAL(error) << "Error creating chat: " << e.what();
        }
    }
    else if(cmd == "joinChat") {
        std::string chatName, username, password;
        iss >> chatName >> username >> password;
        User user(username, password);
        try {
            safeChatApp.joinChat(chatName, user);
            // Подписываем пользователя на чат
            subscriptionManager.subscribeToChat(chatName, clientSocket, username);
            response = "User " + username + " joined chat " + chatName;
            BOOST_LOG_TRIVIAL(info) << "User " << username << " joined chat " << chatName;
        } catch (const std::exception& e) {
            response = "Error: " + std::string(e.what());
            BOOST_LOG_TRIVIAL(error) << "Error joining chat: " << e.what();
        }
    }
    else if(cmd == "sendMessageInChat") {
        std::string token, chatName;
        iss >> token >> chatName;
        std::string content;
        std::getline(iss, content);
        if (content.size() > 0 && content[0] == ' ')
            content = content.substr(1);
            
        try {
            safeChatApp.sendMessageInChat(token, chatName, content);
            
            // Получаем имя пользователя по токену
            std::string username = subscriptionManager.getUsernameByToken(token);
            
            if(!username.empty()) {
                subscriptionManager.broadcastMessage(chatName, username, content, clientSocket);
            }
            
            response = "Message sent to chat " + chatName;
            BOOST_LOG_TRIVIAL(info) << "Message sent to chat " << chatName;
        } catch (const std::exception& e) {
            response = "Error: " + std::string(e.what());
            BOOST_LOG_TRIVIAL(error) << "Error sending message to chat: " << e.what();
        }
    }
    else if(cmd == "leaveChat") {
        std::string chatName, username;
        iss >> chatName >> username;
        try {
            safeChatApp.leaveChat(chatName, username);
            response = "User " + username + " left chat " + chatName;
            BOOST_LOG_TRIVIAL(info) << "User " << username << " left chat " << chatName;
        } catch (const std::exception& e) {
            response = "Error: " + std::string(e.what());
            BOOST_LOG_TRIVIAL(error) << "Error leaving chat: " << e.what();
        }
    }
    else if(cmd == "getChatHistory") {
        std::string chatName;
        iss >> chatName;
        
        try {
            auto messages = safeChatApp.getChatMessages(chatName);
            std::ostringstream oss;
            oss << "CHAT_HISTORY:" << chatName << ":" << messages.size() << "\n";
            
            for (const auto& msg : messages) {
                oss << msg.getFrom() << ":" << msg.getContent() << "\n";
            }
            
            response = oss.str();
            if (!response.empty() && response.back() == '\n') {
                response.pop_back();
            }
            
            BOOST_LOG_TRIVIAL(info) << "Sent " << messages.size() << " messages from chat " << chatName;
        } catch (const std::exception& e) {
            response = "Error: " + std::string(e.what());
            BOOST_LOG_TRIVIAL(error) << "Error getting chat history: " << e.what();
        }
    }
    else if(cmd == "getChatUsers") {
        std::string chatName;
        iss >> chatName;
        
        try {
            auto users = safeChatApp.getChatUsers(chatName);
            std::ostringstream oss;
            oss << "CHAT_USERS:" << chatName << ":" << users.size();
            
            for (const auto& username : users) {
                oss << ":" << username;
            }
            
            response = oss.str();
            BOOST_LOG_TRIVIAL(info) << "Sent list of " << users.size() << " users in chat " << chatName;
        } catch (const std::exception& e) {
            response = "Error: " + std::string(e.what());
            BOOST_LOG_TRIVIAL(error) << "Error getting chat users: " << e.what();
        }
    }
    else if(cmd == "getAllUsers") {
        try {
            auto users = safeChatApp.getAllUsers();
            std::ostringstream oss;
            oss << "ALL_USERS:" << users.size();
            
            for (const auto& username : users) {
                oss << ":" << username;
            }
            
            response = oss.str();
            BOOST_LOG_TRIVIAL(info) << "Sent list of all " << users.size() << " users";
        } catch (const std::exception& e) {
            response = "Error: " + std::string(e.what());
            BOOST_LOG_TRIVIAL(error) << "Error getting all users: " << e.what();
        }
    }
    else {
        response = "Unknown command: " + cmd;
        BOOST_LOG_TRIVIAL(warning) << "Unknown command received: " << cmd;
    }
}

// Обработка клиентской сессии
class Session : public std::enable_shared_from_this<Session> {
private:
    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    std::string client_address_;
    std::string response_;
    std::string current_username_;

public:
    Session(tcp::socket socket) 
        : socket_(std::move(socket)) {
        client_address_ = socket_.remote_endpoint().address().to_string() + 
                         ":" + std::to_string(socket_.remote_endpoint().port());
    }

    ~Session() {
        if (!current_username_.empty()) {
            subscriptionManager.unsubscribeUser(current_username_, getSocket());
        }
    }

    void start() {
        BOOST_LOG_TRIVIAL(info) << "New client connected: " << client_address_;
        read_request();
    }

    std::shared_ptr<tcp::socket> getSocket() {
        return std::shared_ptr<tcp::socket>(&socket_, [](tcp::socket*){});
    }

private:
    void read_request() {
        auto self(shared_from_this());
        
        boost::asio::async_read_until(
            socket_, buffer_, "\n",
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string request;
                    std::istream is(&buffer_);
                    std::getline(is, request);
                    
                    BOOST_LOG_TRIVIAL(debug) << "Request from " << client_address_ << ": " << request;
                    
                    if (request.find("login ") == 0 || request.find("register ") == 0) {
                        std::istringstream iss(request);
                        std::string cmd, username;
                        iss >> cmd >> username;
                        current_username_ = username;
                    }
                    
                    process_command(request, response_, getSocket());
                    
                    response_ += "\n";
                    write_response();
                } else if (ec == boost::asio::error::eof) {
                    BOOST_LOG_TRIVIAL(info) << "Client disconnected: " << client_address_;
                } else {
                    BOOST_LOG_TRIVIAL(error) << "Read error from " << client_address_ << ": " << ec.message();
                }
            });
    }

    void write_response() {
        
        boost::asio::async_write(
            socket_, boost::asio::buffer(response_),
            [this, self = shared_from_this()](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    BOOST_LOG_TRIVIAL(debug) << "Response sent to " << client_address_ << ": " << response_;
                    
                    read_request();
                } else {
                    BOOST_LOG_TRIVIAL(error) << "Write error to " << client_address_ << ": " << ec.message();
                }
            });
    }
};

// Класс сервера для приема соединений
class Server {
private:
    tcp::acceptor acceptor_;
    boost::asio::io_context& io_context_;
    std::atomic<bool> running_;

public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          io_context_(io_context),
          running_(true) {
        BOOST_LOG_TRIVIAL(info) << "Server started on port " << port;
        
        try {
            safeChatApp.createChat("General");
            BOOST_LOG_TRIVIAL(info) << "Default 'General' chat created";
        } catch (std::exception& e) {
            BOOST_LOG_TRIVIAL(error) << "Failed to create default chat: " << e.what();
        }
        
        accept_connection();
    }

    void stop() {
        running_ = false;
        boost::system::error_code ec;
        acceptor_.close(ec);
        if (ec) {
            BOOST_LOG_TRIVIAL(error) << "Error closing acceptor: " << ec.message();
        } else {
            BOOST_LOG_TRIVIAL(info) << "Server stopping, acceptor closed";
        }
    }

private:
    void accept_connection() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec && running_) {
                    std::make_shared<Session>(std::move(socket))->start();
                } else if (ec && running_) {
                    BOOST_LOG_TRIVIAL(error) << "Accept error: " << ec.message();
                }

                if (running_) {
                    accept_connection();
                }
            });
    }
};

int main() {
    try {
        init_logging();
        BOOST_LOG_TRIVIAL(info) << "------------- Server Starting -------------";

        boost::asio::io_context io_context;
        
        // Создаем защиту работы, чтобы io_context продолжал работать
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> 
            work_guard = boost::asio::make_work_guard(io_context);
        
        Server server(io_context, 12345);
        
        // Запускаем рабочие потоки
        const int num_threads = std::thread::hardware_concurrency();
        BOOST_LOG_TRIVIAL(info) << "Starting " << num_threads << " worker threads";
        
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&io_context]() {
                try {
                    io_context.run();
                } catch (std::exception& e) {
                    BOOST_LOG_TRIVIAL(error) << "Thread exception: " << e.what();
                }
            });
        }
        
        std::cout << "Server running on port 12345. Press Enter to stop..." << std::endl;
        std::cin.get();
        
        BOOST_LOG_TRIVIAL(info) << "Stopping server...";
        server.stop();
        work_guard.reset();
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        BOOST_LOG_TRIVIAL(info) << "Server shutdown complete";
    } catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "Exception: " << e.what();
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
