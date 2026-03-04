#include "home.h"
#include "ui_home.h"
#include "login.h"
#include "usersession.h"
#include "../exam/style.h"

Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);

    sidebarExpanded = true;
    activeNavIndex = 0;

    currentQuestionIndex = 0;
    selectedAnswer = -1;
    correctCount = 0;
    timeLeft = 600;
    timerBlinkState = true;
    totalExamsTaken = 0;
    bestScore = -1;
    totalCorrect = 0;

    examTimer = new QTimer(this);
    examTimer->setInterval(1000);
    connect(examTimer, &QTimer::timeout, this, &Home::onExamTimerTick);

    blinkTimer = new QTimer(this);
    blinkTimer->setInterval(500);
    connect(blinkTimer, &QTimer::timeout, this, &Home::onExamTimerBlink);

    loadQuestions();

    connect(ui->buttonToggleSidebar, &QPushButton::clicked, this, &Home::toggleSidebar);

    connect(ui->buttonNavHome, &QPushButton::clicked, this, [this]() {
        setNavActive(0);
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->buttonNavExams, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        ui->stackedWidget->setCurrentIndex(1);
        ui->labelTimer->setStyleSheet(Style::timerNormal);
        startExam();
    });
    connect(ui->buttonStartExam, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        ui->stackedWidget->setCurrentIndex(1);
        ui->labelTimer->setStyleSheet(Style::timerNormal);
        startExam();
    });

    connect(ui->buttonLogOut, &QPushButton::clicked, this, &Home::onLogoutClicked);

    connect(ui->buttonLoginRegister, &QPushButton::clicked, this, [this]() {
        Login *loginPage = new Login();
        this->hide();
        loginPage->show();
    });

    connect(ui->buttonA, &QPushButton::clicked, this, [this]() { submitAnswer(0); });
    connect(ui->buttonB, &QPushButton::clicked, this, [this]() { submitAnswer(1); });
    connect(ui->buttonC, &QPushButton::clicked, this, [this]() { submitAnswer(2); });
    connect(ui->buttonD, &QPushButton::clicked, this, [this]() { submitAnswer(3); });

    connect(ui->buttonNext, &QPushButton::clicked, this, &Home::nextQuestion);
    connect(ui->buttonRetry, &QPushButton::clicked, this, &Home::retryExam);
    connect(ui->buttonBackHome, &QPushButton::clicked, this, [this]() {
        setNavActive(0);
        ui->stackedWidget->setCurrentIndex(0);
    });

    updateStatsCards();
    updateSidebarButtons();
    setNavActive(0);

    if (UserSession::instance().isLoggedIn()) {
        QString name = UserSession::instance().username();
        QString grade = UserSession::instance().grade();
        ui->labelWelcome->setText("Welcome back, " + name + " (Grade " + grade + ")! Ready to test your knowledge?");
    } else {
        ui->labelWelcome->setText("Login or create an account.");
    }
}

Home::~Home() { delete ui; }
