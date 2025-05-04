#include <QtTest>
#include <QSignalSpy>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QTimer>
#include <QInputDialog>
#include <QDialogButtonBox>  // Add this header
#include "../ChatWindow.h"
#include "../NetworkWorker.h"
#include "../MessageListWidget.h"

// Mock class for NetworkWorker
class MockNetworkWorker : public NetworkWorker {
    Q_OBJECT
public:
    explicit MockNetworkWorker(QObject* parent = nullptr) : NetworkWorker(parent) {}
    
    bool isConnected() const { return mockConnected; }
    
public slots:
    void connectToServer(const QString&, quint16) {
        mockConnected = true;
        emit connected();
    }
    
    void sendCommand(const QString& command) {
        lastCommand = command;
        
        if (command.contains("getChatHistory")) {
            QString chatName = command.section(' ', 1, 1);
            emit responseReceived(QString("CHAT_HISTORY:%1:2\nuser1:Hello everyone\nuser2:Hi there").arg(chatName));
        }
        else if (command.contains("getChatUsers")) {
            QString chatName = command.section(' ', 1, 1);
            emit responseReceived(QString("CHAT_USERS:%1:3:user1:user2:user3").arg(chatName));
        }
        else if (command.contains("sendMessageInChat")) {
            emit responseReceived("Message sent to chat General");
        }
        else if (command.contains("createChat")) {
            QString chatName = command.section(' ', 1, 1);
            emit responseReceived(QString("Chat created: %1").arg(chatName));
        }
    }
    
public:
    bool mockConnected = false;
    QString lastCommand;
};

class ChatWindowTest : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testUIElements();
    void testSendMessage();
    void testChatSelection();
    void testCreateChat();
    void testJoinChat();
    void testElementVisibility();
    void testSendButtonSignal();
    void testCreateButtonSignal();
    void testJoinButtonSignal();
    
private:
    MessageListWidget* findChatDisplay(ChatWindow* window);
    QLineEdit* findMessageEdit(ChatWindow* window);
    QPushButton* findSendButton(ChatWindow* window);
    QListWidget* findChatList(ChatWindow* window);
    QPushButton* findCreateChatButton(ChatWindow* window);
    QPushButton* findJoinChatButton(ChatWindow* window);
    QPushButton* findLeaveChatButton(ChatWindow* window);
    QListWidget* findUserList(ChatWindow* window);
};

void ChatWindowTest::initTestCase()
{
    Q_INIT_RESOURCE(client_resources);
}

void ChatWindowTest::cleanupTestCase()
{}

void ChatWindowTest::testUIElements()
{
    ChatWindow window("testuser", "test_token");
    
    QVERIFY(findMessageEdit(&window) != nullptr);
    QVERIFY(findSendButton(&window) != nullptr);
    QVERIFY(findChatList(&window) != nullptr);
    QVERIFY(findCreateChatButton(&window) != nullptr);
    QVERIFY(findJoinChatButton(&window) != nullptr);
    QVERIFY(findLeaveChatButton(&window) != nullptr);
    QVERIFY(findUserList(&window) != nullptr);
    
    QCOMPARE(window.windowTitle(), QString("Chat - testuser"));
}

void ChatWindowTest::testSendMessage()
{
    ChatWindow window("testuser", "test_token");
    
    window.show();
    QTest::qWaitForWindowExposed(&window);
    
    // Access UI elements
    QLineEdit* messageEdit = findMessageEdit(&window);
    QPushButton* sendButton = findSendButton(&window);
    MessageListWidget* chatDisplay = findChatDisplay(&window);
    
    int initialMessageCount = chatDisplay->findChildren<QWidget*>("messageItem").count();
    messageEdit->setText("Hello, world!");
    
    QSignalSpy sendClickedSpy(sendButton, &QPushButton::clicked);
    
    QTest::mouseClick(sendButton, Qt::LeftButton);
    QCOMPARE(sendClickedSpy.count(), 1);
    
    QApplication::processEvents();
    QTest::qWait(100);
    
    int newMessageCount = chatDisplay->findChildren<QWidget*>("messageItem").count();
    
    QVERIFY(newMessageCount > initialMessageCount);
}

void ChatWindowTest::testChatSelection()
{
    ChatWindow window("testuser", "test_token");
    
    QListWidget* chatList = findChatList(&window);
    
    bool hasGeneralChat = false;
    for (int i = 0; i < chatList->count(); i++) {
        if (chatList->item(i)->text() == "General") {
            hasGeneralChat = true;
            break;
        }
    }
    QVERIFY(hasGeneralChat);
    
    if (chatList->count() > 0) {
        chatList->setCurrentRow(0);
    }
}

