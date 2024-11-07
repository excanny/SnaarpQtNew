/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *loginLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QSpacerItem *verticalSpacer_3;
    QLabel *emailLabel;
    QLineEdit *emailField;
    QLabel *passwordLabel;
    QLineEdit *passwordField;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *rememberMeCheckBox;
    QPushButton *forgotPasswordButton;
    QSpacerItem *verticalSpacer_2;
    QPushButton *loginButton;
    QLabel *signupLabel;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow {\n"
"        background-color: #f0f2f5;\n"
"    }\n"
"    QLabel {\n"
"        color: #1a1a1a;\n"
"        font-size: 14px;\n"
"        font-weight: bold;\n"
"    }\n"
"    QLineEdit {\n"
"        padding: 8px;\n"
"        border: 2px solid #e0e0e0;\n"
"        border-radius: 5px;\n"
"        background-color: white;\n"
"        font-size: 14px;\n"
"        min-height: 25px;\n"
"    }\n"
"    QLineEdit:focus {\n"
"        border: 2px solid #2196F3;\n"
"    }\n"
"    QPushButton {\n"
"        background-color: #2196F3;\n"
"        color: white;\n"
"        border: none;\n"
"        border-radius: 5px;\n"
"        padding: 10px;\n"
"        font-size: 14px;\n"
"        font-weight: bold;\n"
"        min-height: 35px;\n"
"    }\n"
"    QPushButton:hover {\n"
"        background-color: #1976D2;\n"
"    }\n"
"    QPushButton#forgotPasswordButton {\n"
"        background-color: transparent;\n"
"        color: #2196F3;\n"
"        text-align: left;\n"
"        padding: 0px;\n"
"    }\n"
"    QPu"
                        "shButton#forgotPasswordButton:hover {\n"
"        color: #1976D2;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        loginLayout = new QVBoxLayout();
        loginLayout->setSpacing(15);
        loginLayout->setObjectName("loginLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setAlignment(Qt::AlignCenter);

        loginLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(centralwidget);
        subtitleLabel->setObjectName("subtitleLabel");
        QFont font1;
        font1.setPointSize(10);
        subtitleLabel->setFont(font1);
        subtitleLabel->setStyleSheet(QString::fromUtf8("color: #666666;"));
        subtitleLabel->setAlignment(Qt::AlignCenter);

        loginLayout->addWidget(subtitleLabel);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        loginLayout->addItem(verticalSpacer_3);

        emailLabel = new QLabel(centralwidget);
        emailLabel->setObjectName("emailLabel");

        loginLayout->addWidget(emailLabel);

        emailField = new QLineEdit(centralwidget);
        emailField->setObjectName("emailField");
        emailField->setMinimumSize(QSize(300, 0));

        loginLayout->addWidget(emailField);

        passwordLabel = new QLabel(centralwidget);
        passwordLabel->setObjectName("passwordLabel");

        loginLayout->addWidget(passwordLabel);

        passwordField = new QLineEdit(centralwidget);
        passwordField->setObjectName("passwordField");
        passwordField->setEchoMode(QLineEdit::Password);

        loginLayout->addWidget(passwordField);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        rememberMeCheckBox = new QCheckBox(centralwidget);
        rememberMeCheckBox->setObjectName("rememberMeCheckBox");

        horizontalLayout_2->addWidget(rememberMeCheckBox);

        forgotPasswordButton = new QPushButton(centralwidget);
        forgotPasswordButton->setObjectName("forgotPasswordButton");
        forgotPasswordButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        horizontalLayout_2->addWidget(forgotPasswordButton);


        loginLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        loginLayout->addItem(verticalSpacer_2);

        loginButton = new QPushButton(centralwidget);
        loginButton->setObjectName("loginButton");
        loginButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        loginLayout->addWidget(loginButton);

        signupLabel = new QLabel(centralwidget);
        signupLabel->setObjectName("signupLabel");
        signupLabel->setStyleSheet(QString::fromUtf8("color: #666666;"));
        signupLabel->setAlignment(Qt::AlignCenter);
        signupLabel->setOpenExternalLinks(true);

        loginLayout->addWidget(signupLabel);


        horizontalLayout->addLayout(loginLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Snaarp", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Snaarp Drive", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("MainWindow", "Please enter your credentials to continue", nullptr));
        emailLabel->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        emailField->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter your email", nullptr));
        passwordLabel->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        passwordField->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter your password", nullptr));
        rememberMeCheckBox->setText(QCoreApplication::translate("MainWindow", "Remember me", nullptr));
        forgotPasswordButton->setText(QCoreApplication::translate("MainWindow", "Forgot Password?", nullptr));
        loginButton->setText(QCoreApplication::translate("MainWindow", "Log In", nullptr));
        signupLabel->setText(QCoreApplication::translate("MainWindow", "Don't have an account? <a href=\"#\" style=\"color: #2196F3;\">Sign up</a>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
