#include "./home.h"
#include "./ui_home.h"
#include "./home_styles.h"
#include "login.h"
#include "usersession.h"
#include <QIcon>
#include <QPropertyAnimation>
#include <QApplication>

Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);

    examLogic = new Exam(this);
    sidebarExpanded = true;
    activeNavIndex = 0;

    qApp->setStyleSheet(qApp->styleSheet() + R"(QToolTip { background-color: #1e1c35; color: #E8E8F0; border: 1px solid #7C5CFC; border-radius: 6px; padding: 4px 8px; font-size: 12px; })");

    ui->progressBar->setStyleSheet(Style::progressBar);
    ui->buttonStartExam->setStyleSheet(Style::startExamBtn);
    ui->buttonNext->setStyleSheet(Style::primaryBtn);
    ui->buttonRetry->setStyleSheet(Style::primaryBtn);
    ui->buttonBackHome->setStyleSheet(Style::secondaryBtn);
    ui->labelWelcome->setStyleSheet(Style::welcomeLabel);

    ui->labelCardValue1->setStyleSheet(Style::statValue);
    ui->labelCardValue2->setStyleSheet(Style::statValue);
    ui->labelCardValue3->setStyleSheet(Style::statValue);

    // signals
    connect(examLogic, &Exam::questionChanged, this, &Home::onExamQuestionChanged);
    connect(examLogic, &Exam::progressUpdated, this, &Home::onProgressUpdated);
    connect(examLogic, &Exam::answerSubmitted, this, &Home::onExamAnswerSubmitted);
    connect(examLogic, &Exam::timerUpdated, this, &Home::onExamTimerUpdated);
    connect(examLogic, &Exam::timerBlinkStateChanged, this, &Home::onExamTimerBlinked);
    connect(examLogic, &Exam::examFinished, this, &Home::onExamFinished);

    connect(ui->buttonToggleSidebar, &QPushButton::clicked, this, &Home::toggleSidebar);

    connect(ui->buttonNavHome, &QPushButton::clicked, this, [this]() {
        setNavActive(0);
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->buttonNavExams, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        onStartExamClicked();
    });
    connect(ui->buttonStartExam, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        onStartExamClicked();
    });

    connect(ui->buttonLogOut, &QPushButton::clicked, this, &Home::onLogoutClicked);

    connect(ui->buttonLoginRegister, &QPushButton::clicked, this, [this]() {
        Login *loginPage = new Login();
        this->hide();
        loginPage->show();
    });

    connect(ui->buttonA, &QPushButton::clicked, this, [this]() { examLogic->submitAnswer(0); });
    connect(ui->buttonB, &QPushButton::clicked, this, [this]() { examLogic->submitAnswer(1); });
    connect(ui->buttonC, &QPushButton::clicked, this, [this]() { examLogic->submitAnswer(2); });
    connect(ui->buttonD, &QPushButton::clicked, this, [this]() { examLogic->submitAnswer(3); });

    connect(ui->buttonNext, &QPushButton::clicked, examLogic, &Exam::nextQuestion);
    connect(ui->buttonRetry, &QPushButton::clicked, this, &Home::onRetryExamClicked);
    connect(ui->buttonBackHome, &QPushButton::clicked, this, &Home::onBackHomeClicked);

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

