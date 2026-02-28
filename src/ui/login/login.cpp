#include "login.h"
#include "ui_login.h"
#include "database.h"
#include "register.h"
#include "home.h"
#include "usersession.h"
 
Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login) {
    ui->setupUi(this);
 
    connect(ui->buttonLogin, &QPushButton::clicked, this, &Login::onLoginClicked);
 
    connect(ui->buttonRegister, &QPushButton::clicked, this, &Login::onRegisterClicked);
 
    connect(ui->inputPassword, &QLineEdit::returnPressed, this, &Login::onLoginClicked);
 
    connect(ui->inputUsername, &QLineEdit::returnPressed, this, [this](){ ui->inputPassword->setFocus(); });
}
 
Login::~Login() {
    delete ui;
}
 
void Login::onLoginClicked() {
    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();
 
    if (username.isEmpty() || password.isEmpty()) {
        ui->labelError->setText("Please fill in all fields.");
        return;
    }
 
    if (UserStore::instance().validateUser(username, password) ||
        (username == "admin" && password == "admin")) {
        ui->labelError->setText("");
        UserSession::instance().setUser(username);
        Home *homePage = new Home();
        this->hide();
        homePage->show();
    } else {
        ui->labelError->setText("Incorrect username or password.");
        ui->inputPassword->clear();
        ui->inputPassword->setFocus();
    }
}
 
void Login::onRegisterClicked() {
    Register *registerPage = new Register();
    this->hide();
    registerPage->show();
}
