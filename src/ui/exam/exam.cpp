#include "home.h"
#include "ui_home.h"
#include "style.h"
#include <QRandomGenerator>

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
         "753 BC", "509 BC", "264 BC", "44 BC", 0, "History");
    addQ("Who was the first Emperor of China?",
         "Kublai Khan", "Sun Yat-sen", "Qin Shi Huang", "Confucius", 2, "History");
    addQ("Which ancient wonder was located in Alexandria, Egypt?",
         "The Colossus", "The Lighthouse", "The Mausoleum", "The Statue of Zeus", 1, "History");
    addQ("In which century did the fall of the Western Roman Empire occur?",
         "3rd century AD", "4th century AD", "5th century AD", "6th century AD", 2, "History");
    addQ("Who built the Parthenon in Athens?",
         "Spartans", "Macedonians", "Athenians", "Persians", 2, "History");
    addQ("What was the name of Alexander the Great's famous horse?",
         "Pegasus", "Bucephalus", "Arion", "Shadowfax", 1, "History");
    addQ("Which ancient civilization built Machu Picchu?",
         "Aztec", "Maya", "Inca", "Olmec", 2, "History");
    addQ("The Library of Alexandria was located in which modern-day country?",
         "Greece", "Turkey", "Egypt", "Libya", 2, "History");
    addQ("In which year did the Battle of Hastings take place?",
         "1066", "1215", "1348", "1453", 0, "History");
    addQ("What was the Black Death?",
         "A volcanic eruption", "A bubonic plague pandemic", "A series of wars", "A drought", 1, "History");
    addQ("Who signed the Magna Carta in 1215?",
         "Richard I", "Edward I", "Henry III", "King John", 3, "History");
    addQ("The Crusades were primarily fought over which city?",
         "Constantinople", "Jerusalem", "Alexandria", "Rome", 1, "History");
    addQ("Which empire conquered Constantinople in 1453?",
         "Mongol Empire", "Russian Empire", "Ottoman Empire", "Persian Empire", 2, "History");
    addQ("What was the primary language of the Catholic Church in medieval Europe?",
         "Greek", "French", "Latin", "Italian", 2, "History");
    addQ("In which year did World War I begin?",
         "1912", "1914", "1916", "1918", 1, "History");
    addQ("In which year did World War II end?",
         "1943", "1944", "1945", "1946", 2, "History");
    addQ("Who was the first person to walk on the Moon?",
         "Buzz Aldrin", "Yuri Gagarin", "Neil Armstrong", "John Glenn", 2, "History");
    addQ("The Berlin Wall fell in which year?",
         "1987", "1988", "1989", "1991", 2, "History");
    addQ("Which country was the first to grant women the right to vote?",
         "USA", "UK", "France", "New Zealand", 3, "History");
    addQ("Who was the leader of the Soviet Union during World War II?",
         "Lenin", "Trotsky", "Khrushchev", "Stalin", 3, "History");
    addQ("The French Revolution began in which year?",
         "1776", "1789", "1799", "1804", 1, "History");
    addQ("Which war was fought between the North and South of the United States?",
         "The Revolutionary War", "The War of 1812", "The Civil War", "The Mexican-American War", 2, "History");
    addQ("What was the code name for the D-Day invasion?",
         "Operation Barbarossa", "Operation Overlord", "Operation Market Garden", "Operation Torch", 1, "History");
    addQ("Which country did Germany invade to start WWII?",
         "France", "Belgium", "Poland", "Netherlands", 2, "History");
    addQ("Who was the British Prime Minister during most of WWII?",
         "Neville Chamberlain", "Clement Attlee", "Anthony Eden", "Winston Churchill", 3, "History");
    addQ("In which city were the atomic bombs dropped in 1945?",
         "Tokyo and Osaka", "Hiroshima and Nagasaki", "Kyoto and Hiroshima", "Nagasaki and Tokyo", 1, "History");
    addQ("What did 'V-E Day' stand for in WWII?",
         "Victory in Europe", "Victory in East Asia", "Veterans of Europe", "Valor in Europe", 0, "History");
    addQ("Which battle is considered the turning point of WWII on the Eastern Front?",
         "Battle of Kursk", "Battle of Moscow", "Battle of Stalingrad", "Battle of Berlin", 2, "History");
    addQ("The Nuremberg Trials prosecuted leaders of which country?",
         "Japan", "Italy", "Germany", "Austria", 2, "History");
    addQ("What was the Holocaust?",
         "A German military operation", "The genocide of Jews and others by Nazi Germany",
         "A British bombing campaign", "A Soviet offensive", 1, "History");
    addQ("Which country suffered the most military deaths in WWII?",
         "USA", "Germany", "China", "Soviet Union", 3, "History");
}

void Home::startExam() {
    ui->labelTimer->setStyleSheet(Style::timerNormal);
    toggleSidebar(true);

    blinkTimer->stop();
    examTimer->stop();

    currentQuestionIndex = 0;
    selectedAnswer = -1;
    correctCount = 0;
    timeLeft = 600;
    timerBlinkState = true;

    initializeExamQuestions();
    showQuestion(currentQuestionIndex);
    updateProgress();
    updateTimerLabel(QString("⏱ 10:00"), false);

    examTimer->start();
}

