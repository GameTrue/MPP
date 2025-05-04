#pragma once

#include <QDialog>
#include <memory>


class QVBoxLayout;
class QFormLayout;
class QHBoxLayout;
class QLineEdit;
class QLabel;
class QPushButton;
class QThread;

class NetworkWorker;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString getUsername() const { return username_; }
    QString getToken() const { return token_; }

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    
    void onConnected();
    void onDisconnected();
    void onResponseReceived(const QString& response);
    void onNetworkError(const QString& errorMessage);

private:
    void setupUi();
    
    void connectToServer();
    void sendCommand(const QString& command);
    
    // UI elements
    QLabel* titleLabel;
    QLabel* usernameLabel;
    QLabel* passwordLabel;
    QLabel* statusLabel;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    QPushButton* registerButton;
    
    QVBoxLayout* mainLayout;
    QFormLayout* formLayout;
    QHBoxLayout* buttonLayout;
    
    QThread* networkThread_;
    NetworkWorker* networkWorker_;
    
    QString username_;
    QString token_;
    
    static const QString SERVER_ADDRESS;
    static const quint16 SERVER_PORT;
};