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
#include <QPainter>
#include <QPainterPath>
#include <QLabel>

static QPixmap makeCircularPfp(const QString &avatarName, int size) {
    QString path = QString(":/icons/%1.png").arg(avatarName);
    QPixmap src(path);
    if (src.isNull()) return QPixmap();

    QPixmap scaled = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int x = (scaled.width()  - size) / 2;
    int y = (scaled.height() - size) / 2;
    scaled = scaled.copy(x, y, size, size);

    QPixmap result(size, size);
    result.fill(Qt::transparent);
    QPainter p(&result);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath clipPath;
    clipPath.addEllipse(0, 0, size, size);
    p.setClipPath(clipPath);
    p.drawPixmap(0, 0, scaled);
    return result;
}

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

    setup(ui->buttonNavHome, "Home", ":/icons/homeDark.svg", navNorm);
    if (UserSession::instance().isLoggedIn()) {
        setup(ui->buttonNavExams,      "Exams",      ":/icons/examDark.svg",    navNorm);
        setup(ui->buttonNavStatistics, "Statistics", ":/icons/diagramDark.svg", navNorm);
    } else {
        setup(ui->buttonNavExams,      "Exams",      ":/icons/examDark.svg",    navDisabled);
        setup(ui->buttonNavStatistics, "Statistics", ":/icons/diagramDark.svg", navDisabled);
    }
    setup(ui->buttonSettings, "Settings", ":/icons/settingsDark.svg", navNorm);

    if (UserSession::instance().isLoggedIn()) {
        setup(ui->buttonLogOut, "My Profile", ":/icons/userDark.svg", logoutSS);
        ui->buttonLogOut->setVisible(true);
        ui->buttonLoginRegister->setVisible(false);

        QString username = UserSession::instance().getUsername();
        QString grade    = UserSession::instance().getGrade();
        QString avatar   = UserSession::instance().getAvatar();
        QString initial  = username.isEmpty() ? "?" : username.left(1).toUpper();

        QPixmap sidebarPfp = makeCircularPfp(avatar, 34);
        if (!sidebarPfp.isNull()) {
            ui->sidebarAvatarCircle->setStyleSheet(
                "QWidget#sidebarAvatarCircle { background-color: transparent; border-radius: 17px; "
                "min-width: 34px; max-width: 34px; min-height: 34px; max-height: 34px; }"
            );
            ui->labelSidebarAvatarInitial->setText("");
            ui->labelSidebarAvatarInitial->setPixmap(sidebarPfp);
            ui->labelSidebarAvatarInitial->setScaledContents(false);
            ui->labelSidebarAvatarInitial->setAlignment(Qt::AlignCenter);
        } else {
            ui->sidebarAvatarCircle->setStyleSheet(
                "QWidget#sidebarAvatarCircle { background-color: #4f6fc4; border-radius: 17px; "
                "min-width: 34px; max-width: 34px; min-height: 34px; max-height: 34px; }"
            );
            ui->labelSidebarAvatarInitial->setPixmap(QPixmap());
            ui->labelSidebarAvatarInitial->setText(initial);
            ui->labelSidebarAvatarInitial->setStyleSheet(
                "color: #ffffff; font-size: 13px; font-weight: 700; background: transparent;"
            );
        }

        QPixmap topBarPfp = makeCircularPfp(avatar, 28);

        auto applyTopBarAvatar = [&](QWidget *avatarWidget, QLabel *avatarLabel) {
            if (!avatarWidget || !avatarLabel) return;
            if (!topBarPfp.isNull()) {
                avatarWidget->setStyleSheet(
                    "QWidget { background-color: transparent; border-radius: 14px; "
                    "min-width: 28px; max-width: 28px; min-height: 28px; max-height: 28px; }"
                );
                avatarLabel->setText("");
                avatarLabel->setPixmap(topBarPfp);
                avatarLabel->setScaledContents(false);
                avatarLabel->setAlignment(Qt::AlignCenter);
            } else {
                avatarWidget->setStyleSheet(
                    "QWidget { background-color: #4f6fc4; border-radius: 14px; "
                    "min-width: 28px; max-width: 28px; min-height: 28px; max-height: 28px; }"
                );
                avatarLabel->setPixmap(QPixmap());
                avatarLabel->setText(initial);
                avatarLabel->setStyleSheet(
                    "color: #ffffff; font-size: 12px; font-weight: 700; background: transparent;"
                );
            }
        };

        applyTopBarAvatar(ui->labelTopBarAvatar, ui->labelTopBarAvatarText);

        QList<QWidget*> subPages = {
            profilePage,
            historyPage,
            settingsPage,
            statisticsPage,
            reviewPage,
            difficultiesPage,
            subjectsPage
        };
        for (QWidget *page : subPages) {
            if (!page) continue;
            QWidget *avatarWidget = page->findChild<QWidget*>("labelTopBarAvatar");
            if (!avatarWidget) continue;
            QLabel *avatarLabel = avatarWidget->findChild<QLabel*>("labelTopBarAvatarText");
            applyTopBarAvatar(avatarWidget, avatarLabel);
            avatarWidget->setVisible(true);

            QLabel *gradeLabel = page->findChild<QLabel*>("labelTopBarGrade");
            if (gradeLabel) {
                gradeLabel->setText("Grade " + grade);
                gradeLabel->setVisible(true);
            }
        }

        ui->labelTopBarGrade->setText("Grade " + grade);
        ui->labelTopBarGrade->setVisible(true);
        ui->labelTopBarAvatar->setVisible(true);
        ui->sidebarUserPanel->setVisible(true);
        ui->labelSidebarUsername->setText(username);
        ui->labelSidebarGrade->setText("Grade " + grade);

    } else {
        setup(ui->buttonLoginRegister, "Login / Register", ":/icons/userDark.svg", loginSS);
        ui->buttonLoginRegister->setVisible(true);
        ui->buttonLogOut->setVisible(false);
        ui->labelTopBarGrade->setVisible(false);
        ui->labelTopBarAvatar->setVisible(false);
        ui->sidebarUserPanel->setVisible(false);
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
    } else {
        ui->buttonNavExams     ->setStyleSheet(index == NavPage::Exams      ? act : norm);
        ui->buttonNavStatistics->setStyleSheet(index == NavPage::Statistics ? act : norm);
    }
    ui->buttonSettings->setStyleSheet(index == NavPage::Settings ? act : norm);

    if (UserSession::instance().isLoggedIn()) {
        ui->buttonLogOut->setStyleSheet(index == NavPage::Profile ? act : logoutNorm);
    }
}