void Home::onLogoutClicked() {
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
    const char* navAct = sidebarExpanded ? Style::navExpandedActive : Style::navCollapsedActive;
    const char* logoutSS = sidebarExpanded ? Style::logoutExpanded : Style::logoutCollapsed;
    const char* loginSS = sidebarExpanded ? Style::loginExpanded : Style::loginCollapsed;

    auto setup = [this](QPushButton *btn, const QString &label,
                     const QString &iconPath, const char *ss)
    {
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
}

void Home::onStartExamClicked() {
    ui->stackedWidget->setCurrentIndex(1);
    ui->labelTimer->setStyleSheet(Style::timerNormal);
    examLogic->startExam();
}

void Home::showQuestion(int index) {
    if (index < 0 || index >= examLogic->getTotalQuestions()) return;

    Question question = examLogic->getCurrentQuestion();

    ui->labelQuestion->setText(question.text);
    ui->buttonA->setText("A)  " + question.optionA);
    ui->buttonB->setText("B)  " + question.optionB);
    ui->buttonC->setText("C)  " + question.optionC);
    ui->buttonD->setText("D)  " + question.optionD);

    ui->buttonNext->setEnabled(false);
    resetAnswerButtons();
}

void Home::onExamQuestionChanged(int index) {
    showQuestion(index);
}

void Home::onProgressUpdated(int current, int total) {
    ui->labelProgress->setText(QString::number(current) + " / " + QString::number(total));

    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

void Home::onExamAnswerSubmitted(int selected, int correct) {
    highlightAnswer(selected, correct);
    ui->buttonNext->setEnabled(true);
}

void Home::highlightAnswer(int answer, int correct) {
    QPushButton *btns[4] = { ui->buttonA, ui->buttonB, ui->buttonC, ui->buttonD };
    for (int i = 0; i < 4; i++) {
        if (i == correct) btns[i]->setStyleSheet(Style::answerCorrect);
        else if (i == answer && answer != correct) btns[i]->setStyleSheet(Style::answerWrong);
        else btns[i]->setStyleSheet(Style::answerNormal);
    }
}

void Home::onExamTimerUpdated(QString timeStr, bool isWarning) {
    updateTimerLabel(timeStr, isWarning);
}

void Home::updateTimerLabel(QString timeStr, bool isWarning) {
    ui->labelTimer->setText(timeStr);
    if (isWarning) {
        ui->labelTimer->setStyleSheet(Style::timerWarnOn);
    } else {
        ui->labelTimer->setStyleSheet(Style::timerNormal);
    }
}

void Home::onExamTimerBlinked(bool blinking) {
    if (blinking) {
        ui->labelTimer->setStyleSheet(Style::timerWarnOn);
    } else {
        ui->labelTimer->setStyleSheet(Style::timerWarnOff);
    }
}

void Home::onExamFinished() {
    QString color = examLogic->getGradeColor();
    QString grade = examLogic->calculateGrade();

    ui->labelScore->setText(QString("%1 / %2").arg(examLogic->getCorrectCount()).arg(examLogic->getTotalQuestions()));
    ui->labelScore->setStyleSheet(
        QString("color: %1; font-size: 42px; font-weight: 800;").arg(color));
    ui->labelGrade->setText("Grade: " + grade);
    ui->labelGrade->setStyleSheet(
        QString("color: %1; font-size: 16px; font-weight: 600;").arg(color));

    updateStatsCards();
    ui->stackedWidget->setCurrentIndex(2);
}

void Home::onRetryExamClicked() {
    examLogic->retry();
}

void Home::updateStatsCards() {
    int totalExamsTaken = examLogic->getTotalExamsTaken();
    int bestScore = examLogic->getBestScore();
    int totalCorrect = examLogic->getTotalCorrect();

    ui->labelCardValue1->setText(QString::number(totalExamsTaken));
    ui->labelCardValue2->setText(bestScore >= 0 ? QString("%1/20").arg(bestScore) : "-");

    if (totalExamsTaken > 0) {
        double avg = (double)totalCorrect / totalExamsTaken;
        ui->labelCardValue3->setText(QString::number(avg, 'f', 1));
    } else {
        ui->labelCardValue3->setText("-");
    }
}

void Home::onBackHomeClicked() {
    setNavActive(0);
    ui->stackedWidget->setCurrentIndex(0);
}

void Home::resetAnswerButtons() {
    ui->buttonA->setStyleSheet(Style::answerNormal);
    ui->buttonB->setStyleSheet(Style::answerNormal);
    ui->buttonC->setStyleSheet(Style::answerNormal);
    ui->buttonD->setStyleSheet(Style::answerNormal);
}
