cmake_minimum_required(VERSION 3.10)

# Настройки Qt
find_package(Qt5 COMPONENTS Core Widgets Network Test REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)  # Removed as we're not using UI files anymore

# Define resources
set(RESOURCES
    resources/client_resources.qrc
)

# Main application
add_executable(chat_client
    ClientExample.cpp
    ChatWindow.cpp
    ChatWindow.h
    LoginDialog.cpp
    LoginDialog.h
    NetworkWorker.cpp
    NetworkWorker.h
    MessageItem.cpp
    MessageItem.h
    MessageListWidget.cpp
    MessageListWidget.h
    ${RESOURCES}
)

target_link_libraries(chat_client
    chat_core 
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    ${Boost_LIBRARIES}
)

if(WIN32)
    target_link_libraries(chat_client ws2_32 mswsock)
endif()

# Tests
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    # Add the test subdirectory
    add_subdirectory(tests)
endif()