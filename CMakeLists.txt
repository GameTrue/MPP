cmake_minimum_required(VERSION 3.10)
project(ChatApp)

set(CMAKE_CXX_STANDARD 17)

# Define BOOST_BIND_GLOBAL_PLACEHOLDERS to avoid the warning
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

# Set Boost paths - проверяем что компоненты найдены
find_package(Boost REQUIRED COMPONENTS system thread log log_setup unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

# Common library code (back/src)
set(CORE_SOURCES
    back/src/ChatApp.cpp
    back/src/User.cpp
    back/src/Message.cpp
    back/src/UserManager.cpp
    back/src/PasswordHasher.cpp
    back/src/Chat.cpp
)

add_library(chat_core ${CORE_SOURCES})
target_include_directories(chat_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/back/src)

# Собираем серверную часть
add_executable(chat_server server/BoostServer.cpp)

target_link_libraries(chat_server
    chat_core
    ${Boost_LIBRARIES}
)

if(WIN32)
    target_link_libraries(chat_server ws2_32 mswsock)
endif()

# Собираем тесты
set(TEST_SOURCES
    back/tests/test_chatapp.cpp
)

add_executable(chat_tests ${TEST_SOURCES})

# Add Boost Test compile definitions
set_target_properties(chat_tests PROPERTIES
    COMPILE_DEFINITIONS "BOOST_TEST_DYN_LINK;BOOST_TEST_MAIN"
)

target_link_libraries(chat_tests
    chat_core
    ${Boost_LIBRARIES}
)

# Добавляем клиентскую директорию для сборки Qt клиента
add_subdirectory(client)
