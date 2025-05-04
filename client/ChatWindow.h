#pragma once

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include <vector>
#include <string>

// Forward declarations for Qt widgets
class QVBoxLayout;
class QHBoxLayout;
class QSplitter;
class QListWidget;
class QPlainTextEdit;
class QLineEdit;
class QPushButton;
class QLabel;
class QAction;
class QMenu;
class QMenuBar;
class QStatusBar;
class QWidget;
class QThread;

class NetworkWorker;
class MessageListWidget;  // Add this forward declaration

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(const QString& username, const QString& token, QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void onSendClicked();
    void onCreateChatClicked();
    void onJoinChatClicked();
    void onLeaveChatClicked();
    void onChatSelectionChanged();
    
    void onConnected();
    void onDisconnected();
    void onResponseReceived(const QString& response);
    void onNetworkError(const QString& errorMessage);
    
    void onRefreshTimer();
    
private:
    void setupUi();
    
    void connectToServer();
    void sendCommand(const QString& command);
    
    void addMessage(const QString& from, const QString& content, bool isSystem = false);
    void refreshChats();
    void refreshUsers();
    
    void requestChatHistory(const QString& chatName);
    void requestChatUsers(const QString& chatName);
    void processChatHistoryResponse(const QString& response);
    void processChatUsersResponse(const QString& response);
    void processAllUsersResponse(const QString& response);
    
    // Central widget and main layouts
    QWidget* centralWidget;
    QSplitter* splitter;
    
    // Left panel widgets
    QWidget* leftPanelWidget;
    QVBoxLayout* leftPanelLayout;
    QLabel* chatsLabel;
    QListWidget* chatList;
    QHBoxLayout* chatButtonsLayout;
    QPushButton* createChatButton;
    QPushButton* joinChatButton;
    QPushButton* leaveChatButton;
    QLabel* usersLabel;
    QListWidget* userList;
    
    // Right panel widgets
    QWidget* rightPanelWidget;
    QVBoxLayout* rightPanelLayout;
    MessageListWidget* chatDisplay;  // Change from QPlainTextEdit to MessageListWidget
    QHBoxLayout* messageLayout;
    QLineEdit* messageEdit;
    QPushButton* sendButton;
    
    // Menu and status bar
    QMenuBar* menuBar;
    QMenu* fileMenu;
    QAction* disconnectAction;
    QAction* exitAction;
    QStatusBar* statusBar;
    
    // Network thread and worker
    QThread* networkThread_;
    NetworkWorker* networkWorker_;
    
    QTimer refreshTimer_;
    
    QString username_;
    QString token_;
    QString currentChat_;
    
    static const QString SERVER_ADDRESS;
    static const quint16 SERVER_PORT;
    
    bool hasConfirmedGeneralChat = true; 
};