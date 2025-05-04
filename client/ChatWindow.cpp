#include "ChatWindow.h"
#include "NetworkWorker.h"
#include "MessageListWidget.h"  // Include the new header

#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>
#include <QFont>
#include <QSizePolicy>
#include <QThread>

// same as LoginDialog
const QString ChatWindow::SERVER_ADDRESS = "127.0.0.1";
const quint16 ChatWindow::SERVER_PORT = 12345;

ChatWindow::ChatWindow(const QString& username, const QString& token, QWidget *parent) :
    QMainWindow(parent),
    username_(username),
    token_(token),
    hasConfirmedGeneralChat(false)
{
    setupUi();
    
    networkThread_ = new QThread(this);
    networkWorker_ = new NetworkWorker();
    networkWorker_->moveToThread(networkThread_);
    
    connect(networkWorker_, &NetworkWorker::connected, this, &ChatWindow::onConnected);
    connect(networkWorker_, &NetworkWorker::disconnected, this, &ChatWindow::onDisconnected);
    connect(networkWorker_, &NetworkWorker::responseReceived, this, &ChatWindow::onResponseReceived);
    connect(networkWorker_, &NetworkWorker::errorOccurred, this, &ChatWindow::onNetworkError);
    
    connect(this, &ChatWindow::destroyed, networkWorker_, &NetworkWorker::deleteLater);
    connect(networkThread_, &QThread::finished, networkWorker_, &NetworkWorker::deleteLater);
    
    networkThread_->start();
    
    setObjectName("ChatWindow");
    
    setWindowTitle(QString("Chat - %1").arg(username_));
    
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendClicked);
    connect(createChatButton, &QPushButton::clicked, this, &ChatWindow::onCreateChatClicked);
    connect(joinChatButton, &QPushButton::clicked, this, &ChatWindow::onJoinChatClicked);
    connect(leaveChatButton, &QPushButton::clicked, this, &ChatWindow::onLeaveChatClicked);
    connect(chatList, &QListWidget::currentItemChanged, this, &ChatWindow::onChatSelectionChanged);
    
    connect(&refreshTimer_, &QTimer::timeout, this, &ChatWindow::onRefreshTimer);
    refreshTimer_.start(5000);
    
    connectToServer();
    addMessage("System", "Welcome to Chat App, " + username_ + "!", true);
    refreshChats();
    refreshUsers();
}

ChatWindow::~ChatWindow()
{
    networkThread_->quit();
    networkThread_->wait();
}

