#include "home.h"
#include "ui_home.h"
#include <QRandomGenerator>
#include <algorithm>

Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Home)
    , sidebarExpanded(true)
    , currentQuestionIndex(0)
    , selectedAnswer(-1)
    , correctCount(0)
    , timeLeft(600)
    , totalExamsTaken(0)
    , bestScore(-1)
    , totalCorrect(0)
{
    ui->setupUi(this);

    examTimer = new QTimer(this);
    examTimer->setInterval(1000);
    connect(examTimer, &QTimer::timeout, this, &Home::onTimerTick);

    connect(ui->buttonToggleSidebar, &QPushButton::clicked, this, &Home::toggleSidebar);

    connect(ui->buttonNavDashboard, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->buttonNavExams, &QPushButton::clicked, this, [this]() {
        startExam();
    });

    connect(ui->buttonStartExam, &QPushButton::clicked, this, &Home::startExam);
    connect(ui->buttonLogOut, &QPushButton::clicked, this, &Home::loggedOut);
    connect(ui->buttonLoginRegister, &QPushButton::clicked, this, &Home::loginRequested);

    connect(ui->buttonA, &QPushButton::clicked, this, [this]() { onAnswerSelected(0); });
    connect(ui->buttonB, &QPushButton::clicked, this, [this]() { onAnswerSelected(1); });
    connect(ui->buttonC, &QPushButton::clicked, this, [this]() { onAnswerSelected(2); });
    connect(ui->buttonD, &QPushButton::clicked, this, [this]() { onAnswerSelected(3); });

    connect(ui->buttonNext, &QPushButton::clicked, this, &Home::onNextClicked);

    connect(ui->buttonRetry,    &QPushButton::clicked, this, &Home::onRetryClicked);
    connect(ui->buttonBackHome, &QPushButton::clicked, this, &Home::onBackHomeClicked);

    loadQuestions();

    updateStatsCards();
}

Home::~Home() {
    delete ui;
}

void Home::setUsername(const QString &username) {
    ui->labelWelcome->setText("Welcome back, " + username + "! Ready to test your knowledge?");
}

void Home::toggleSidebar() {
    sidebarExpanded = !sidebarExpanded;

    int sidebarWidth = sidebarExpanded ? 200 : 50;
    int contentX     = sidebarExpanded ? 200 : 50;
    int contentWidth = 1000 - contentX;

    ui->sidebar->setGeometry(0, 0, sidebarWidth, 750);
    ui->stackedWidget->setGeometry(contentX, 0, contentWidth, 750);

    ui->buttonNavDashboard->setVisible(sidebarExpanded);
    ui->buttonNavExams->setVisible(sidebarExpanded);
    ui->buttonLoginRegister->setVisible(sidebarExpanded);
    ui->buttonSettings->setVisible(sidebarExpanded);
}

