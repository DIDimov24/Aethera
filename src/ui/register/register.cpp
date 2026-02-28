#include "register.h"
#include "ui_register.h"
#include "database.h"
#include "../login/login.h"
 
Register::Register(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Register) {
    ui->setupUi(this);
 
    connect(ui->buttonCreate, &QPushButton::clicked, this, &Register::onCreateClicked);
    connect(ui->buttonBack, &QPushButton::clicked, this, &Register::onBackClicked);
}
 
Register::~Register() {
    delete ui;
}
 
void Register::onCreateClicked() {
    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();
    QString confirm = ui->inputConfirm->text().trimmed();
 
    ui->labelError->setText("");
 
    if (username.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        ui->labelError->setText("Please fill in all fields.");
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
 
    UserStore::instance().registerUser(username, password);
 
    Login *loginPage = new Login();
    this->hide();
    loginPage->show();
}
 
void Register::onBackClicked() {
    Login *loginPage = new Login();
    this->hide();
    loginPage->show();
}
