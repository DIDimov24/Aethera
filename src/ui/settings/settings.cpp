#include "settings.h"
#include "ui_settings.h"
#include "usersession.h"
#include "database.h"
#include <QMessageBox>
#include <QPushButton>

Settings::Settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings) {
    ui->setupUi(this);

    connect(ui->buttonSaveUsername,  &QPushButton::clicked, this, &Settings::onSaveUsername);
    connect(ui->buttonSavePassword,  &QPushButton::clicked, this, &Settings::onSavePassword);
    connect(ui->buttonSaveGrade,     &QPushButton::clicked, this, &Settings::onSaveGrade);
    connect(ui->buttonSaveBio,       &QPushButton::clicked, this, &Settings::onSaveBio);
    connect(ui->buttonDeleteAccount, &QPushButton::clicked, this, &Settings::onDeleteAccountClicked);

    connect(ui->buttonViewTerms, &QPushButton::clicked, this, [this]() {
        ui->stackedWidgetSettings->setCurrentIndex(1);
    });
    connect(ui->buttonViewPrivacy, &QPushButton::clicked, this, [this]() {
        ui->stackedWidgetSettings->setCurrentIndex(2);
    });
    connect(ui->buttonBackFromTerms, &QPushButton::clicked, this, [this]() {
        ui->stackedWidgetSettings->setCurrentIndex(0);
    });
    connect(ui->buttonBackFromPrivacy, &QPushButton::clicked, this, [this]() {
        ui->stackedWidgetSettings->setCurrentIndex(0);
    });
}

Settings::~Settings() {
    delete ui;
}

void Settings::refresh() {
    ui->stackedWidgetSettings->setCurrentIndex(0);
    if (UserSession::instance().isLoggedIn()) {
        ui->textSettingsBio->setPlainText(UserSession::instance().getBio());
    }
}

