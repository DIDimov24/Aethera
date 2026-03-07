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
    connect(ui->buttonBackHome, &QPushButton::clicked, this, &Login::onBackHomeClicked);
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

    if (!Database::instance().userExists(username)) {
        ui->labelError->setText("Username does not exist. Try creating an account.");
        return;
    }

    if (Database::instance().validateUser(username, password)) {
        ui->labelError->setText("");

        User user;

        if (!Database::instance().getUser(username, user)) {
            ui->labelError->setText("Could not load user profile.");
            return;
        }

        UserSession::instance().setUser(user);

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

void Login::onBackHomeClicked() {
    Home *homePage = new Home();
    this->hide();
    homePage->show();
}
