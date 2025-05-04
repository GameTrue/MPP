#include <QtTest>
#include <QSignalSpy>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include "../LoginDialog.h"
#include "../NetworkWorker.h"

// Mock class for NetworkWorker to avoid actual network operations
class MockNetworkWorker : public NetworkWorker {
    Q_OBJECT
public:
    explicit MockNetworkWorker(QObject* parent = nullptr) : NetworkWorker(parent) {}
    
    // Make sure we don't use 'override' since we're not overriding these methods
    bool isConnected() const { return mockConnected; }
    
public slots:
    void connectToServer(const QString&, quint16) {
        mockConnected = true;
        emit connected();
    }
    
    void sendCommand(const QString& command) {
        lastCommand = command;
        
        if (command.startsWith("login")) {
            if (command.contains("successful")) {
                emit responseReceived("Token: test_token");
            } else {
                emit responseReceived("Login failed: incorrect credentials");
            }
        } else if (command.startsWith("register")) {
            if (command.contains("newuser")) {
                emit responseReceived("User registered successfully");
            } else {
                emit responseReceived("User already exists");
            }
        }
    }
    
public:
    bool mockConnected = false;
    QString lastCommand;
};

class LoginDialogTest : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testUIElements();
    void testLoginButtonClick();
    void testRegisterButtonClick();
    void testElementVisibility();
    void testLoginButtonSignals();
    void testRegisterButtonSignals();
    
private:
    QPushButton* findLoginButton(LoginDialog* dialog);
    QPushButton* findRegisterButton(LoginDialog* dialog);
    QLineEdit* findUsernameEdit(LoginDialog* dialog);
    QLineEdit* findPasswordEdit(LoginDialog* dialog);
};

void LoginDialogTest::initTestCase()
{
    Q_INIT_RESOURCE(client_resources);
}

void LoginDialogTest::cleanupTestCase()
{
}

void LoginDialogTest::testUIElements()
{
    LoginDialog dialog;
    
    QLineEdit* usernameEdit = findUsernameEdit(&dialog);
    QLineEdit* passwordEdit = findPasswordEdit(&dialog);
    QPushButton* loginButton = findLoginButton(&dialog);
    QPushButton* registerButton = findRegisterButton(&dialog);
    
    QVERIFY(usernameEdit != nullptr);
    QVERIFY(passwordEdit != nullptr);
    QVERIFY(loginButton != nullptr);
    QVERIFY(registerButton != nullptr);
    
    QCOMPARE(passwordEdit->echoMode(), QLineEdit::Password);
    
    QCOMPARE(loginButton->text(), QString("Login"));
    QCOMPARE(registerButton->text(), QString("Register"));
}

void LoginDialogTest::testLoginButtonClick()
{
    LoginDialog dialog;
    
    QLineEdit* usernameEdit = findUsernameEdit(&dialog);
    QLineEdit* passwordEdit = findPasswordEdit(&dialog);
    QPushButton* loginButton = findLoginButton(&dialog);
    
    QTest::keyClicks(static_cast<QWidget*>(usernameEdit), "successful_user");
    QTest::keyClicks(static_cast<QWidget*>(passwordEdit), "password123");
    
    QTest::mouseClick(static_cast<QWidget*>(loginButton), Qt::LeftButton);
    
}

void LoginDialogTest::testRegisterButtonClick()
{
    LoginDialog dialog;
    
    // Access UI elements
    QLineEdit* usernameEdit = findUsernameEdit(&dialog);
    QLineEdit* passwordEdit = findPasswordEdit(&dialog);
    QPushButton* registerButton = findRegisterButton(&dialog);
    
    QTest::keyClicks(static_cast<QWidget*>(usernameEdit), "newuser");
    QTest::keyClicks(static_cast<QWidget*>(passwordEdit), "password123");
    
    QTest::mouseClick(static_cast<QWidget*>(registerButton), Qt::LeftButton);
}

void LoginDialogTest::testElementVisibility()
{
    LoginDialog dialog;
    
    dialog.show();
    QTest::qWaitForWindowExposed(&dialog);
    
    QLineEdit* usernameEdit = findUsernameEdit(&dialog);
    QLineEdit* passwordEdit = findPasswordEdit(&dialog);
    QPushButton* loginButton = findLoginButton(&dialog);
    QPushButton* registerButton = findRegisterButton(&dialog);
    
    QVERIFY(usernameEdit->isVisible());
    QVERIFY(passwordEdit->isVisible());
    QVERIFY(loginButton->isVisible());
    QVERIFY(registerButton->isVisible());
    
    QVERIFY(usernameEdit->isEnabled());
    QVERIFY(passwordEdit->isEnabled());
    QVERIFY(loginButton->isEnabled());
    QVERIFY(registerButton->isEnabled());
}

void LoginDialogTest::testLoginButtonSignals()
{
    LoginDialog dialog;
    
    // Show the dialog to make widgets visible
    dialog.show();
    QTest::qWaitForWindowExposed(&dialog);
    
    QLineEdit* usernameEdit = findUsernameEdit(&dialog);
    QLineEdit* passwordEdit = findPasswordEdit(&dialog);
    QPushButton* loginButton = findLoginButton(&dialog);
    
    QSignalSpy loginClickedSpy(loginButton, &QPushButton::clicked);
    
    usernameEdit->setText("testuser");
    passwordEdit->setText("password");
    
    QApplication::processEvents();
    
    QTest::mouseClick(loginButton, Qt::LeftButton);
    QCOMPARE(loginClickedSpy.count(), 1);
}

void LoginDialogTest::testRegisterButtonSignals()
{
    LoginDialog dialog;
    
    dialog.show();
    QTest::qWaitForWindowExposed(&dialog);
    
    QLineEdit* usernameEdit = findUsernameEdit(&dialog);
    QLineEdit* passwordEdit = findPasswordEdit(&dialog);
    QPushButton* registerButton = findRegisterButton(&dialog);
    
    QSignalSpy registerClickedSpy(registerButton, &QPushButton::clicked);
    
    usernameEdit->setText("testuser");
    passwordEdit->setText("password");
    
    QApplication::processEvents();
    
    QTest::mouseClick(registerButton, Qt::LeftButton);
    QCOMPARE(registerClickedSpy.count(), 1);
}

// Helper methods to find UI elements
QPushButton* LoginDialogTest::findLoginButton(LoginDialog* dialog)
{
    return dialog->findChild<QPushButton*>("loginButton");
}

QPushButton* LoginDialogTest::findRegisterButton(LoginDialog* dialog)
{
    return dialog->findChild<QPushButton*>("registerButton");
}

QLineEdit* LoginDialogTest::findUsernameEdit(LoginDialog* dialog)
{
    return dialog->findChild<QLineEdit*>("usernameEdit");
}

QLineEdit* LoginDialogTest::findPasswordEdit(LoginDialog* dialog)
{
    return dialog->findChild<QLineEdit*>("passwordEdit");
}

QTEST_MAIN(LoginDialogTest)
#include "LoginDialogTest.moc"
