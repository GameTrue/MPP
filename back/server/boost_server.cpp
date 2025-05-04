#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

void session(tcp::socket socket) {
    try {
        for (;;) {
            char data[512];
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // connection closed cleanly 
            else if (error)
                throw boost::system::system_error(error);
            boost::asio::write(socket, boost::asio::buffer(data, length));
        }
    } catch(std::exception& e) {
        std::cerr << "Exception in session: " << e.what() << "\n";
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "Server started on port 12345" << std::endl;

        // Пул потоков
        std::vector<std::thread> threadPool;
        const int numThreads = std::thread::hardware_concurrency();

        for (;;) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            // Для каждого нового клиента запускаем сессию в новом потоке.
            threadPool.emplace_back(std::thread(session, std::move(socket)));
        }
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
