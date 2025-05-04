/********************************************************************************
** Form generated from reading UI file 'ChatWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.16
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWindow
{
public:
    QAction *actionDisconnect;
    QAction *actionExit;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *leftPanel;
    QLabel *label;
    QListWidget *chatList;
    QHBoxLayout *chatButtonsLayout;
    QPushButton *createChatButton;
    QPushButton *joinChatButton;
    QPushButton *leaveChatButton;
    QFrame *line;
    QLabel *label_2;
    QListWidget *userList;
    QWidget *layoutWidget_2;
    QVBoxLayout *rightPanel;
    QPlainTextEdit *chatDisplay;
    QHBoxLayout *messageLayout;
    QLineEdit *messageEdit;
    QPushButton *sendButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ChatWindow)
    {
        if (ChatWindow->objectName().isEmpty())
            ChatWindow->setObjectName(QString::fromUtf8("ChatWindow"));
        ChatWindow->resize(800, 600);
        actionDisconnect = new QAction(ChatWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        actionExit = new QAction(ChatWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(ChatWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        leftPanel = new QVBoxLayout(layoutWidget);
        leftPanel->setObjectName(QString::fromUtf8("leftPanel"));
        leftPanel->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        leftPanel->addWidget(label);

        chatList = new QListWidget(layoutWidget);
        chatList->setObjectName(QString::fromUtf8("chatList"));

        leftPanel->addWidget(chatList);

        chatButtonsLayout = new QHBoxLayout();
        chatButtonsLayout->setObjectName(QString::fromUtf8("chatButtonsLayout"));
        createChatButton = new QPushButton(layoutWidget);
        createChatButton->setObjectName(QString::fromUtf8("createChatButton"));

        chatButtonsLayout->addWidget(createChatButton);

        joinChatButton = new QPushButton(layoutWidget);
        joinChatButton->setObjectName(QString::fromUtf8("joinChatButton"));

        chatButtonsLayout->addWidget(joinChatButton);

        leaveChatButton = new QPushButton(layoutWidget);
        leaveChatButton->setObjectName(QString::fromUtf8("leaveChatButton"));

        chatButtonsLayout->addWidget(leaveChatButton);


        leftPanel->addLayout(chatButtonsLayout);

        line = new QFrame(layoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        leftPanel->addWidget(line);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        leftPanel->addWidget(label_2);

        userList = new QListWidget(layoutWidget);
        userList->setObjectName(QString::fromUtf8("userList"));

        leftPanel->addWidget(userList);

        splitter->addWidget(layoutWidget);
        layoutWidget_2 = new QWidget(splitter);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        rightPanel = new QVBoxLayout(layoutWidget_2);
        rightPanel->setObjectName(QString::fromUtf8("rightPanel"));
        rightPanel->setContentsMargins(0, 0, 0, 0);
        chatDisplay = new QPlainTextEdit(layoutWidget_2);
        chatDisplay->setObjectName(QString::fromUtf8("chatDisplay"));
        chatDisplay->setReadOnly(true);

        rightPanel->addWidget(chatDisplay);

        messageLayout = new QHBoxLayout();
        messageLayout->setObjectName(QString::fromUtf8("messageLayout"));
        messageEdit = new QLineEdit(layoutWidget_2);
        messageEdit->setObjectName(QString::fromUtf8("messageEdit"));

        messageLayout->addWidget(messageEdit);

        sendButton = new QPushButton(layoutWidget_2);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));

        messageLayout->addWidget(sendButton);


        rightPanel->addLayout(messageLayout);

        splitter->addWidget(layoutWidget_2);

        horizontalLayout_2->addWidget(splitter);

        ChatWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChatWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        ChatWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ChatWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ChatWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionDisconnect);
        menuFile->addAction(actionExit);

        retranslateUi(ChatWindow);

        QMetaObject::connectSlotsByName(ChatWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChatWindow)
    {
        ChatWindow->setWindowTitle(QCoreApplication::translate("ChatWindow", "Chat", nullptr));
        actionDisconnect->setText(QCoreApplication::translate("ChatWindow", "Disconnect", nullptr));
        actionExit->setText(QCoreApplication::translate("ChatWindow", "Exit", nullptr));
        label->setText(QCoreApplication::translate("ChatWindow", "Chats", nullptr));
        createChatButton->setText(QCoreApplication::translate("ChatWindow", "Create", nullptr));
        joinChatButton->setText(QCoreApplication::translate("ChatWindow", "Join", nullptr));
        leaveChatButton->setText(QCoreApplication::translate("ChatWindow", "Leave", nullptr));
        label_2->setText(QCoreApplication::translate("ChatWindow", "Users", nullptr));
        messageEdit->setPlaceholderText(QCoreApplication::translate("ChatWindow", "Type your message here...", nullptr));
        sendButton->setText(QCoreApplication::translate("ChatWindow", "Send", nullptr));
        menuFile->setTitle(QCoreApplication::translate("ChatWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatWindow: public Ui_ChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
