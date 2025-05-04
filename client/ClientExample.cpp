#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "LoginDialog.h"
#include "ChatWindow.h"

using namespace boost::asio;
using ip::tcp;

// Добавим константы для IP и порта сервера
const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 12345;

// Function to load and apply stylesheet
void loadStyleSheet(QApplication& app)
{
    QFile styleFile(":/styles/style.qss");
    if (!styleFile.open(QFile::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Warning", "Failed to load stylesheet.");
        return;
    }
    
    QTextStream styleStream(&styleFile);
    QString style = styleStream.readAll();
    app.setStyleSheet(style);
    
    styleFile.close();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Load application stylesheet
    loadStyleSheet(app);
    
    app.setApplicationName("Boost Chat Client");
    app.setStyle("Fusion"); 
    
    LoginDialog loginDialog;
    
    if (loginDialog.exec() == QDialog::Accepted) {
        QString username = loginDialog.getUsername();
        QString token = loginDialog.getToken();
        
        ChatWindow chatWindow(username, token);
        chatWindow.show();
        
        return app.exec();
    }
    
    return 0;
}
