#include "home.h"
#include "ui_home.h"
#include "login.h"
#include "usersession.h"
#include "profile.h"
#include <QMessageBox>

Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);

    settingsPage = new Settings(this);
    ui->stackedWidget->addWidget(settingsPage);

    historyPage = new History(this);
    ui->stackedWidget->addWidget(historyPage);

    profilePage = new Profile(this);
    ui->stackedWidget->addWidget(profilePage);

    sessionPage = new Session(this);
    ui->stackedWidget->addWidget(sessionPage);

    resultsPage = new Results(this);
    ui->stackedWidget->addWidget(resultsPage);

    difficultiesPage = new Difficulties(this);
    ui->stackedWidget->addWidget(difficultiesPage);

    subjectsPage = new Subjects(this);
    ui->stackedWidget->addWidget(subjectsPage);

    sidebarExpanded = true;
    activeNavIndex = 0;

    connect(ui->buttonToggleSidebar, &QPushButton::clicked, this, &Home::toggleSidebar);

    connect(ui->buttonNavHome, &QPushButton::clicked, this, [this]() {
        setNavActive(0);
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->buttonNavExams, &QPushButton::clicked, this, [this]() {
        if (!UserSession::instance().isLoggedIn()) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Account Required");
            msgBox.setText("You need an account to take exams.\nWould you like to create one?");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
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
            if (msgBox.exec() == QMessageBox::Yes) {
                Login *loginPage = new Login();
                this->hide();
                loginPage->show();
            }
            return;
        }
        setNavActive(1);
        historyPage->populate(sessionPage->getExamHistory());
        ui->stackedWidget->setCurrentWidget(historyPage);
    });
    connect(ui->buttonStartExam, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        ui->stackedWidget->setCurrentWidget(sessionPage);
        sessionPage->startExam();
    });

    connect(ui->buttonLogOut, &QPushButton::clicked, this, [this]() {
        setNavActive(3);
        ui->stackedWidget->setCurrentWidget(profilePage);
    });

    connect(ui->buttonLoginRegister, &QPushButton::clicked, this, [this]() {
        Login *loginPage = new Login();
        this->hide();
        loginPage->show();
    });

    connect(ui->buttonSettings, &QPushButton::clicked, this, [this]() {
        setNavActive(2);
        settingsPage->refresh();
        ui->stackedWidget->setCurrentWidget(settingsPage);
    });

    connect(profilePage, &Profile::logoutRequested, this, &Home::onLogoutClicked);

    connect(settingsPage, &Settings::accountDeleted, this, [this]() {
        Login *loginPage = new Login();
        this->hide();
        loginPage->show();
    });

    connect(historyPage, &History::newExamRequested, this, [this]() {
        setNavActive(1);
        ui->stackedWidget->setCurrentWidget(sessionPage);
        sessionPage->startExam();
    });

    connect(sessionPage, &Session::examCompleted, this, [this](int score, int total) {
        updateStatsCards();
        resultsPage->displayResults(score, total);
        ui->stackedWidget->setCurrentWidget(resultsPage);
    });
    connect(resultsPage, &Results::backToHomeRequested, this, [this]() {
        setNavActive(0);
        updateStatsCards();
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(resultsPage, &Results::viewExamsRequested, this, [this]() {
        setNavActive(1);
        historyPage->populate(sessionPage->getExamHistory());
        ui->stackedWidget->setCurrentWidget(historyPage);
    });
    connect(sessionPage, &Session::sidebarToggleRequested, this, &Home::toggleSidebar);

    updateStatsCards();
    updateSidebarButtons();
    setNavActive(0);

    if (UserSession::instance().isLoggedIn()) {
        QString name = UserSession::instance().getUsername();
        QString grade = UserSession::instance().getGrade();
        ui->labelWelcome->setText("Welcome back, " + name + " (Grade " + grade + ")! Ready to test your knowledge?");
    } else {
        ui->labelWelcome->setText("Login or create an account.");
    }
}

Home::~Home() { delete ui; }

void Home::updateStatsCards() {
    int totalExamsTaken = sessionPage->getTotalExamsTaken();
    int bestScore = sessionPage->getBestScore();
    int totalCorrect = sessionPage->getTotalCorrect();

    ui->labelCardValue1->setText(QString::number(totalExamsTaken));
    ui->labelCardValue2->setText(bestScore >= 0 ? QString("%1/20").arg(bestScore) : "-");

    if (totalExamsTaken > 0) {
        double avg = (double)totalCorrect / totalExamsTaken;
        ui->labelCardValue3->setText(QString::number(avg, 'f', 1));
    } else {
        ui->labelCardValue3->setText("-");
    }
}
