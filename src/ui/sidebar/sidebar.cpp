#include "home.h"
#include "ui_home.h"
#include "login.h"
#include "usersession.h"
#include "style.h"
#include "utils.h"
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QPushButton>
#include <QRect>
#include <QSize>
#include <QEasingCurve>
#include <QMessageBox>
#include <QLabel>

void Home::onLogoutClicked() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Log Out");
    msgBox.setText("Are you sure you want to log out?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet(R"(
        QMessageBox { background-color: #f4f5fa; color: #1a2440; }
        QMessageBox QLabel { color: #1a2440; font-size: 13px; }
        QPushButton {
            background-color: #ffffff; color: #1a2440; font-size: 13px; font-weight: 600;
            border: 1px solid #d4d8ea; border-radius: 6px; padding: 8px 20px; min-width: 80px;
        }
        QPushButton:hover { background-color: #eceef5; border-color: #b0bcd8; }
    )");

    if (msgBox.exec() != QMessageBox::Yes) return;

    UserSession::instance().clear();
    Login *loginPage = new Login();
    this->hide();
    loginPage->show();
}

void Home::toggleSidebar(bool hidden) {
    sidebarExpanded = hidden ? false : !sidebarExpanded;

    const int expandedW  = 200;
    const int collapsedW = 52;
    const int totalH = 750;
    const int totalW = 1000;

    int targetSW = sidebarExpanded ? expandedW : collapsedW;
    int targetCW = totalW - targetSW;

    auto *sa = new QPropertyAnimation(ui->sidebar, "geometry", this);
    sa->setDuration(180);
    sa->setEasingCurve(QEasingCurve::InOutQuad);
    sa->setStartValue(ui->sidebar->geometry());
    sa->setEndValue(QRect(0, 0, targetSW, totalH));
    sa->start(QAbstractAnimation::DeleteWhenStopped);

    auto *ca = new QPropertyAnimation(ui->stackedWidget, "geometry", this);
    ca->setDuration(180);
    ca->setEasingCurve(QEasingCurve::InOutQuad);
    ca->setStartValue(ui->stackedWidget->geometry());
    ca->setEndValue(QRect(targetSW, 0, targetCW, totalH));
    ca->start(QAbstractAnimation::DeleteWhenStopped);

    repositionSidebarButtons();
    updateSidebarButtons();
}

void Home::repositionSidebarButtons() {
    const int btnW = sidebarExpanded ? 184 : 36;
    const int btnX = 8;
    const int btnH = 40;

    ui->sidebarBrand->setGeometry(0, 0, sidebarExpanded ? 200 : 52, 56);
    ui->labelBrandName->setVisible(sidebarExpanded);
    ui->labelBrandSub->setVisible(sidebarExpanded);

    ui->sidebarUserPanel->setGeometry(0, 56, sidebarExpanded ? 200 : 52, 60);
    ui->labelSidebarUsername->setVisible(sidebarExpanded);
    ui->labelSidebarGrade->setVisible(sidebarExpanded);

    int navTop = 128;
    ui->buttonNavHome      ->setGeometry(btnX, navTop,       btnW, btnH);
    ui->buttonNavExams     ->setGeometry(btnX, navTop + 44,  btnW, btnH);
    ui->buttonNavStatistics->setGeometry(btnX, navTop + 88,  btnW, btnH);
    ui->buttonNavLessons   ->setGeometry(btnX, navTop + 132, btnW, btnH);
    ui->buttonNavSchedule  ->setGeometry(btnX, navTop + 176, btnW, btnH);
    ui->buttonNavInbox     ->setGeometry(btnX, navTop + 220, btnW, btnH);
    ui->buttonSettings     ->setGeometry(btnX, 696, btnW, btnH);
    ui->buttonLogOut       ->setGeometry(btnX, 648, btnW, btnH);
    ui->buttonLoginRegister->setGeometry(btnX, 648, btnW, btnH);
}

void Home::updateSidebarButtons() {
    const char* navNorm     = sidebarExpanded ? Style::navExpanded         : Style::navCollapsed;
    const char* logoutSS    = sidebarExpanded ? Style::logoutExpanded      : Style::logoutCollapsed;
    const char* loginSS     = sidebarExpanded ? Style::loginExpanded       : Style::loginCollapsed;
    const char* navDisabled = sidebarExpanded ? Style::navDisabledExpanded : Style::navDisabledCollapsed;

    auto setup = [this](QPushButton *btn, const QString &label, const QString &iconPath, const char *ss) {
        btn->setStyleSheet(ss);
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(18, 18));
        if (sidebarExpanded) {
            btn->setText("   " + label);
            btn->setToolTip("");
        } else {
            btn->setText("");
            btn->setToolTip(label);
        }
    };

    setup(ui->buttonNavHome, "Home", ":/icons/home.svg", navNorm);
    if (UserSession::instance().isLoggedIn()) {
        setup(ui->buttonNavExams, "Exams", ":/icons/exam.svg", navNorm);
        setup(ui->buttonNavStatistics, "Statistics", ":/icons/diagram.svg", navNorm);
    } else {
        setup(ui->buttonNavExams, "Exams", ":/icons/exam.svg", navDisabled);
        setup(ui->buttonNavStatistics, "Statistics", ":/icons/diagram.svg", navDisabled);
    }
    if (UserSession::instance().isLoggedIn()) {
        setup(ui->buttonNavLessons, "Lessons", ":/icons/lessons.svg", navNorm);
        setup(ui->buttonNavSchedule, "Schedule", ":/icons/schedule.svg", navNorm);
        setup(ui->buttonNavInbox, "Inbox", ":/icons/inbox.svg", navNorm);
    } else {
        setup(ui->buttonNavLessons, "Lessons", ":/icons/lessons.svg", navDisabled);
        setup(ui->buttonNavSchedule, "Schedule", ":/icons/schedule.svg", navDisabled);
        setup(ui->buttonNavInbox, "Inbox", ":/icons/inbox.svg", navDisabled);
    }
    setup(ui->buttonSettings, "Settings", ":/icons/settings.svg", navNorm);

    if (UserSession::instance().isLoggedIn()) {
        setup(ui->buttonLogOut, "My Profile", ":/icons/user.svg", logoutSS);
        ui->buttonLogOut->setVisible(true);
        ui->buttonLoginRegister->setVisible(false);

        QString username = UserSession::instance().getUsername();
        QString grade    = UserSession::instance().getGrade();

        QPixmap sidebarAvatar = Utils::createAvatarPixmap(username, 34);
        ui->sidebarAvatarCircle->setStyleSheet(
            "QWidget#sidebarAvatarCircle { background-color: transparent; border-radius: 17px; "
            "min-width: 34px; max-width: 34px; min-height: 34px; max-height: 34px; }"
        );
        ui->labelSidebarAvatarInitial->setPixmap(sidebarAvatar);
        ui->labelSidebarAvatarInitial->setScaledContents(false);
        ui->labelSidebarAvatarInitial->setAlignment(Qt::AlignCenter);

        ui->sidebarUserPanel->setVisible(true);
        ui->labelSidebarUsername->setText(username);
        ui->labelSidebarGrade->setText("Grade " + grade);

    } else {
        setup(ui->buttonLoginRegister, "Login / Register", ":/icons/user.svg", loginSS);
        ui->buttonLoginRegister->setVisible(true);
        ui->buttonLogOut->setVisible(false);
        ui->sidebarUserPanel->setVisible(true);
        ui->labelSidebarUsername->setText("Guest");
        ui->labelSidebarGrade->setText("Guest");

        QPixmap guestAvatar = Utils::createAvatarPixmap("Guest", 34);
        ui->sidebarAvatarCircle->setStyleSheet(
            "QWidget#sidebarAvatarCircle { background-color: transparent; border-radius: 17px; "
            "min-width: 34px; max-width: 34px; min-height: 34px; max-height: 34px; }"
        );
        ui->labelSidebarAvatarInitial->setPixmap(guestAvatar);
        ui->labelSidebarAvatarInitial->setScaledContents(false);
        ui->labelSidebarAvatarInitial->setAlignment(Qt::AlignCenter);
    }

    setNavActive(activeNavIndex);
}

void Home::setNavActive(NavPage index) {
    activeNavIndex = index;

    const char* norm       = sidebarExpanded ? Style::navExpanded      : Style::navCollapsed;
    const char* act        = sidebarExpanded ? Style::navExpandedActive : Style::navCollapsedActive;
    const char* logoutNorm = sidebarExpanded ? Style::logoutExpanded    : Style::logoutCollapsed;

    ui->buttonNavHome->setStyleSheet(index == NavPage::Home ? act : norm);
    if (!UserSession::instance().isLoggedIn()) {
        ui->buttonNavExams     ->setStyleSheet(sidebarExpanded ? Style::navDisabledExpanded : Style::navDisabledCollapsed);
        ui->buttonNavStatistics->setStyleSheet(sidebarExpanded ? Style::navDisabledExpanded : Style::navDisabledCollapsed);
        ui->buttonNavLessons   ->setStyleSheet(sidebarExpanded ? Style::navDisabledExpanded : Style::navDisabledCollapsed);
        ui->buttonNavSchedule  ->setStyleSheet(sidebarExpanded ? Style::navDisabledExpanded : Style::navDisabledCollapsed);
        ui->buttonNavInbox     ->setStyleSheet(sidebarExpanded ? Style::navDisabledExpanded : Style::navDisabledCollapsed);
    } else {
        ui->buttonNavExams     ->setStyleSheet(index == NavPage::Exams      ? act : norm);
        ui->buttonNavStatistics->setStyleSheet(index == NavPage::Statistics ? act : norm);
        ui->buttonNavLessons   ->setStyleSheet(index == NavPage::Lessons    ? act : norm);
        ui->buttonNavSchedule  ->setStyleSheet(index == NavPage::Schedule   ? act : norm);
        ui->buttonNavInbox     ->setStyleSheet(index == NavPage::Inbox      ? act : norm);
    }
    ui->buttonSettings->setStyleSheet(index == NavPage::Settings ? act : norm);

    if (UserSession::instance().isLoggedIn()) {
        ui->buttonLogOut->setStyleSheet(index == NavPage::Profile ? act : logoutNorm);
    }
}
