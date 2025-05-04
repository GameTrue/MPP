/********************************************************************************
** Form generated from reading UI file 'LoginDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.16
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QFormLayout *formLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QSpacerItem *verticalSpacer_2;
    QLabel *statusLabel;
    QHBoxLayout *horizontalLayout;
    QPushButton *registerButton;
    QPushButton *loginButton;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QString::fromUtf8("LoginDialog"));
        LoginDialog->resize(400, 250);
        verticalLayout = new QVBoxLayout(LoginDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(LoginDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        usernameLabel = new QLabel(LoginDialog);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, usernameLabel);

        usernameEdit = new QLineEdit(LoginDialog);
        usernameEdit->setObjectName(QString::fromUtf8("usernameEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, usernameEdit);

        passwordLabel = new QLabel(LoginDialog);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, passwordLabel);

        passwordEdit = new QLineEdit(LoginDialog);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, passwordEdit);


        verticalLayout->addLayout(formLayout);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        statusLabel = new QLabel(LoginDialog);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(statusLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        registerButton = new QPushButton(LoginDialog);
        registerButton->setObjectName(QString::fromUtf8("registerButton"));

        horizontalLayout->addWidget(registerButton);

        loginButton = new QPushButton(LoginDialog);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));

        horizontalLayout->addWidget(loginButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(LoginDialog);

        loginButton->setDefault(true);


        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Login", nullptr));
        label->setText(QCoreApplication::translate("LoginDialog", "Chat Messenger Login", nullptr));
        usernameLabel->setText(QCoreApplication::translate("LoginDialog", "Username:", nullptr));
        passwordLabel->setText(QCoreApplication::translate("LoginDialog", "Password:", nullptr));
        statusLabel->setText(QCoreApplication::translate("LoginDialog", "Please enter your credentials", nullptr));
        registerButton->setText(QCoreApplication::translate("LoginDialog", "Register", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginDialog", "Login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
