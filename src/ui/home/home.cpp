#include "./home.h"
#include "./ui_home.h"
#include "./home_styles.h"
#include "login.h"
#include "usersession.h"
#include <QRandomGenerator>
#include <QIcon>
#include <QPropertyAnimation>
#include <QApplication>
#include <algorithm>

Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Home)
    , sidebarExpanded(true)
    , timerBlinkState(true)
    , currentQuestionIndex(0)
    , selectedAnswer(-1)
    , correctCount(0)
    , timeLeft(600)
    , totalExamsTaken(0)
    , bestScore(-1)
    , totalCorrect(0)
    , activeNavIndex(0)
{
    ui->setupUi(this);

    examTimer = new QTimer(this);
    examTimer->setInterval(1000);
    connect(examTimer, &QTimer::timeout, this, &Home::onTimerTick);

    blinkTimer = new QTimer(this);
    blinkTimer->setInterval(500);
    connect(blinkTimer, &QTimer::timeout, this, &Home::onTimerBlink);

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

    connect(ui->buttonToggleSidebar, &QPushButton::clicked, this, &Home::toggleSidebar);

    connect(ui->buttonNavHome, &QPushButton::clicked, this, [this]() {
        setNavActive(0);
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->buttonNavExams, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        startExam();
    });
    connect(ui->buttonStartExam, &QPushButton::clicked, this, [this]() {
        setNavActive(1);
        startExam();
    });

    connect(ui->buttonLogOut, &QPushButton::clicked, this, &Home::onLogoutClicked);

    connect(ui->buttonLoginRegister, &QPushButton::clicked, this, [this]() {
        Login *loginPage = new Login();
        this->hide();
        loginPage->show();
    });

    connect(ui->buttonA, &QPushButton::clicked, this, [this]() { onAnswerSelected(0); });
    connect(ui->buttonB, &QPushButton::clicked, this, [this]() { onAnswerSelected(1); });
    connect(ui->buttonC, &QPushButton::clicked, this, [this]() { onAnswerSelected(2); });
    connect(ui->buttonD, &QPushButton::clicked, this, [this]() { onAnswerSelected(3); });

    connect(ui->buttonNext, &QPushButton::clicked, this, &Home::onNextClicked);
    connect(ui->buttonRetry, &QPushButton::clicked, this, &Home::onRetryClicked);
    connect(ui->buttonBackHome, &QPushButton::clicked, this, &Home::onBackHomeClicked);

    loadQuestions();
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

    auto setup = [&](QPushButton *btn, const QString &label,
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

    setup(ui->buttonNavHome,  "Home", ":/icons/home.svg", navNorm);
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

void Home::loadQuestions() {
    auto addQ = [this](QString text, QString a, QString b, QString c, QString d, int correct, QString cat) {
        Question q;
        q.text = text;
        q.optionA = a; q.optionB = b; q.optionC = c; q.optionD = d;
        q.correctAnswer = correct;
        q.category = cat;
        allQuestions.append(q);
    };

    addQ("When was the city of Rome traditionally founded?",
         "753 BC","509 BC","264 BC","44 BC", 0, "Ancient");
    addQ("Who was the first Emperor of China?",
         "Kublai Khan","Sun Yat-sen","Qin Shi Huang","Confucius", 2, "Ancient");
    addQ("Which ancient wonder was located in Alexandria, Egypt?",
         "The Colossus","The Lighthouse","The Mausoleum","The Statue of Zeus", 1, "Ancient");
    addQ("In which century did the fall of the Western Roman Empire occur?",
         "3rd century AD","4th century AD","5th century AD","6th century AD", 2, "Ancient");
    addQ("Who built the Parthenon in Athens?",
         "Spartans","Macedonians","Athenians","Persians", 2, "Ancient");
    addQ("What was the name of Alexander the Great's famous horse?",
         "Pegasus","Bucephalus","Arion","Shadowfax", 1, "Ancient");
    addQ("Which ancient civilization built Machu Picchu?",
         "Aztec","Maya","Inca","Olmec", 2, "Ancient");
    addQ("The Library of Alexandria was located in which modern-day country?",
         "Greece","Turkey","Egypt","Libya", 2, "Ancient");
    addQ("In which year did the Battle of Hastings take place?",
         "1066","1215","1348","1453", 0, "Middle Ages");
    addQ("What was the Black Death?",
         "A volcanic eruption","A bubonic plague pandemic","A series of wars","A drought", 1, "Middle Ages");
    addQ("Who signed the Magna Carta in 1215?",
         "Richard I","Edward I","Henry III","King John", 3, "Middle Ages");
    addQ("The Crusades were primarily fought over which city?",
         "Constantinople","Jerusalem","Alexandria","Rome", 1, "Middle Ages");
    addQ("Which empire conquered Constantinople in 1453?",
         "Mongol Empire","Russian Empire","Ottoman Empire","Persian Empire", 2, "Middle Ages");
    addQ("What was the primary language of the Catholic Church in medieval Europe?",
         "Greek","French","Latin","Italian", 2, "Middle Ages");
    addQ("In which year did World War I begin?",
         "1912","1914","1916","1918", 1, "Modern");
    addQ("In which year did World War II end?",
         "1943","1944","1945","1946", 2, "Modern");
    addQ("Who was the first person to walk on the Moon?",
         "Buzz Aldrin","Yuri Gagarin","Neil Armstrong","John Glenn", 2, "Modern");
    addQ("The Berlin Wall fell in which year?",
         "1987","1988","1989","1991", 2, "Modern");
    addQ("Which country was the first to grant women the right to vote?",
         "USA","UK","France","New Zealand", 3, "Modern");
    addQ("Who was the leader of the Soviet Union during World War II?",
         "Lenin","Trotsky","Khrushchev","Stalin", 3, "Modern");
    addQ("The French Revolution began in which year?",
         "1776","1789","1799","1804", 1, "Modern");
    addQ("Which war was fought between the North and South of the United States?",
         "The Revolutionary War","The War of 1812","The Civil War","The Mexican-American War", 2, "Modern");
    addQ("What was the code name for the D-Day invasion?",
         "Operation Barbarossa","Operation Overlord","Operation Market Garden","Operation Torch", 1, "WWII");
    addQ("Which country did Germany invade to start WWII?",
         "France","Belgium","Poland","Netherlands", 2, "WWII");
    addQ("Who was the British Prime Minister during most of WWII?",
         "Neville Chamberlain","Clement Attlee","Anthony Eden","Winston Churchill", 3, "WWII");
    addQ("In which city were the atomic bombs dropped in 1945?",
         "Tokyo and Osaka","Hiroshima and Nagasaki","Kyoto and Hiroshima","Nagasaki and Tokyo", 1, "WWII");
    addQ("What did 'V-E Day' stand for in WWII?",
         "Victory in Europe","Victory in East Asia","Veterans of Europe","Valor in Europe", 0, "WWII");
    addQ("Which battle is considered the turning point of WWII on the Eastern Front?",
         "Battle of Kursk","Battle of Moscow","Battle of Stalingrad","Battle of Berlin", 2, "WWII");
    addQ("The Nuremberg Trials prosecuted leaders of which country?",
         "Japan","Italy","Germany","Austria", 2, "WWII");
    addQ("What was the Holocaust?",
         "A German military operation","The genocide of Jews and others by Nazi Germany",
         "A British bombing campaign","A Soviet offensive", 1, "WWII");
    addQ("Which country suffered the most military deaths in WWII?",
         "USA","Germany","China","Soviet Union", 3, "WWII");
}

void Home::startExam() {
    blinkTimer->stop();
    examTimer->stop();

    currentQuestionIndex = 0;
    selectedAnswer  = -1;
    correctCount = 0;
    timeLeft = 600;
    timerBlinkState = true;
    ui->labelTimer->setStyleSheet(Style::timerNormal);

    examQuestions = allQuestions;
    std::shuffle(examQuestions.begin(), examQuestions.end(),
                 std::default_random_engine(QRandomGenerator::global()->generate()));
    while (examQuestions.size() > 20) examQuestions.removeLast();

    ui->stackedWidget->setCurrentIndex(1);
    showQuestion(0);
    examTimer->start();
    updateTimerLabel();
}

void Home::showQuestion(int index) {
    if (index >= examQuestions.size()) return;

    const Question &q = examQuestions[index];

    ui->labelQuestion->setText(q.text);
    ui->buttonA->setText("A)  " + q.optionA);
    ui->buttonB->setText("B)  " + q.optionB);
    ui->buttonC->setText("C)  " + q.optionC);
    ui->buttonD->setText("D)  " + q.optionD);

    ui->labelProgress->setText(
        QString("Question %1 of %2").arg(index + 1).arg(examQuestions.size()));
    ui->progressBar->setValue(index + 1);
    ui->progressBar->setMaximum(examQuestions.size());

    ui->buttonNext->setEnabled(false);
    selectedAnswer = -1;
    resetAnswerButtons();
}

void Home::onAnswerSelected(int answer) {
    if (selectedAnswer != -1) return;
    selectedAnswer = answer;
    ui->buttonNext->setEnabled(true);
    highlightAnswer(answer);
}

void Home::highlightAnswer(int answer) {
    int correct = examQuestions[currentQuestionIndex].correctAnswer;

    QPushButton *btns[4] = { ui->buttonA, ui->buttonB, ui->buttonC, ui->buttonD };
    for (int i = 0; i < 4; i++) {
        if (i == correct) btns[i]->setStyleSheet(Style::answerCorrect);
        else if (i == answer && answer != correct) btns[i]->setStyleSheet(Style::answerWrong);
        else btns[i]->setStyleSheet(Style::answerNormal);
    }

    if (answer == correct) correctCount++;
}

void Home::onNextClicked() {
    currentQuestionIndex++;
    if (currentQuestionIndex >= examQuestions.size()) {
        showResults();
    } else {
        showQuestion(currentQuestionIndex);
    }
}

void Home::onTimerTick() {
    timeLeft--;
    updateTimerLabel();
    if (timeLeft == 120) blinkTimer->start();
    if (timeLeft <= 0) {
        blinkTimer->stop();
        examTimer->stop();
        showResults();
    }
}

void Home::onTimerBlink() {
    timerBlinkState = !timerBlinkState;
    ui->labelTimer->setStyleSheet(timerBlinkState ? Style::timerWarnOn : Style::timerWarnOff);
}

void Home::updateTimerLabel() {
    int m = timeLeft / 60, s = timeLeft % 60;
    ui->labelTimer->setText(
        QString("⏱ %1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')));
    if (timeLeft > 120)
        ui->labelTimer->setStyleSheet(Style::timerNormal);
}

QString Home::calculateGrade(int correct, int total) {
    double pct = (double)correct / total * 100.0;
    if (pct >= 90) return "Excellent (6)";
    if (pct >= 75) return "Very Good (5)";
    if (pct >= 60) return "Good (4)";
    if (pct >= 45) return "Average (3)";
    if (pct >= 30) return "Poor (2)";
    return "Very Poor (2-)";
}

QString Home::gradeColor(int correct, int total) {
    double pct = (double)correct / total * 100.0;
    if (pct >= 75) return "#00D4AA";
    if (pct >= 45) return "#F5C518";
    return "#FF6B6B";
}

void Home::showResults() {
    blinkTimer->stop();
    examTimer->stop();

    QString color = gradeColor(correctCount, examQuestions.size());
    QString grade = calculateGrade(correctCount, examQuestions.size());

    ui->labelScore->setText(QString("%1 / %2").arg(correctCount).arg(examQuestions.size()));
    ui->labelScore->setStyleSheet(
        QString("color: %1; font-size: 42px; font-weight: 800;").arg(color));
    ui->labelGrade->setText("Grade: " + grade);
    ui->labelGrade->setStyleSheet(
        QString("color: %1; font-size: 16px; font-weight: 600;").arg(color));

    totalExamsTaken++;
    if (bestScore == -1 || correctCount > bestScore) bestScore = correctCount;
    totalCorrect += correctCount;

    updateStatsCards();
    ui->stackedWidget->setCurrentIndex(2);
}

void Home::updateStatsCards() {
    ui->labelCardValue1->setText(QString::number(totalExamsTaken));
    ui->labelCardValue2->setText(bestScore >= 0 ? QString("%1/20").arg(bestScore) : "-");

    if (totalExamsTaken > 0) {
        double avg = (double)totalCorrect / totalExamsTaken;
        ui->labelCardValue3->setText(QString::number(avg, 'f', 1));
    } else {
        ui->labelCardValue3->setText("-");
    }
}

void Home::onRetryClicked() { startExam(); }

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