void Home::loadQuestions() {
    auto addQ = [this](QString text, QString a, QString b, QString c, QString d, int correct, QString cat) {
        Question q;
        q.text = text;
        q.optionA = a;
        q.optionB = b;
        q.optionC = c;
        q.optionD = d;
        q.correctAnswer = correct;
        q.category = cat;
        allQuestions.append(q);
    };

    addQ("When was the city of Rome traditionally founded?",
         "753 BC", "509 BC", "264 BC", "44 BC", 0, "Ancient");

    addQ("Who was the first Emperor of China?",
         "Kublai Khan", "Sun Yat-sen", "Qin Shi Huang", "Confucius", 2, "Ancient");

    addQ("Which ancient wonder was located in Alexandria, Egypt?",
         "The Colossus", "The Lighthouse", "The Mausoleum", "The Statue of Zeus", 1, "Ancient");

    addQ("In which century did the fall of the Western Roman Empire occur?",
         "3rd century AD", "4th century AD", "5th century AD", "6th century AD", 2, "Ancient");

    addQ("Who built the Parthenon in Athens?",
         "Spartans", "Macedonians", "Athenians", "Persians", 2, "Ancient");

    addQ("What was the name of Alexander the Great's famous horse?",
         "Pegasus", "Bucephalus", "Arion", "Shadowfax", 1, "Ancient");

    addQ("Which ancient civilization built Machu Picchu?",
         "Aztec", "Maya", "Inca", "Olmec", 2, "Ancient");

    addQ("The Library of Alexandria was located in which modern-day country?",
         "Greece", "Turkey", "Egypt", "Libya", 2, "Ancient");

    addQ("In which year did the Battle of Hastings take place?",
         "1066", "1215", "1348", "1453", 0, "Middle Ages");

    addQ("What was the Black Death?",
         "A volcanic eruption", "A bubonic plague pandemic", "A series of wars", "A drought", 1, "Middle Ages");

    addQ("Who signed the Magna Carta in 1215?",
         "Richard I", "Edward I", "Henry III", "King John", 3, "Middle Ages");

    addQ("The Crusades were primarily fought over which city?",
         "Constantinople", "Jerusalem", "Alexandria", "Rome", 1, "Middle Ages");

    addQ("Which empire conquered Constantinople in 1453?",
         "Mongol Empire", "Russian Empire", "Ottoman Empire", "Persian Empire", 2, "Middle Ages");

    addQ("What was the primary language of the Catholic Church in medieval Europe?",
         "Greek", "French", "Latin", "Italian", 2, "Middle Ages");

    addQ("In which year did World War I begin?",
         "1912", "1914", "1916", "1918", 1, "Modern");

    addQ("In which year did World War II end?",
         "1943", "1944", "1945", "1946", 2, "Modern");

    addQ("Who was the first person to walk on the Moon?",
         "Buzz Aldrin", "Yuri Gagarin", "Neil Armstrong", "John Glenn", 2, "Modern");

    addQ("The Berlin Wall fell in which year?",
         "1987", "1988", "1989", "1991", 2, "Modern");

    addQ("Which country was the first to grant women the right to vote?",
         "USA", "UK", "France", "New Zealand", 3, "Modern");

    addQ("Who was the leader of the Soviet Union during World War II?",
         "Lenin", "Trotsky", "Khrushchev", "Stalin", 3, "Modern");

    addQ("The French Revolution began in which year?",
         "1776", "1789", "1799", "1804", 1, "Modern");

    addQ("Which war was fought between the North and South of the United States?",
         "The Revolutionary War", "The War of 1812", "The Civil War", "The Mexican-American War", 2, "Modern");

    addQ("What was the code name for the D-Day invasion?",
         "Operation Barbarossa", "Operation Overlord", "Operation Market Garden", "Operation Torch", 1, "WWII");

    addQ("Which country did Germany invade to start WWII?",
         "France", "Belgium", "Poland", "Netherlands", 2, "WWII");

    addQ("Who was the British Prime Minister during most of WWII?",
         "Neville Chamberlain", "Clement Attlee", "Anthony Eden", "Winston Churchill", 3, "WWII");

    addQ("In which city were the atomic bombs dropped in 1945?",
         "Tokyo and Osaka", "Hiroshima and Nagasaki", "Kyoto and Hiroshima", "Nagasaki and Tokyo", 1, "WWII");

    addQ("What did 'V-E Day' stand for in WWII?",
         "Victory in Europe", "Victory in East Asia", "Veterans of Europe", "Valor in Europe", 0, "WWII");

    addQ("Which battle is considered the turning point of WWII on the Eastern Front?",
         "Battle of Kursk", "Battle of Moscow", "Battle of Stalingrad", "Battle of Berlin", 2, "WWII");

    addQ("The Nuremberg Trials prosecuted leaders of which country?",
         "Japan", "Italy", "Germany", "Austria", 2, "WWII");

    addQ("What was the Holocaust?",
         "A German military operation", "The genocide of Jews and others by Nazi Germany",
         "A British bombing campaign", "A Soviet offensive", 1, "WWII");

    addQ("Which country suffered the most military deaths in WWII?",
         "USA", "Germany", "China", "Soviet Union", 3, "WWII");
}

void Home::startExam() {
    examTimer->stop();

    currentQuestionIndex = 0;
    selectedAnswer = -1;
    correctCount = 0;
    timeLeft = 600;

    examQuestions = allQuestions;
    std::shuffle(examQuestions.begin(), examQuestions.end(),
                 std::default_random_engine(QRandomGenerator::global()->generate()));

    while (examQuestions.size() > 20) {
        examQuestions.removeLast();
    }

    ui->stackedWidget->setCurrentIndex(1);

    showQuestion(0);

    examTimer->start();
    updateTimerLabel();
}