void ChatWindow::setupUi()
{
    resize(800, 600);
    
    // central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // main horizontal layout
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(9, 9, 9, 9);
    mainLayout->setSpacing(6);
    
    // Create splitter
    splitter = new QSplitter(Qt::Horizontal, centralWidget);
    mainLayout->addWidget(splitter);
    
    // Left panel 
    leftPanelWidget = new QWidget(splitter);
    leftPanelLayout = new QVBoxLayout(leftPanelWidget);
    leftPanelLayout->setContentsMargins(0, 0, 0, 0);
    leftPanelLayout->setSpacing(6);
    
    leftPanelWidget->setMinimumWidth(240);
    leftPanelWidget->setMaximumWidth(300);
    
    // chats section
    QFont boldFont;
    boldFont.setPointSize(10);
    boldFont.setBold(true);
    
    chatsLabel = new QLabel("Chats", leftPanelWidget);
    chatsLabel->setObjectName("chatsLabel");
    chatsLabel->setFont(boldFont);
    leftPanelLayout->addWidget(chatsLabel);
    
    chatList = new QListWidget(leftPanelWidget);
    chatList->setObjectName("chatList");
    chatList->setAlternatingRowColors(true);
    leftPanelLayout->addWidget(chatList);
    
    // chat buttons
    chatButtonsLayout = new QHBoxLayout();
    createChatButton = new QPushButton("Create", leftPanelWidget);
    createChatButton->setObjectName("createChatButton");
    joinChatButton = new QPushButton("Join", leftPanelWidget);
    joinChatButton->setObjectName("joinChatButton");
    leaveChatButton = new QPushButton("Leave", leftPanelWidget);
    leaveChatButton->setObjectName("leaveChatButton");
    
    chatButtonsLayout->addWidget(createChatButton);
    chatButtonsLayout->addWidget(joinChatButton);
    chatButtonsLayout->addWidget(leaveChatButton);
    leftPanelLayout->addLayout(chatButtonsLayout);
    
    // Horizontal line
    QFrame* line = new QFrame(leftPanelWidget);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    leftPanelLayout->addWidget(line);
    
    // Users section
    usersLabel = new QLabel("Users", leftPanelWidget);
    usersLabel->setObjectName("usersLabel");
    usersLabel->setFont(boldFont);
    leftPanelLayout->addWidget(usersLabel);
    
    userList = new QListWidget(leftPanelWidget);
    userList->setObjectName("userList");
    userList->setAlternatingRowColors(true);
    leftPanelLayout->addWidget(userList);
    
    // Right panel 
    rightPanelWidget = new QWidget(splitter);
    rightPanelLayout = new QVBoxLayout(rightPanelWidget);
    rightPanelLayout->setContentsMargins(0, 0, 0, 0);
    rightPanelLayout->setSpacing(6);
    
    // Use MessageListWidget instead of QPlainTextEdit
    chatDisplay = new MessageListWidget(rightPanelWidget);
    chatDisplay->setObjectName("chatDisplay");
    rightPanelLayout->addWidget(chatDisplay);
    
    // Message input area
    messageLayout = new QHBoxLayout();
    messageEdit = new QLineEdit(rightPanelWidget);
    messageEdit->setObjectName("messageEdit");
    messageEdit->setPlaceholderText("Type your message here...");
    sendButton = new QPushButton("Send", rightPanelWidget);
    sendButton->setObjectName("sendButton");
    
    messageLayout->addWidget(messageEdit);
    messageLayout->addWidget(sendButton);
    rightPanelLayout->addLayout(messageLayout);
    
    splitter->setCollapsible(0, false);
    splitter->setStretchFactor(0, 0);  
    splitter->setStretchFactor(1, 1);  
    
    QList<int> sizes;
    sizes << 250 << 550;  
    splitter->setSizes(sizes);
    
    // Menu bar 
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 800, 21));
    setMenuBar(menuBar);
    
    fileMenu = new QMenu("File", menuBar);
    menuBar->addAction(fileMenu->menuAction());
    
    disconnectAction = new QAction("Disconnect", this);
    exitAction = new QAction("Exit", this);
    
    fileMenu->addAction(disconnectAction);
    fileMenu->addAction(exitAction);
    
    // Status bar 
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
}

void ChatWindow::connectToServer()
{
    QMetaObject::invokeMethod(networkWorker_, "connectToServer", 
                             Q_ARG(QString, SERVER_ADDRESS),
                             Q_ARG(quint16, SERVER_PORT));
}

void ChatWindow::sendCommand(const QString& command)
{
    QMetaObject::invokeMethod(networkWorker_, "sendCommand", 
                             Q_ARG(QString, command));
}

void ChatWindow::addMessage(const QString& from, const QString& content, bool isSystem)
{
    QDateTime now = QDateTime::currentDateTime();
    
    // Debug output
    // qDebug() << "ChatWindow::addMessage - Adding message:" << from << content << isSystem;
    
    if (!chatDisplay) {
        // qDebug() << "ERROR: chatDisplay is null!";
        return;
    }
    
    chatDisplay->addMessage(from, content, now, isSystem);
}

void ChatWindow::refreshChats()
{
    QString currentChat = chatList->currentItem() ? chatList->currentItem()->text() : "";
    
    sendCommand(QString("getChats %1").arg(token_));
    
    chatList->clear();
    
    QListWidgetItem* generalItem = new QListWidgetItem("General");
    generalItem->setForeground(Qt::blue); 
    chatList->addItem(generalItem);
    
    hasConfirmedGeneralChat = true; 
    
    if (!currentChat.isEmpty()) {
        for (int i = 0; i < chatList->count(); i++) {
            if (chatList->item(i)->text() == currentChat) {
                chatList->setCurrentRow(i);
                break;
            }
        }
    } else {
        chatList->setCurrentRow(0);
    }
}

