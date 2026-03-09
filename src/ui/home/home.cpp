#include "home.h"
#include "ui_home.h"
#include "login.h"
#include "usersession.h"
#include <QMessageBox>

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
        populateExamHistory();
        ui->stackedWidget->setCurrentIndex(6);
    });
    connect(ui->buttonStartExam, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        ui->stackedWidget->setCurrentIndex(1);
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
    connect(ui->buttonSkip, &QPushButton::clicked, this, &Home::skipExam);
    connect(ui->buttonBackHome, &QPushButton::clicked, this, [this]() {
        setNavActive(0);
        updateStatsCards();
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->buttonViewExams, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        populateExamHistory();
        ui->stackedWidget->setCurrentIndex(6);
    });

    connect(ui->buttonSettings, &QPushButton::clicked, this, [this]() {
        setNavActive(2);
        ui->stackedWidget->setCurrentIndex(3);
    });

    connect(ui->buttonNewExam, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        ui->stackedWidget->setCurrentIndex(1);
        startExam();
    });

    initSettingsPage();
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
