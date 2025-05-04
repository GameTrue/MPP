#include "LoginDialog.h"
#include "NetworkWorker.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QFont>
#include <QThread>

// Server connection parameters
const QString LoginDialog::SERVER_ADDRESS = "127.0.0.1";
const quint16 LoginDialog::SERVER_PORT = 12345;

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi();
    
    networkThread_ = new QThread(this);
    networkWorker_ = new NetworkWorker();
    networkWorker_->moveToThread(networkThread_);
    
    connect(networkWorker_, &NetworkWorker::connected, this, &LoginDialog::onConnected);
    connect(networkWorker_, &NetworkWorker::disconnected, this, &LoginDialog::onDisconnected);
    connect(networkWorker_, &NetworkWorker::responseReceived, this, &LoginDialog::onResponseReceived);
    connect(networkWorker_, &NetworkWorker::errorOccurred, this, &LoginDialog::onNetworkError);
    
    connect(this, &LoginDialog::destroyed, networkWorker_, &NetworkWorker::deleteLater);
    connect(networkThread_, &QThread::finished, networkWorker_, &NetworkWorker::deleteLater);
    
    networkThread_->start();
    
    setObjectName("LoginDialog");
    
    setWindowTitle("Chat Login");
    setModal(true);
    
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
    
    statusLabel->setText("Please login or register");
    statusLabel->setStyleSheet("color: gray;");
}

LoginDialog::~LoginDialog()
{
    networkThread_->quit();
    networkThread_->wait();
}

void LoginDialog::setupUi()
{
    resize(400, 250);
    
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);
    
    formLayout = new QFormLayout();
    formLayout->setSpacing(10);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    
    titleLabel = new QLabel("Chat Messenger Login");
    titleLabel->setObjectName("titleLabel");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    
    usernameLabel = new QLabel("Username:");
    usernameLabel->setObjectName("usernameLabel");
    
    passwordLabel = new QLabel("Password:");
    passwordLabel->setObjectName("passwordLabel");
    
    usernameEdit = new QLineEdit();
    usernameEdit->setObjectName("usernameEdit");
    
    passwordEdit = new QLineEdit();
    passwordEdit->setObjectName("passwordEdit");
    passwordEdit->setEchoMode(QLineEdit::Password);
    
    statusLabel = new QLabel("Please enter your credentials");
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);
    
    loginButton = new QPushButton("Login");
    loginButton->setObjectName("loginButton");
    loginButton->setDefault(true);
    
    registerButton = new QPushButton("Register");
    registerButton->setObjectName("registerButton");
    
    // form layout
    formLayout->addRow(usernameLabel, usernameEdit);
    formLayout->addRow(passwordLabel, passwordEdit);
    
    // button layout
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(loginButton);
    
    // spacers 
    QSpacerItem* spacer1 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* spacer2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    
    // main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addItem(spacer1);
    mainLayout->addLayout(formLayout);
    mainLayout->addItem(spacer2);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void LoginDialog::connectToServer()
{
    // statusLabel->setText("Connecting to server...");
    statusLabel->setStyleSheet("color: blue;");
    
    QMetaObject::invokeMethod(networkWorker_, "connectToServer", 
                              Q_ARG(QString, SERVER_ADDRESS),
                              Q_ARG(quint16, SERVER_PORT));
}

void LoginDialog::sendCommand(const QString& command)
{
    QMetaObject::invokeMethod(networkWorker_, "sendCommand", 
                              Q_ARG(QString, command));
}

void LoginDialog::onLoginClicked()
{
    username_ = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    
    if (username_.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Username and password cannot be empty");
        return;
    }
    
    QString command = QString("login %1 %2").arg(username_, password);
    
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    statusLabel->setText("Logging in...");
    statusLabel->setStyleSheet("color: blue;");
    
    connectToServer();
    sendCommand(command);
}

void LoginDialog::onRegisterClicked()
{
    username_ = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    
    if (username_.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Username and password cannot be empty");
        return;
    }
    
    if (username_.length() < 3) {
        QMessageBox::warning(this, "Input Error", "Username must be at least 3 characters");
        return;
    }
    
    if (password.length() < 4) {
        QMessageBox::warning(this, "Input Error", "Password must be at least 4 characters");
        return;
    }
    
    QString command = QString("register %1 %2").arg(username_, password);
    
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    statusLabel->setText("Registering...");
    statusLabel->setStyleSheet("color: blue;");
    
    connectToServer();
    sendCommand(command);
}

void LoginDialog::onConnected()
{
    statusLabel->setText("Connected to server");
    statusLabel->setStyleSheet("color: green;");
}

void LoginDialog::onDisconnected()
{
    statusLabel->setText("Disconnected from server");
    statusLabel->setStyleSheet("color: gray;");
    loginButton->setEnabled(true);
    registerButton->setEnabled(true);
}

void LoginDialog::onResponseReceived(const QString& response)
{
    loginButton->setEnabled(true);
    registerButton->setEnabled(true);
    
    if (response.startsWith("User registered")) {
        statusLabel->setText("Registration successful! You can now login.");
        statusLabel->setStyleSheet("color: green;");
    } 
    else if (response.startsWith("User already exists")) {
        statusLabel->setText("Username already exists!");
        statusLabel->setStyleSheet("color: red;");
    }
    else if (response.startsWith("Token:")) {
        token_ = response.mid(7).trimmed();
        statusLabel->setText("Login successful!");
        statusLabel->setStyleSheet("color: green;");
        accept();
    }
    else if (response.startsWith("Login failed")) {
        statusLabel->setText("Login failed: Invalid credentials");
        statusLabel->setStyleSheet("color: red;");
    }
    else {
        statusLabel->setText("Unexpected response: " + response);
        statusLabel->setStyleSheet("color: red;");
    }
}

void LoginDialog::onNetworkError(const QString& errorMessage)
{
    statusLabel->setText("Socket error: " + errorMessage);
    statusLabel->setStyleSheet("color: red;");
    loginButton->setEnabled(true);
    registerButton->setEnabled(true);
}