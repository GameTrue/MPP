#include <mutex>
#include <iostream>

std::mutex mtx;

void print_numbers() {
    std::lock_guard<std::mutex> guard(mtx);
    for (int i = 0; i < 10; ++i) {
        std::cout << i << std::endl;
    }
}

int main() {
    print_numbers();
    return 0;
}
