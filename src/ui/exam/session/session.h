#pragma once

#include <QWidget>
#include <QTimer>
#include <QString>
#include <QList>
#include "questionsBank.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Session; }
QT_END_NAMESPACE

class Session : public QWidget {
    Q_OBJECT

public:
    explicit Session(QWidget *parent = nullptr);
    ~Session();

    void startExam(const QString &subject, const QString &difficulty);

    int getTotalExamsTaken() { return totalExamsTaken; }
    int getBestScore() { return bestScore; }
    int getTotalCorrect() { return totalCorrect; }
    QString getCurrentSubject() { return currentSubject; }
    QString getCurrentDifficulty() { return currentDifficulty; }

signals:
    void examCompleted(int score, int total);
    void sidebarToggleRequested(bool hidden);

private slots:
    void onExamTimerTick();
    void onExamTimerBlink();

private:
    Ui::Session *ui;

    QList<ExamQuestion> examQuestions;
    int currentQuestionIndex;
    int selectedAnswer;
    int correctCount;
    int timeLeft;
    bool timerBlinkState;
    int totalExamsTaken;
    int bestScore;
    int totalCorrect;
    QString currentSubject;
    QString currentDifficulty;
    QList<int> selectedAnswers;
    QTimer *examTimer;
    QTimer *blinkTimer;

    void initializeExamQuestions();
    void submitAnswer(int answerIndex);
    void nextQuestion();
    void finishExam();
    void recordExamResult();
    void showQuestion(int index);
    void updateProgress();
    void highlightAnswer(int answer, int correct);
    void updateTimerLabel(QString timeStr, bool isWarning);
    void resetAnswerButtons();
};
