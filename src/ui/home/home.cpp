#include "home.h"
#include "ui_home.h"
#include "login.h"
#include "usersession.h"
#include "profile.h"
#include "database.h"
#include "statistics.h"
#include "utils.h"
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

    reviewPage = new Review(this);
    ui->stackedWidget->addWidget(reviewPage);

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

    statisticsPage = new Statistics(this);
    ui->stackedWidget->addWidget(statisticsPage);

    sidebarExpanded = true;
    activeNavIndex = NavPage::Home;
    selectedExamSubject = "History";

    connect(ui->buttonToggleSidebar, &QPushButton::clicked, this, &Home::toggleSidebar);

    connect(ui->buttonNavHome, &QPushButton::clicked, this, [this]() {
        setNavActive(NavPage::Home);
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->buttonNavExams, &QPushButton::clicked, this, [this]() {
        if (!UserSession::instance().isLoggedIn()) { showAccountRequired(); return; }
        setNavActive(NavPage::Exams);
        historyPage->loadAndPopulate(UserSession::instance().getUsername());
        ui->stackedWidget->setCurrentWidget(historyPage);
    });
    connect(ui->buttonNavStatistics, &QPushButton::clicked, this, [this]() {
        if (!UserSession::instance().isLoggedIn()) { showAccountRequired(); return; }
        setNavActive(NavPage::Statistics);
        statisticsPage->refresh();
        ui->stackedWidget->setCurrentWidget(statisticsPage);
    });
    connect(ui->buttonLogOut, &QPushButton::clicked, this, [this]() {
        setNavActive(NavPage::Profile);
        ui->stackedWidget->setCurrentWidget(profilePage);
    });
    connect(profilePage, &Profile::editProfileRequested, this, [this]() {
        setNavActive(NavPage::Settings);
        ui->stackedWidget->setCurrentWidget(settingsPage);
    });
    connect(ui->buttonLoginRegister, &QPushButton::clicked, this, [this]() {
        Login *loginPage = new Login();
        this->hide();
        loginPage->show();
    });
    connect(ui->buttonSettings, &QPushButton::clicked, this, [this]() {
        setNavActive(NavPage::Settings);
        ui->stackedWidget->setCurrentWidget(settingsPage);
    });
    connect(profilePage, &Profile::logoutRequested, this, &Home::onLogoutClicked);
    connect(settingsPage, &Settings::accountDeleted, this, [this]() {
        Login *loginPage = new Login();
        this->hide();
        loginPage->show();
    });
    connect(historyPage, &History::newExamRequested, this, [this]() {
        setNavActive(NavPage::Exams);
        ui->stackedWidget->setCurrentWidget(subjectsPage);
    });
    connect(historyPage, &History::reviewRequested, this, [this](int attemptId) {
        QList<ExamAttempt> attempts = Database::instance().loadExamAttemptsForUser(UserSession::instance().getUsername());
        for (int i = 0; i < attempts.size(); i++) {
            if (attempts[i].id == attemptId) {
                reviewPage->loadAndShow(attemptId, attempts[i].subject, attempts[i].difficulty, attempts[i].score);
                ui->stackedWidget->setCurrentWidget(reviewPage);
                return;
            }
        }
    });
    connect(reviewPage, &Review::backToHistoryRequested, this, [this]() {
        setNavActive(NavPage::Exams);
        historyPage->loadAndPopulate(UserSession::instance().getUsername());
        ui->stackedWidget->setCurrentWidget(historyPage);
    });
    connect(subjectsPage, &Subjects::subjectSelected, this, [this](const QString &subject) {
        selectedExamSubject = subject;
        ui->stackedWidget->setCurrentWidget(difficultiesPage);
    });
    connect(difficultiesPage, &Difficulties::difficultySelected, this, [this](int difficulty) {
        QString difficultySelected = "Beginner";
        if (difficulty == 1) difficultySelected = "Intermediate";
        if (difficulty == 2) difficultySelected = "Advanced";
        ui->stackedWidget->setCurrentWidget(sessionPage);
        sessionPage->startExam(selectedExamSubject, difficultySelected);
    });
    connect(sessionPage, &Session::examCompleted, this, [this](int score, int total) {
        resultsPage->displayResults(score, total);
        updateStatsCards();
        statisticsPage->refresh();
        ui->stackedWidget->setCurrentWidget(resultsPage);
    });
    connect(resultsPage, &Results::backToHomeRequested, this, [this]() {
        setNavActive(NavPage::Home);
        updateStatsCards();
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(resultsPage, &Results::viewExamsRequested, this, [this]() {
        setNavActive(NavPage::Exams);
        historyPage->loadAndPopulate(UserSession::instance().getUsername());
        ui->stackedWidget->setCurrentWidget(historyPage);
    });
    connect(sessionPage, &Session::sidebarToggleRequested, this, &Home::toggleSidebar);

    connect(ui->buttonQuickExam, &QPushButton::clicked, this, [this]() {
        if (!UserSession::instance().isLoggedIn()) { showAccountRequired(); return; }
        setNavActive(NavPage::Exams);
        ui->stackedWidget->setCurrentWidget(subjectsPage);
    });
    connect(ui->buttonQuickHistory, &QPushButton::clicked, this, [this]() {
        if (!UserSession::instance().isLoggedIn()) { showAccountRequired(); return; }
        setNavActive(NavPage::Exams);
        historyPage->loadAndPopulate(UserSession::instance().getUsername());
        ui->stackedWidget->setCurrentWidget(historyPage);
    });


    updateSidebarButtons();
    setNavActive(NavPage::Home);
    updateStatsCards();

    if (UserSession::instance().isLoggedIn()) {
        QString name  = UserSession::instance().getUsername();
        QString grade = UserSession::instance().getGrade();
        ui->labelWelcome->setText("Welcome back, " + name + " (Grade " + grade + "). Ready to test your knowledge?");
    } else {
        ui->labelWelcome->setText("Login or create an account to get started.");
    }
}

Home::~Home() { delete ui; }

void Home::updateStatsCards() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->labelCardValue1->setText("0");
        ui->labelCardValue2->setText("-");
        ui->labelCardValue3->setText("-");
        return;
    }

    QString username = UserSession::instance().getUsername();
    QList<ExamAttempt> attempts = Database::instance().loadExamAttemptsForUser(username);

    int totalExamsTaken = attempts.size();
    int bestScore = -1;
    int totalScore = 0;

    for (int i = 0; i < totalExamsTaken; i++) {
        totalScore += attempts[i].score;
        if (bestScore == -1 || attempts[i].score > bestScore)
            bestScore = attempts[i].score;
    }

    ui->labelCardValue1->setText(QString::number(totalExamsTaken));
    ui->labelCardValue2->setText(bestScore >= 0 ? Utils::gradeFromScore(bestScore) : "-");

    if (totalExamsTaken > 0) {
        double avg = (double)totalScore / totalExamsTaken;
        ui->labelCardValue3->setText(Utils::gradeFromScore(avg));
    } else {
        ui->labelCardValue3->setText("-");
    }
}

void Home::showAccountRequired() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Account Required");
    msgBox.setText("You need an account to continue.\nWould you like to create one?");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setStyleSheet(R"(
        QMessageBox { background-color: #f4f5fa; color: #1a2440; }
        QMessageBox QLabel { color: #1a2440; font-size: 13px; }
        QPushButton {
            background-color: #ffffff; color: #1a2440; font-size: 13px; font-weight: 600;
            border: 1px solid #d4d8ea; border-radius: 6px; padding: 8px 20px; min-width: 80px;
        }
        QPushButton:hover { background-color: #eceef5; border-color: #b0bcd8; }
    )");
    if (msgBox.exec() == QMessageBox::Yes) {
        Login *loginPage = new Login();
        this->hide();
        loginPage->show();
    }
}