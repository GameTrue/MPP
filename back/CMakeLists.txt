cmake_minimum_required(VERSION 3.10)
project(ChatApp)

# set(CMAKE_CXX_STANDARD 17)

# set(BOOST_ROOT "C:/Program Files/boost/boost_1_87_0")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(chatapp 
    Tasks/src/ChatApp.cpp
    Tasks/src/User.cpp
    Tasks/src/Message.cpp
    Tasks/src/UserManager.cpp
    Tasks/src/PasswordHasher.cpp
    server/BoostServer.cpp
)

# target_link_libraries(chatapp)