void ChatWindow::refreshUsers()
{
    sendCommand("getAllUsers");
}

void ChatWindow::onSendClicked()
{
    QString message = messageEdit->text().trimmed();
    if (message.isEmpty()) {
        return;
    }
    
    QString recipient = chatList->currentItem() ? chatList->currentItem()->text() : "";
    if (recipient.isEmpty()) {
        QMessageBox::warning(this, "Send Error", "Please select a chat room or user first");
        return;
    }
    
    messageEdit->clear();
    
    QString command;
    if (recipient == "General" || chatList->currentItem()->foreground().color() == Qt::blue) {
        command = QString("sendMessageInChat %1 %2 %3").arg(token_, recipient, message);
    } else {
        command = QString("sendMessage %1 %2 %3").arg(token_, recipient, message);
    }
    
    sendCommand(command);
    if (recipient != "General" || hasConfirmedGeneralChat) {
        addMessage(username_, message);
    }
}

void ChatWindow::onCreateChatClicked()
{
    bool ok;
    QString chatName = QInputDialog::getText(this, "Create Chat",
                                         "Enter chat room name:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !chatName.isEmpty()) {
        QString command = QString("createChat %1").arg(chatName);
        sendCommand(command);
        QTimer::singleShot(500, this, &ChatWindow::refreshChats);
    }
}

void ChatWindow::onJoinChatClicked()
{
    bool ok;
    QString chatName = QInputDialog::getText(this, "Join Chat",
                                         "Enter chat room name:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !chatName.isEmpty()) {
        QString command = QString("joinChat %1 %2 %3").arg(chatName, username_, token_);
        sendCommand(command);
        QListWidgetItem* item = new QListWidgetItem(chatName);
        item->setForeground(Qt::blue);
        chatList->addItem(item);
        chatList->setCurrentItem(item);
    }
}

void ChatWindow::onLeaveChatClicked()
{
    QListWidgetItem* currentItem = chatList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Leave Error", "Please select a chat room first");
        return;
    }
    
    QString chatName = currentItem->text();
    if (chatName == "General") {
        QMessageBox::warning(this, "Leave Error", "Cannot leave the General chat");
        return;
    }
    
    QString command = QString("leaveChat %1 %2").arg(chatName, username_);
    sendCommand(command);
    delete chatList->takeItem(chatList->row(currentItem));
    
    for (int i = 0; i < chatList->count(); i++) {
        if (chatList->item(i)->text() == "General") {
            chatList->setCurrentRow(i);
            break;
        }
    }
}

void ChatWindow::onChatSelectionChanged()
{
    QListWidgetItem* currentItem = chatList->currentItem();
    if (currentItem) {
        currentChat_ = currentItem->text();
        
        leaveChatButton->setEnabled(currentChat_ != "General");
        
        if (chatDisplay) {
            chatDisplay->clear();
        } else {
            // qDebug() << "ERROR: chatDisplay is null when changing chats!";
        }
        
        addMessage("System", "Switched to chat: " + currentChat_, true);
        
        // Request chat history when switching chats
        requestChatHistory(currentChat_);
        
        // Request user list for this chat
        requestChatUsers(currentChat_);
    }
}

void ChatWindow::requestChatHistory(const QString& chatName)
{
    QString command = QString("getChatHistory %1").arg(chatName);
    sendCommand(command);
}

void ChatWindow::requestChatUsers(const QString& chatName)
{
    QString command = QString("getChatUsers %1").arg(chatName);
    sendCommand(command);
}

void ChatWindow::onConnected()
{
    addMessage("System", "Connected to server", true);
}

void ChatWindow::onDisconnected()
{
    addMessage("System", "Disconnected from server", true);
}

