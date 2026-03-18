#include "session.h"
#include "ui_session.h"
#include "style.h"
#include "database.h"
#include "usersession.h"
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <algorithm>
#include <random>

Session::Session(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Session) {
    ui->setupUi(this);

    currentQuestionIndex = 0;
    selectedAnswer = -1;
    correctCount = 0;
    timeLeft = 600;
    timerBlinkState = true;
    totalExamsTaken = 0;
    bestScore = -1;
    totalCorrect = 0;
    currentDifficulty = "Beginner";

    examTimer = new QTimer(this);
    examTimer->setInterval(1000);
    connect(examTimer, &QTimer::timeout, this, &Session::onExamTimerTick);

    blinkTimer = new QTimer(this);
    blinkTimer->setInterval(500);
    connect(blinkTimer, &QTimer::timeout, this, &Session::onExamTimerBlink);

    connect(ui->buttonA, &QPushButton::clicked, this, [this]() { submitAnswer(0); });
    connect(ui->buttonB, &QPushButton::clicked, this, [this]() { submitAnswer(1); });
    connect(ui->buttonC, &QPushButton::clicked, this, [this]() { submitAnswer(2); });
    connect(ui->buttonD, &QPushButton::clicked, this, [this]() { submitAnswer(3); });

    connect(ui->buttonNext, &QPushButton::clicked, this, &Session::nextQuestion);
}

Session::~Session() {
    delete ui;
}

void Session::startExam(const QString &subject, const QString &difficulty) {
    currentSubject = subject;
    currentDifficulty = difficulty;

    ui->labelTimer->setStyleSheet(Style::timerNormal);
    emit sidebarToggleRequested(true);

    blinkTimer->stop();
    examTimer->stop();

    currentQuestionIndex = 0;
    selectedAnswer = -1;
    correctCount = 0;
    timeLeft = 600;
    timerBlinkState = true;

    initializeExamQuestions();
    ui->labelExamTitle->setText(QString("%1 • %2").arg(currentSubject, currentDifficulty));

    if (examQuestions.isEmpty()) {
        emit sidebarToggleRequested(false);
        emit examCompleted(0, 0);
        return;
    }

    selectedAnswers = QList<int>(examQuestions.size(), -1);

    showQuestion(currentQuestionIndex);
    updateProgress();
    updateTimerLabel(QString("10:00"), false);

    examTimer->start();
}

void Session::initializeExamQuestions() {
    QList<ExamQuestion> pool;
    for (int i = 0; i < QUESTIONS_BANK.size(); i++) {
        const ExamQuestion &question = QUESTIONS_BANK[i];
        if (question.subject == currentSubject && question.difficulty == currentDifficulty) {
            pool.append(question);
        }
    }

    std::shuffle(
        pool.begin(),
        pool.end(),
        std::default_random_engine(QRandomGenerator::global()->generate())
    );

    examQuestions = pool;
}

void Session::submitAnswer(int answerIndex) {
    if (selectedAnswer != -1) return;

    selectedAnswer = answerIndex;
    selectedAnswers[currentQuestionIndex] = answerIndex;
    if (answerIndex == examQuestions[currentQuestionIndex].correctAnswer) {
        correctCount++;
    }

    int correct = examQuestions[currentQuestionIndex].correctAnswer;
    highlightAnswer(answerIndex, correct);
    ui->buttonNext->setEnabled(true);
}

void Session::nextQuestion() {
    if (selectedAnswer == -1) {
        return;
    }

    currentQuestionIndex++;
    selectedAnswer = -1;

    if (currentQuestionIndex >= examQuestions.size()) {
        finishExam();
    } else {
        showQuestion(currentQuestionIndex);
        updateProgress();
    }
}

void Session::finishExam() {
    blinkTimer->stop();
    examTimer->stop();
    recordExamResult();

    emit sidebarToggleRequested(false);
    emit examCompleted(correctCount, examQuestions.size());
}

void Session::recordExamResult() {
    totalExamsTaken++;
    if (bestScore == -1 || correctCount > bestScore) {
        bestScore = correctCount;
    }
    totalCorrect += correctCount;

    QJsonArray resultsJson;
    for (int i = 0; i < examQuestions.size(); i++) {
        ExamQuestion question = examQuestions[i];
        int selectedAnswer = selectedAnswers[i];

        QJsonObject qObj;
        qObj["index"] = i;
        qObj["selected"] = selectedAnswer;
        qObj["correct"] = question.correctAnswer;
        qObj["is_correct"] = (selectedAnswer == question.correctAnswer) ? 1 : 0;
        qObj["question_text"] = question.text;
        resultsJson.append(qObj);
    }

    QString username = UserSession::instance().getUsername();
    QString jsonStr = QJsonDocument(resultsJson).toJson(QJsonDocument::Compact);
    QString completedAtStr = QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm");
    Database::instance().saveExamAttempt(username, currentSubject, currentDifficulty, correctCount, completedAtStr, jsonStr);
}

void Session::showQuestion(int index) {
    if (index < 0 || index >= examQuestions.size()) return;

    const ExamQuestion &question = examQuestions[index];

    ui->labelQuestion->setText(question.text);
    ui->buttonA->setText("A)  " + question.optionA);
    ui->buttonB->setText("B)  " + question.optionB);
    ui->buttonC->setText("C)  " + question.optionC);
    ui->buttonD->setText("D)  " + question.optionD);

    ui->buttonNext->setEnabled(false);
    resetAnswerButtons();
}

void Session::updateProgress() {
    int current = currentQuestionIndex + 1;
    int total = examQuestions.size();
    ui->labelProgress->setText(QString::number(current) + " / " + QString::number(total));
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

void Session::highlightAnswer(int answer, int correct) {
    QPushButton *btns[4] = { ui->buttonA, ui->buttonB, ui->buttonC, ui->buttonD };
    for (int i = 0; i < 4; i++) {
        if (i == correct) btns[i]->setStyleSheet(Style::answerCorrect);
        else if (i == answer && answer != correct) btns[i]->setStyleSheet(Style::answerWrong);
        else btns[i]->setStyleSheet(Style::answerNormal);
    }
}

void Session::updateTimerLabel(QString timeStr, bool isWarning) {
    ui->labelTimer->setText(timeStr);
    if (isWarning) {
        ui->labelTimer->setStyleSheet(Style::timerWarnOn);
    } else {
        ui->labelTimer->setStyleSheet(Style::timerNormal);
    }
}

void Session::onExamTimerTick() {
    timeLeft--;

    int m = timeLeft / 60;
    int s = timeLeft % 60;
    QString timeStr = QString("%1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
    bool isWarning = timeLeft <= 120;
    updateTimerLabel(timeStr, isWarning);

    if (timeLeft == 120) {
        blinkTimer->start();
    }

    if (timeLeft <= 0) {
        finishExam();
    }
}

void Session::onExamTimerBlink() {
    timerBlinkState = !timerBlinkState;
    if (timerBlinkState) {
        ui->labelTimer->setStyleSheet(Style::timerWarnOn);
    } else {
        ui->labelTimer->setStyleSheet(Style::timerWarnOff);
    }
}

void Session::resetAnswerButtons() {
    ui->buttonA->setStyleSheet(Style::answerNormal);
    ui->buttonB->setStyleSheet(Style::answerNormal);
    ui->buttonC->setStyleSheet(Style::answerNormal);
    ui->buttonD->setStyleSheet(Style::answerNormal);
}
