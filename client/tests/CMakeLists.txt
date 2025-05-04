cmake_minimum_required(VERSION 3.10)
project(chat_client_tests)

find_package(Qt5 COMPONENTS Core Widgets Network Test REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_executable(login_dialog_test
    LoginDialogTest.cpp
    ../LoginDialog.cpp
    ../NetworkWorker.cpp
    ../resources/client_resources.qrc
)

add_executable(chat_window_test
    ChatWindowTest.cpp
    ../ChatWindow.cpp
    ../NetworkWorker.cpp
    ../MessageItem.cpp      
    ../MessageListWidget.cpp 
    ../resources/client_resources.qrc
)

target_link_libraries(login_dialog_test
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Test
)

target_link_libraries(chat_window_test
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Test
)

enable_testing()
add_test(NAME LoginDialogTest COMMAND login_dialog_test)
add_test(NAME ChatWindowTest COMMAND chat_window_test)
# add_test(NAME NetworkWorkerTest COMMAND network_worker_test)
