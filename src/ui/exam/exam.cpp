#include "exam.h"
#include <QRandomGenerator>
#include <algorithm>

Exam::Exam(QObject *parent)
    : QObject(parent) {
    totalExamsTaken = 0;
    bestScore = -1;
    totalCorrect = 0;

    examTimer = new QTimer(this);
    examTimer->setInterval(1000);
    connect(examTimer, &QTimer::timeout, this, &Exam::onTimerTick);

    blinkTimer = new QTimer(this);
    blinkTimer->setInterval(500);
    connect(blinkTimer, &QTimer::timeout, this, &Exam::onTimerBlink);

    loadQuestions();
}

Exam::~Exam() {}

void Exam::loadQuestions() {
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

void Exam::startExam() {
    blinkTimer->stop();
    examTimer->stop();

    currentQuestionIndex = 0;
    selectedAnswer = -1;
    correctCount = 0;
    timeLeft = 600;
    timerBlinkState = true;

    initializeExamQuestions();

    emit questionChanged(currentQuestionIndex);
    emit progressUpdated(currentQuestionIndex + 1, getTotalQuestions());
    emit timerUpdated(QString("⏱ 10:00"), false);

    examTimer->start();
}

void Exam::reset() {
    currentQuestionIndex = 0;
    selectedAnswer = -1;
    correctCount = 0;
    timeLeft = 600;
}

Question& Exam::getCurrentQuestion() {
    return examQuestions[currentQuestionIndex];
}

int Exam::getCurrentQuestionIndex() {
    return currentQuestionIndex;
}

int Exam::getTotalQuestions() {
    return examQuestions.size();
}

QString Exam::getProgressText() {
    return QString("Question %1 of %2").arg(currentQuestionIndex + 1).arg(examQuestions.size());
}

int Exam::getProgressPercentage() {
    if (examQuestions.size() == 0) return 0;
    return currentQuestionIndex + 1;
}

void Exam::submitAnswer(int answerIndex) {
    if (selectedAnswer != -1) return;

    selectedAnswer = answerIndex;
    if (answerIndex == examQuestions[currentQuestionIndex].correctAnswer) {
        correctCount++;
    }

    int correct = examQuestions[currentQuestionIndex].correctAnswer;
    emit answerSubmitted(answerIndex, correct);
}

bool Exam::isAnswerSelected() {
    return selectedAnswer != -1;
}

int Exam::getSelectedAnswer() {
    return selectedAnswer;
}

int Exam::getCorrectAnswer() {
    return examQuestions[currentQuestionIndex].correctAnswer;
}

void Exam::nextQuestion() {
    currentQuestionIndex++;
    selectedAnswer = -1;

    if (!hasNextQuestion()) {
        blinkTimer->stop();
        examTimer->stop();
        recordExamResult();
        emit examFinished();
    } else {
        emit questionChanged(currentQuestionIndex);
        emit progressUpdated(currentQuestionIndex + 1, getTotalQuestions());
    }
}

bool Exam::hasNextQuestion() {
    return currentQuestionIndex + 1 < examQuestions.size();
}

int Exam::getTimeLeft() {
    return timeLeft;
}

bool Exam::isTimeExpired() {
    return timeLeft <= 0;
}

void Exam::onTimerTick() {
    timeLeft--;
    
    int m = timeLeft / 60;
    int s = timeLeft % 60;
    QString timeStr = QString("⏱ %1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
    bool isWarning = timeLeft <= 120;
    emit timerUpdated(timeStr, isWarning);
    
    if (timeLeft == 120) {
        blinkTimer->start();
        emit timerBlinkStateChanged(true);
    }
    
    if (timeLeft <= 0) {
        blinkTimer->stop();
        examTimer->stop();
        recordExamResult();
        emit examFinished();
    }
}

void Exam::onTimerBlink() {
    timerBlinkState = !timerBlinkState;
    emit timerBlinkStateChanged(timerBlinkState);
}

void Exam::retry() {
    startExam();
}

int Exam::getCorrectCount() {
    return correctCount;
}

QString Exam::calculateGrade() {
    double pct = (double)correctCount / examQuestions.size() * 100.0;
    if (pct >= 90) return "Excellent (6)";
    if (pct >= 75) return "Very Good (5)";
    if (pct >= 60) return "Good (4)";
    if (pct >= 45) return "Average (3)";
    if (pct >= 30) return "Poor (2)";
    return "Very Poor (2-)";
}

QString Exam::getGradeColor() {
    double pct = (double)correctCount / examQuestions.size() * 100.0;
    if (pct >= 75) return "#00D4AA";
    if (pct >= 45) return "#F5C518";
    return "#FF6B6B";
}

double Exam::getAverageScore() {
    if (totalExamsTaken == 0) return 0.0;
    return (double)totalCorrect / totalExamsTaken;
}

int Exam::getTotalExamsTaken() {
    return totalExamsTaken;
}

int Exam::getBestScore() {
    return bestScore;
}

int Exam::getTotalCorrect() {
    return totalCorrect;
}

void Exam::recordExamResult() {
    totalExamsTaken++;
    if (bestScore == -1 || correctCount > bestScore) {
        bestScore = correctCount;
    }
    totalCorrect += correctCount;
}

void Exam::initializeExamQuestions() {
    examQuestions = allQuestions;

    // seed a default_random_engine with a 32-bit value from
    // Qt's global QRandomGenerator. This gives an independent
    // seed value so that shuffle produces a randomized order each run.
    std::shuffle(examQuestions.begin(), examQuestions.end(),
                 std::default_random_engine(QRandomGenerator::global()->generate()));

    while (examQuestions.size() > 20) {
        examQuestions.removeLast();
    }
}
