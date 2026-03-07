#include "home.h"
#include "ui_home.h"
#include "login.h"
#include "usersession.h"
#include "style.h"
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QPushButton>
#include <QRect>
#include <QSize>
#include <QEasingCurve>
#include <QMessageBox>

void Home::onLogoutClicked() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Log Out");
    msgBox.setText("Are you sure you want to log out?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet(R"(
        QMessageBox {
            background-color: #161618;
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
    )");

    if (msgBox.exec() != QMessageBox::Yes) return;

    UserSession::instance().clear();
    Login *loginPage = new Login();
    this->hide();
    loginPage->show();
}

void Home::toggleSidebar() {
    sidebarExpanded = !sidebarExpanded;

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

    ui->buttonNavHome ->setGeometry(btnX,  60, btnW, btnH);
    ui->buttonNavExams->setGeometry(btnX, 108, btnW, btnH);
    ui->buttonSettings->setGeometry(btnX, 696, btnW, btnH);
    ui->buttonLogOut->setGeometry(btnX, 648, btnW, btnH);
    ui->buttonLoginRegister->setGeometry(btnX, 648, btnW, btnH);
}

void Home::updateSidebarButtons() {
    const char* navNorm = sidebarExpanded ? Style::navExpanded : Style::navCollapsed;
    const char* logoutSS = sidebarExpanded ? Style::logoutExpanded : Style::logoutCollapsed;
    const char* loginSS = sidebarExpanded ? Style::loginExpanded : Style::loginCollapsed;

    auto setup = [this](QPushButton *btn, const QString &label, const QString &iconPath, const char *ss) {
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(18, 18));
        btn->setStyleSheet(ss);
        if (sidebarExpanded) {
            btn->setText("   " + label);
            btn->setToolTip("");
        } else {
            btn->setText("");
            btn->setToolTip(label);
        }
    };

    setup(ui->buttonNavHome, "Home", ":/icons/home.svg", navNorm);
    setup(ui->buttonNavExams, "Exams", ":/icons/exam.svg", navNorm);
    setup(ui->buttonSettings, "Settings", ":/icons/settings.svg", navNorm);

    setNavActive(activeNavIndex);

    if (UserSession::instance().isLoggedIn()) {
        setup(ui->buttonLogOut, "Log Out", ":/icons/user.svg", logoutSS);
        ui->buttonLogOut->setVisible(true);
        ui->buttonLoginRegister->setVisible(false);
    } else {
        setup(ui->buttonLoginRegister, "Login / Register", ":/icons/user.svg", loginSS);
        ui->buttonLoginRegister->setVisible(true);
        ui->buttonLogOut->setVisible(false);
    }
}

void Home::setNavActive(int index) {
    activeNavIndex = index;

    const char* norm = sidebarExpanded ? Style::navExpanded : Style::navCollapsed;
    const char* act = sidebarExpanded ? Style::navExpandedActive : Style::navCollapsedActive;

    ui->buttonNavHome ->setStyleSheet(index == 0 ? act : norm);
    ui->buttonNavExams->setStyleSheet(index == 1 ? act : norm);
    ui->buttonSettings->setStyleSheet(index == 2 ? act : norm);
}