void Settings::onSaveUsername() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->labelSettingsUsernameStatus->setText("Please log in first.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }

    QString newUsername = ui->inputSettingsUsername->text().trimmed();
    if (newUsername.isEmpty()) {
        ui->labelSettingsUsernameStatus->setText("Username cannot be empty.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }
    if (newUsername.length() < 3) {
        ui->labelSettingsUsernameStatus->setText("Username must be at least 3 characters.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }

    QString oldUsername = UserSession::instance().getUsername();
    if (newUsername == oldUsername) {
        ui->labelSettingsUsernameStatus->setText("New username is the same as current.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }

    if (!Database::instance().updateUsername(oldUsername, newUsername)) {
        ui->labelSettingsUsernameStatus->setText("Username already taken.");
        ui->labelSettingsUsernameStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsUsernameStatus->setVisible(true);
        return;
    }

    UserSession::instance().setUsername(newUsername);
    ui->inputSettingsUsername->clear();
    ui->labelSettingsUsernameStatus->setText("Username updated successfully!");
    ui->labelSettingsUsernameStatus->setStyleSheet("color: #2a6a20; font-size: 12px;");
    ui->labelSettingsUsernameStatus->setVisible(true);
    emit usernameChanged();
}

void Settings::onSavePassword() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->labelSettingsPasswordStatus->setText("Please log in first.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    QString current = ui->inputSettingsCurrentPassword->text().trimmed();
    QString newPass = ui->inputSettingsNewPassword->text().trimmed();
    QString confirm = ui->inputSettingsConfirmPassword->text().trimmed();

    if (current.isEmpty() || newPass.isEmpty() || confirm.isEmpty()) {
        ui->labelSettingsPasswordStatus->setText("Please fill in all fields.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    QString username = UserSession::instance().getUsername();
    if (!Database::instance().validateUser(username, current)) {
        ui->labelSettingsPasswordStatus->setText("Current password is incorrect.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    if (newPass.length() < 5) {
        ui->labelSettingsPasswordStatus->setText("Password must be at least 5 characters.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    bool hasUpper = false;
    bool hasDigit = false;
    for (int i = 0; i < newPass.length(); i++) {
        if (newPass[i].isUpper()) hasUpper = true;
        if (newPass[i].isDigit()) hasDigit = true;
    }
    if (!hasUpper) {
        ui->labelSettingsPasswordStatus->setText("Password must contain at least one uppercase letter.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }
    if (!hasDigit) {
        ui->labelSettingsPasswordStatus->setText("Password must contain at least one digit.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    if (newPass != confirm) {
        ui->labelSettingsPasswordStatus->setText("New passwords do not match.");
        ui->labelSettingsPasswordStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsPasswordStatus->setVisible(true);
        return;
    }

    Database::instance().updatePassword(username, newPass);
    ui->inputSettingsCurrentPassword->clear();
    ui->inputSettingsNewPassword->clear();
    ui->inputSettingsConfirmPassword->clear();
    ui->labelSettingsPasswordStatus->setText("Password updated successfully!");
    ui->labelSettingsPasswordStatus->setStyleSheet("color: #2a6a20; font-size: 12px;");
    ui->labelSettingsPasswordStatus->setVisible(true);
}

void Settings::onSaveGrade() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->labelSettingsGradeStatus->setText("Please log in first.");
        ui->labelSettingsGradeStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsGradeStatus->setVisible(true);
        return;
    }

    QString newGrade = ui->comboSettingsGrade->currentText().trimmed();
    QString currentGrade = UserSession::instance().getGrade();

    if (newGrade.isEmpty()) {
        ui->labelSettingsGradeStatus->setText("Please select your grade.");
        ui->labelSettingsGradeStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsGradeStatus->setVisible(true);
        return;
    }

    if (newGrade == currentGrade) {
        ui->labelSettingsGradeStatus->setText("New grade is the same as current.");
        ui->labelSettingsGradeStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsGradeStatus->setVisible(true);
        return;
    }

    Database::instance().updateGrade(UserSession::instance().getUsername(), newGrade);
    UserSession::instance().setGrade(newGrade);

    ui->labelSettingsGradeStatus->setText("Grade updated successfully!");
    ui->labelSettingsGradeStatus->setStyleSheet("color: #2a6a20; font-size: 12px;");
    ui->labelSettingsGradeStatus->setVisible(true);
    emit gradeChanged();
}

void Settings::onSaveBio() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->labelSettingsBioStatus->setText("Please log in first.");
        ui->labelSettingsBioStatus->setStyleSheet("color: #c0392b; font-size: 12px;");
        ui->labelSettingsBioStatus->setVisible(true);
        return;
    }

    QString bio = ui->textSettingsBio->toPlainText().trimmed();
    UserSession::instance().setBio(bio);
    Database::instance().updateBio(UserSession::instance().getUsername(), bio);

    ui->labelSettingsBioStatus->setText("Bio updated successfully!");
    ui->labelSettingsBioStatus->setStyleSheet("color: #2a6a20; font-size: 12px;");
    ui->labelSettingsBioStatus->setVisible(true);
    emit bioChanged();
}

void Settings::onDeleteAccountClicked() {
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
            background-color: #f4f5fa;
            color: #1a2440;
        }
        QMessageBox QLabel {
            color: #1a2440;
            font-size: 13px;
        }
        QPushButton {
            background-color: #ffffff;
            color: #1a2440;
            font-size: 13px;
            font-weight: 600;
            border: 1px solid #d4d8ea;
            border-radius: 6px;
            padding: 8px 20px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #eceef5;
            border-color: #b0bcd8;
        }
        QPushButton:focus {
            border-color: #c0392b;
        }
    )");

    if (msgBox.exec() != QMessageBox::Yes) return;

    QString username = UserSession::instance().getUsername();
    Database::instance().deleteUser(username);
    UserSession::instance().clear();

    emit accountDeleted();
}