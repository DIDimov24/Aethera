#include "register.h"
#include "ui_register.h"
#include "database.h"
#include "login.h"
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

static QString validatePassword(const QString &password) {
    if (password.length() < 8)
        return "Password must be at least 8 characters.";

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (const QChar &c : password) {
        if (c.isUpper())  hasUpper = true;
        if (c.isLower())  hasLower = true;
        if (c.isDigit())  hasDigit = true;
        if (!c.isLetterOrNumber()) hasSpecial = true;
    }

    if (!hasUpper)   return "Password must contain at least one uppercase letter.";
    if (!hasLower)   return "Password must contain at least one lowercase letter.";
    if (!hasDigit)   return "Password must contain at least one number.";
    if (!hasSpecial) return "Password must contain at least one special character.";

    return "";
}

void Register::onCreateClicked() {
    QString username = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();
    QString confirm  = ui->inputConfirm->text().trimmed();
    QString grade    = ui->comboGrade->currentText().trimmed();

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

    QString passwordError = validatePassword(password);
    if (!passwordError.isEmpty()) {
        ui->labelError->setText(passwordError);
        return;
    }

    if (Database::instance().userExists(username)) {
        ui->labelError->setText("Username already taken. Please choose another.");
        ui->inputUsername->setFocus();
        return;
    }

    if (password != confirm) {
        ui->labelError->setText("Passwords do not match.");
        ui->inputConfirm->setFocus();
        return;
    }

    if (!Database::instance().registerUser(username, password, grade)) {
        ui->labelError->setText("Could not create account. Try a different username.");
        return;
    }

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