void Home::initializeExamQuestions() {
    examQuestions = allQuestions;

    std::shuffle(examQuestions.begin(), examQuestions.end(), std::default_random_engine(QRandomGenerator::global()->generate()));

    while (examQuestions.size() > 20) {
        examQuestions.removeLast();
    }
}

void Home::submitAnswer(int answerIndex) {
    if (selectedAnswer != -1) return;

    selectedAnswer = answerIndex;
    if (answerIndex == examQuestions[currentQuestionIndex].correctAnswer) {
        correctCount++;
    }

    int correct = examQuestions[currentQuestionIndex].correctAnswer;
    highlightAnswer(answerIndex, correct);
    ui->buttonNext->setEnabled(true);
}

void Home::nextQuestion() {
    currentQuestionIndex++;
    selectedAnswer = -1;

    if (currentQuestionIndex >= examQuestions.size()) {
        finishExam();
    } else {
        showQuestion(currentQuestionIndex);
        updateProgress();
    }
}

void Home::finishExam() {
    blinkTimer->stop();
    examTimer->stop();
    recordExamResult();
    if (!sidebarExpanded) toggleSidebar();

    QString color = getGradeColor();
    QString grade = calculateGrade();

    ui->labelScore->setText(QString("%1 / %2").arg(correctCount).arg(examQuestions.size()));
    ui->labelScore->setStyleSheet(
        QString("color: %1; font-size: 42px; font-weight: 800;").arg(color));
    ui->labelGrade->setText("Grade: " + grade);
    ui->labelGrade->setStyleSheet(
        QString("color: %1; font-size: 16px; font-weight: 600;").arg(color));

    updateStatsCards();
    ui->stackedWidget->setCurrentIndex(2);
}

void Home::recordExamResult() {
    totalExamsTaken++;
    if (bestScore == -1 || correctCount > bestScore) {
        bestScore = correctCount;
    }
    totalCorrect += correctCount;

    ExamRecord record;
    record.number = totalExamsTaken;
    record.score = correctCount;
    record.total = examQuestions.size();
    examHistory.append(record);
}

void Home::showQuestion(int index) {
    if (index < 0 || index >= examQuestions.size()) return;

    Question question = examQuestions[currentQuestionIndex];

    ui->labelQuestion->setText(question.text);
    ui->buttonA->setText("A)  " + question.optionA);
    ui->buttonB->setText("B)  " + question.optionB);
    ui->buttonC->setText("C)  " + question.optionC);
    ui->buttonD->setText("D)  " + question.optionD);

    ui->buttonNext->setEnabled(false);
    resetAnswerButtons();
}

void Home::updateProgress() {
    int current = currentQuestionIndex + 1;
    int total = examQuestions.size();
    ui->labelProgress->setText(QString::number(current) + " / " + QString::number(total));
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

void Home::highlightAnswer(int answer, int correct) {
    QPushButton *btns[4] = { ui->buttonA, ui->buttonB, ui->buttonC, ui->buttonD };
    for (int i = 0; i < 4; i++) {
        if (i == correct) btns[i]->setStyleSheet(Style::answerCorrect);
        else if (i == answer && answer != correct) btns[i]->setStyleSheet(Style::answerWrong);
        else btns[i]->setStyleSheet(Style::answerNormal);
    }
}

void Home::updateTimerLabel(QString timeStr, bool isWarning) {
    ui->labelTimer->setText(timeStr);
    if (isWarning) {
        ui->labelTimer->setStyleSheet(Style::timerWarnOn);
    } else {
        ui->labelTimer->setStyleSheet(Style::timerNormal);
    }
}

void Home::onExamTimerTick() {
    timeLeft--;

    int m = timeLeft / 60;
    int s = timeLeft % 60;
    QString timeStr = QString("⏱ %1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
    bool isWarning = timeLeft <= 120;
    updateTimerLabel(timeStr, isWarning);

    if (timeLeft == 120) {
        blinkTimer->start();
    }

    if (timeLeft <= 0) {
        finishExam();
    }
}

void Home::onExamTimerBlink() {
    timerBlinkState = !timerBlinkState;
    if (timerBlinkState) {
        ui->labelTimer->setStyleSheet(Style::timerWarnOn);
    } else {
        ui->labelTimer->setStyleSheet(Style::timerWarnOff);
    }
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

void Home::resetAnswerButtons() {
    ui->buttonA->setStyleSheet(Style::answerNormal);
    ui->buttonB->setStyleSheet(Style::answerNormal);
    ui->buttonC->setStyleSheet(Style::answerNormal);
    ui->buttonD->setStyleSheet(Style::answerNormal);
}

QString Home::calculateGrade() {
    double pct = (double)correctCount / examQuestions.size() * 100.0;
    if (pct >= 90) return "Excellent (6)";
    if (pct >= 75) return "Very Good (5)";
    if (pct >= 60) return "Good (4)";
    if (pct >= 45) return "Average (3)";
    if (pct >= 30) return "Poor (2)";
    return "Very Poor (2-)";
}

QString Home::getGradeColor() {
    double pct = (double)correctCount / examQuestions.size() * 100.0;
    if (pct >= 75) return "#00D4AA";
    if (pct >= 45) return "#F5C518";
    return "#FF6B6B";
}
