#include "register.h"
#include "ui_register.h"
#include "database.h"
#include "../login/login.h"
#include "home.h"
 
Register::Register(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Register) {
    ui->setupUi(this);
 
    connect(ui->buttonCreate, &QPushButton::clicked, this, &Register::onCreateClicked);
    connect(ui->buttonBack, &QPushButton::clicked, this, &Register::onBackClicked);
    connect(ui->buttonBackHome, &QPushButton::clicked, this, &Register::onBackHomeClicked);
    connect(ui->inputUsername, &QLineEdit::returnPressed, this, [this]() { ui->inputPassword->setFocus(); });
    connect(ui->inputPassword, &QLineEdit::returnPressed, this, [this]() { ui->inputConfirm->setFocus(); });
    connect(ui->inputConfirm, &QLineEdit::returnPressed, this, [this]() { ui->comboGrade->setFocus(); });
}
 
Register::~Register() {
    delete ui;
}
 
void Register::onCreateClicked() {
    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();
    QString confirm = ui->inputConfirm->text().trimmed();
    QString grade = ui->comboGrade->currentText().trimmed();
 
    ui->labelError->setText("");
 
    if (username.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        ui->labelError->setText("Please fill in all fields.");
        return;
    }
 
    if (grade.isEmpty()) {
        ui->labelError->setText("Please select your grade.");
        return;
    }
 
    if (username.length() < 3) {
        ui->labelError->setText("Username must be at least 3 characters.");
        return;
    }
 
    if (UserStore::instance().userExists(username)) {
        ui->labelError->setText("Username already taken. Please choose another.");
        ui->inputUsername->setFocus();
        return;
    }
 
    if (password != confirm) {
        ui->labelError->setText("Passwords do not match.");
        ui->inputConfirm->setFocus();
        return;
    }
 
    UserStore::instance().registerUser(username, password, grade);
 
    Login *loginPage = new Login();
    this->hide();
    loginPage->show();
}
 
void Register::onBackClicked() {
    Login *loginPage = new Login();
    this->hide();
    loginPage->show();
}
 
void Register::onBackHomeClicked() {
    Home *homePage = new Home();
    this->hide();
    homePage->show();
}
 