void ChatWindowTest::testCreateChat()
{
    ChatWindow window("testuser", "test_token");
    
    QPushButton* createChatButton = findCreateChatButton(&window);
    
    QVERIFY(createChatButton != nullptr);
}

void ChatWindowTest::testJoinChat()
{
    ChatWindow window("testuser", "test_token");
    
    QPushButton* joinChatButton = findJoinChatButton(&window);
    QVERIFY(joinChatButton != nullptr);
}

void ChatWindowTest::testElementVisibility()
{
    ChatWindow window("testuser", "test_token");
    
    window.show();
    QTest::qWaitForWindowExposed(&window);
    
    QLineEdit* messageEdit = findMessageEdit(&window);
    QPushButton* sendButton = findSendButton(&window);
    QListWidget* chatList = findChatList(&window);
    QPushButton* createChatButton = findCreateChatButton(&window);
    QPushButton* joinChatButton = findJoinChatButton(&window);
    QPushButton* leaveChatButton = findLeaveChatButton(&window);
    QListWidget* userList = findUserList(&window);
    
    QVERIFY(messageEdit->isVisible());
    QVERIFY(sendButton->isVisible());
    QVERIFY(chatList->isVisible());
    QVERIFY(createChatButton->isVisible());
    QVERIFY(joinChatButton->isVisible());
    QVERIFY(userList->isVisible());
    
    QVERIFY(messageEdit->isEnabled());
    QVERIFY(sendButton->isEnabled());
    QVERIFY(createChatButton->isEnabled());
    QVERIFY(joinChatButton->isEnabled());
}

void ChatWindowTest::testSendButtonSignal()
{
    ChatWindow window("testuser", "test_token");
    
    window.show();
    QTest::qWaitForWindowExposed(&window);
    
    QLineEdit* messageEdit = findMessageEdit(&window);
    QPushButton* sendButton = findSendButton(&window);
    
    QSignalSpy sendClickedSpy(sendButton, &QPushButton::clicked);
    
    messageEdit->setText("Test message");
    
    QApplication::processEvents();
    
    QTest::mouseClick(sendButton, Qt::LeftButton);
    QCOMPARE(sendClickedSpy.count(), 1);
}

void ChatWindowTest::testCreateButtonSignal()
{
    ChatWindow window("testuser", "test_token");
    
    window.show();
    QTest::qWaitForWindowExposed(&window);
    
    QPushButton* createChatButton = findCreateChatButton(&window);
    
    QSignalSpy createChatClickedSpy(createChatButton, &QPushButton::clicked);
    
    
    QApplication::processEvents();
    
    
    createChatButton->click();
    QCOMPARE(createChatClickedSpy.count(), 1);
}

void ChatWindowTest::testJoinButtonSignal()
{
    ChatWindow window("testuser", "test_token");
    
    window.show();
    QTest::qWaitForWindowExposed(&window);
    
    QPushButton* joinChatButton = findJoinChatButton(&window);
    
    QSignalSpy joinChatClickedSpy(joinChatButton, &QPushButton::clicked);
    
    
    QApplication::processEvents();
    
    joinChatButton->click();
    QCOMPARE(joinChatClickedSpy.count(), 1);
}

// Helper methods implementation
MessageListWidget* ChatWindowTest::findChatDisplay(ChatWindow* window)
{
    return window->findChild<MessageListWidget*>("chatDisplay");
}

QLineEdit* ChatWindowTest::findMessageEdit(ChatWindow* window)
{
    return window->findChild<QLineEdit*>("messageEdit");
}

QPushButton* ChatWindowTest::findSendButton(ChatWindow* window)
{
    return window->findChild<QPushButton*>("sendButton");
}

QListWidget* ChatWindowTest::findChatList(ChatWindow* window)
{
    return window->findChild<QListWidget*>("chatList");
}

QPushButton* ChatWindowTest::findCreateChatButton(ChatWindow* window)
{
    return window->findChild<QPushButton*>("createChatButton");
}

QPushButton* ChatWindowTest::findJoinChatButton(ChatWindow* window)
{
    return window->findChild<QPushButton*>("joinChatButton");
}

QPushButton* ChatWindowTest::findLeaveChatButton(ChatWindow* window)
{
    return window->findChild<QPushButton*>("leaveChatButton");
}

QListWidget* ChatWindowTest::findUserList(ChatWindow* window)
{
    return window->findChild<QListWidget*>("userList");
}

QTEST_MAIN(ChatWindowTest)
#include "ChatWindowTest.moc"
