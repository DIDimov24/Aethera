#pragma once

#include <QString>
#include <QList>
#include <QTimer>

struct Question {
    QString text;
    QString optionA;
    QString optionB;
    QString optionC;
    QString optionD;
    int correctAnswer;
    QString category;
};

class Exam : public QObject {
    Q_OBJECT

public:
    Exam(QObject *parent = nullptr);
    ~Exam();

    void loadQuestions();

    Question& getCurrentQuestion();
    int getCurrentQuestionIndex();
    int getTotalQuestions();
    QString getProgressText();
    int getProgressPercentage();

    bool isAnswerSelected();
    int getSelectedAnswer();
    int getCorrectAnswer();
    bool hasNextQuestion();

    int getTimeLeft();
    bool isTimeExpired();

    int getCorrectCount();
    QString calculateGrade();
    QString getGradeColor();
    double getAverageScore();

    int getTotalExamsTaken();
    int getBestScore();
    int getTotalCorrect();

public slots:
    void startExam();
    void reset();
    void submitAnswer(int answerIndex);
    void nextQuestion();
    void retry();
    void onTimerTick();
    void onTimerBlink();

signals:
    void questionChanged(int index);
    void timerUpdated(QString timeStr, bool isWarning);
    void timerBlinkStateChanged(bool blinking);
    void examFinished();
    void progressUpdated(int current, int total);
    void answerSubmitted(int selected, int correct);

private:
    QList<Question> allQuestions;
    QList<Question> examQuestions;

    int currentQuestionIndex;
    int selectedAnswer;
    int correctCount;
    int timeLeft;
    bool timerBlinkState;

    int totalExamsTaken;
    int bestScore;
    int totalCorrect;

    QTimer *examTimer;
    QTimer *blinkTimer;

    void initializeExamQuestions();
    void recordExamResult();
};