void ChatWindow::onResponseReceived(const QString& response)
{
    if (response.startsWith("CHAT_HISTORY:")) {
        processChatHistoryResponse(response);
        return;
    }
    
    if (response.startsWith("CHAT_USERS:")) {
        processChatUsersResponse(response);
        return;
    }
    
    if (response.startsWith("ALL_USERS:")) {
        processAllUsersResponse(response);
        return;
    }
    
    if (response.startsWith("CHAT_MESSAGE:")) {
        QStringList parts = response.split(":", Qt::KeepEmptyParts);
        if (parts.size() >= 4) {
            QString chatName = parts[1];
            QString fromUser = parts[2];
            
            QString message;
            for (int i = 3; i < parts.size(); i++) {
                if (i > 3) message += ":";
                message += parts[i];
            }
            
            if (chatName == currentChat_) {
                addMessage(fromUser, message);
            } else {
                addMessage("System", "New message from " + fromUser + " in chat '" + chatName + "'", true);
            }
            return;
        }
    }
    
    if (response.startsWith("Message sent")) {}
    else if (response.startsWith("Chat created")) {
        addMessage("System", response, true);
        refreshChats();
    }
    else if (response.startsWith("User joined chat")) {
        addMessage("System", response, true);
    }
    else if (response.startsWith("User left chat")) {
        addMessage("System", response, true);
    }
    else if (response.startsWith("Error:")) {
        addMessage("System", response, true);
        
        if (response.contains("not found") || response.contains("does not exist")) {
            QString currentChat = chatList->currentItem() ? chatList->currentItem()->text() : "";
            if (!currentChat.isEmpty()) {
                addMessage("System", "Your message could not be delivered. Please check that the chat exists.", true);
            }
        }
    }
    else {
        addMessage("System", "Server response: " + response, true);
    }
}

void ChatWindow::processChatHistoryResponse(const QString& response)
{
    QStringList lines = response.split("\n", Qt::SkipEmptyParts);
    
    QStringList header = lines[0].split(":", Qt::KeepEmptyParts);
    if (header.size() < 3) return;
    
    QString chatName = header[1];
    int messageCount = header[2].toInt();
    
    addMessage("System", QString("Loading history for chat \"%1\" (%2 messages)").arg(chatName).arg(messageCount), true);
    
    QDateTime baseTime = QDateTime::currentDateTime().addSecs(-messageCount * 60);
    
    for (int i = 1; i < lines.size(); ++i) {
        QStringList msgParts = lines[i].split(":", Qt::KeepEmptyParts);
        if (msgParts.size() >= 2) {
            QString sender = msgParts[0];
            
            QString content;
            for (int j = 1; j < msgParts.size(); ++j) {
                if (j > 1) content += ":";
                content += msgParts[j];
            }
            
            QDateTime messageTime = baseTime.addSecs(i * 60);
            chatDisplay->addMessage(sender, content, messageTime, false);
        }
    }
    
    addMessage("System", "--- End of history ---", true);
}

void ChatWindow::processChatUsersResponse(const QString& response)
{
    QStringList parts = response.split(":", Qt::KeepEmptyParts);
    if (parts.size() < 3) return;
    
    QString chatName = parts[1];
    int userCount = parts[2].toInt();
    
    if (chatName != currentChat_) return;
    
    userList->clear();
    
    for (int i = 3; i < parts.size(); ++i) {
        QString username = parts[i];
        QListWidgetItem* item = new QListWidgetItem(username);
        
        if (username == username_) {
            item->setText(username + " (You)");
            item->setForeground(Qt::blue);
        }
        
        userList->addItem(item);
    }
}

void ChatWindow::processAllUsersResponse(const QString& response)
{
    QStringList parts = response.split(":", Qt::KeepEmptyParts);
    if (parts.size() < 2) return;
    
    int userCount = parts[1].toInt();
    
    statusBar->showMessage(QString("%1 users online").arg(userCount));
}

void ChatWindow::onNetworkError(const QString& errorMessage)
{
    addMessage("System", "Socket error: " + errorMessage, true);
}

void ChatWindow::onRefreshTimer()
{
    if (!networkWorker_->isConnected()) {
        connectToServer();
    }
    
    refreshUsers();
    
    if (!currentChat_.isEmpty()) {
        requestChatUsers(currentChat_);
    }
}