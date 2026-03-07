#include "home.h"
#include "ui_home.h"
#include "usersession.h"
#include "database.h"
#include "login.h"
#include <QMessageBox>
#include <QPushButton>

void Home::initSettingsPage() {
    connect(ui->buttonSaveUsername, &QPushButton::clicked, this, &Home::onSaveUsername);
    connect(ui->buttonSavePassword, &QPushButton::clicked, this, &Home::onSavePassword);
    connect(ui->buttonSaveBio, &QPushButton::clicked, this, &Home::onSaveBio);
    connect(ui->buttonDeleteAccount, &QPushButton::clicked, this, &Home::onDeleteAccountClicked);

    connect(ui->buttonViewTerms, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(4);
    });
    connect(ui->buttonViewPrivacy, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(5);
    });
    connect(ui->buttonBackFromTerms, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
    });
    connect(ui->buttonBackFromPrivacy, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
    });

    if (UserSession::instance().isLoggedIn()) {
        ui->textSettingsBio->setPlainText(UserSession::instance().getBio());
    }
}

void Home::onSaveUsername() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->labelSettingsUsernameStatus->setText("Please log in first.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }

    QString newUsername = ui->inputSettingsUsername->text().trimmed();
    if (newUsername.isEmpty()) {
        ui->labelSettingsUsernameStatus->setText("Username cannot be empty.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }
    if (newUsername.length() < 3) {
        ui->labelSettingsUsernameStatus->setText("Username must be at least 3 characters.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }

    QString oldUsername = UserSession::instance().getUsername();
    if (newUsername == oldUsername) {
        ui->labelSettingsUsernameStatus->setText("New username is the same as current.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }

    if (!Database::instance().updateUsername(oldUsername, newUsername)) {
        ui->labelSettingsUsernameStatus->setText("Username already taken.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }

    UserSession::instance().setUsername(newUsername);
    ui->inputSettingsUsername->clear();
    ui->labelSettingsUsernameStatus->setText("Username updated successfully!");
    ui->labelSettingsUsernameStatus->setStyleSheet("color: #00D4AA; font-size: 12px;");
    ui->labelSettingsUsernameStatus->setVisible(true);
}

void Home::onSavePassword() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->labelSettingsPasswordStatus->setText("Please log in first.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    QString current = ui->inputSettingsCurrentPassword->text().trimmed();
    QString newPass = ui->inputSettingsNewPassword->text().trimmed();
    QString confirm = ui->inputSettingsConfirmPassword->text().trimmed();

    if (current.isEmpty() || newPass.isEmpty() || confirm.isEmpty()) {
        ui->labelSettingsPasswordStatus->setText("Please fill in all fields.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    QString username = UserSession::instance().getUsername();
    if (!Database::instance().validateUser(username, current)) {
        ui->labelSettingsPasswordStatus->setText("Current password is incorrect.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    if (newPass != confirm) {
        ui->labelSettingsPasswordStatus->setText("New passwords do not match.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    Database::instance().updatePassword(username, newPass);
    ui->inputSettingsCurrentPassword->clear();
    ui->inputSettingsNewPassword->clear();
    ui->inputSettingsConfirmPassword->clear();
    ui->labelSettingsPasswordStatus->setText("Password updated successfully!");
    ui->labelSettingsPasswordStatus->setStyleSheet("color: #00D4AA; font-size: 12px;");
    ui->labelSettingsPasswordStatus->setVisible(true);
}

void Home::onSaveBio() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->labelSettingsBioStatus->setText("Please log in first.");
        ui->labelSettingsBioStatus->setStyleSheet("color: #FF6B6B; font-size: 12px;");
        ui->labelSettingsBioStatus->setVisible(true);
        return;
    }

    QString bio = ui->textSettingsBio->toPlainText().trimmed();
    UserSession::instance().setBio(bio);
    Database::instance().updateBio(UserSession::instance().getUsername(), bio);

    ui->labelSettingsBioStatus->setText("Bio updated successfully!");
    ui->labelSettingsBioStatus->setStyleSheet("color: #00D4AA; font-size: 12px;");
    ui->labelSettingsBioStatus->setVisible(true);
}

void Home::onDeleteAccountClicked() {
    if (!UserSession::instance().isLoggedIn()) return;

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Delete Account");
    msgBox.setText("Are you sure you want to delete your account?");
    msgBox.setInformativeText("This action is permanent and cannot be undone.");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet(R"(
        QMessageBox {
            background-color: #1a1212;
            color: #e0e0e4;
        }
        QMessageBox QLabel {
            color: #e0e0e4;
            font-size: 13px;
        }
        QPushButton {
            background-color: #222224;
            color: #e0e0e4;
            font-size: 13px;
            font-weight: 600;
            border: 1px solid #2a2a2d;
            border-radius: 6px;
            padding: 8px 20px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #2a2a2d;
            color: #ffffff;
            border-color: #3a3a3e;
        }
        QPushButton:focus {
            border-color: #8b2020;
        }
    )");

    if (msgBox.exec() != QMessageBox::Yes) return;

    QString username = UserSession::instance().getUsername();
    Database::instance().deleteUser(username);
    UserSession::instance().clear();

    Login *loginPage = new Login();
    this->hide();
    loginPage->show();
}