void Home::showQuestion(int index) {
    if (index >= examQuestions.size()) return;

    Question &q = examQuestions[index];

    ui->labelQuestion->setText(q.text);
    ui->buttonA->setText("A)  " + q.optionA);
    ui->buttonB->setText("B)  " + q.optionB);
    ui->buttonC->setText("C)  " + q.optionC);
    ui->buttonD->setText("D)  " + q.optionD);

    ui->labelProgress->setText(
        QString("Question %1 of %2").arg(index + 1).arg(examQuestions.size())
        );
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

    QString rightStyle = "background-color: #1a3a2a; border: 1px solid #00D4AA; color: #00D4AA; "
                         "font-size: 14px; border-radius: 8px; padding: 14px 20px; text-align: left;";
    QString wrongStyle = "background-color: #3a1a1a; border: 1px solid #FF6B6B; color: #FF6B6B; "
                         "font-size: 14px; border-radius: 8px; padding: 14px 20px; text-align: left;";
    QString normalStyle = "background-color: #272541; color: #D4D5D9; font-size: 14px; "
                          "border: 1px solid #3D3B5E; border-radius: 8px; padding: 14px 20px; text-align: left;";

    QPushButton *buttons[4] = {
        ui->buttonA, ui->buttonB, ui->buttonC, ui->buttonD
    };

    for (int i = 0; i < 4; i++) {
        if (i == correct) {
            buttons[i]->setStyleSheet(rightStyle);
        } else if (i == answer && answer != correct) {
            buttons[i]->setStyleSheet(wrongStyle);
        } else {
            buttons[i]->setStyleSheet(normalStyle);
        }
    }

    if (answer == correct) {
        correctCount++;
    }
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

    if (timeLeft <= 0) {
        examTimer->stop();
        showResults();
    }
}

void Home::updateTimerLabel() {
    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;

    QString timeText = QString("⏱ %1:%2")
                           .arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'));

    ui->labelTimer->setText(timeText);

    if (timeLeft <= 120) {
        ui->labelTimer->setStyleSheet("color: #FF6B6B; font-size: 22px; font-weight: bold;");
    }
}

void Home::showResults() {
    examTimer->stop();

    QString grade = calculateGrade(correctCount, examQuestions.size());

    ui->labelScore->setText(QString("%1 / %2").arg(correctCount).arg(examQuestions.size()));
    ui->labelGrade->setText("Grade: " + grade);

    totalExamsTaken++;

    if (bestScore == -1 || correctCount > bestScore) {
        bestScore = correctCount;
    }

    totalCorrect += correctCount;

    updateStatsCards();

    ui->stackedWidget->setCurrentIndex(2);
}

QString Home::calculateGrade(int correct, int total) {
    double percent = (double)correct / total * 100.0;

    if (percent >= 90) return "Excellent (6)";
    if (percent >= 75) return "Very Good (5)";
    if (percent >= 60) return "Good (4)";
    if (percent >= 45) return "Average (3)";
    if (percent >= 30) return "Poor (2)";
    return "Very Poor (2-)";
}

void Home::updateStatsCards() {

    ui->labelCardValue1->setText(QString::number(totalExamsTaken));

    if (bestScore >= 0) {
        ui->labelCardValue2->setText(QString("%1/20").arg(bestScore));
    } else {
        ui->labelCardValue2->setText("—");
    }

    if (totalExamsTaken > 0) {
        double avg = (double)totalCorrect / totalExamsTaken;
        ui->labelCardValue3->setText(QString::number(avg, 'f', 1));
    } else {
        ui->labelCardValue3->setText("—");
    }
}

void Home::onRetryClicked() {
    startExam();
}

void Home::onBackHomeClicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void Home::resetAnswerButtons() {
    QString normalStyle = "background-color: #272541; color: #D4D5D9; font-size: 14px; "
                          "border: 1px solid #3D3B5E; border-radius: 8px; padding: 14px 20px; text-align: left;";

    ui->buttonA->setStyleSheet(normalStyle);
    ui->buttonB->setStyleSheet(normalStyle);
    ui->buttonC->setStyleSheet(normalStyle);
    ui->buttonD->setStyleSheet(normalStyle);
}
