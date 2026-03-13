#pragma once

#include <QWidget>
#include <QTimer>
#include <QString>
#include <QList>

struct Question {
    QString text;
    QString optionA;
    QString optionB;
    QString optionC;
    QString optionD;
    int correctAnswer;
    QString category;
};


struct ExamRecord {
    int number;
    int score;
    int total;
};

QT_BEGIN_NAMESPACE
namespace Ui { class Exam; }
QT_END_NAMESPACE

class Exam : public QWidget {
    Q_OBJECT

public:
    explicit Exam(QWidget *parent = nullptr);
    ~Exam();

    void startExam();
    void updateStatsCards(int totalExamsTaken, int bestScore, int totalCorrect);
    void populateExamHistory(const QList<ExamRecord> &history);
    
    // Getters for stats that Home needs
    int getTotalExamsTaken() const { return totalExamsTaken; }
    int getBestScore() const { return bestScore; }
    int getTotalCorrect() const { return totalCorrect; }
    const QList<ExamRecord>& getExamHistory() const { return examHistory; }

signals:
    void examFinished();
    void backToHomeRequested();
    void viewExamsRequested();
    void sidebarToggleRequested(bool hidden);

private slots:
    void onExamTimerTick();
    void onExamTimerBlink();

private:
    Ui::Exam *ui;

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
    QList<ExamRecord> examHistory;
    QTimer *examTimer;
    QTimer *blinkTimer;

    void loadQuestions();
    void initializeExamQuestions();
    void submitAnswer(int answerIndex);
    void nextQuestion();
    void skipExam();
    void finishExam();
    void recordExamResult();
    void showQuestion(int index);
    void updateProgress();
    void highlightAnswer(int answer, int correct);
    void updateTimerLabel(QString timeStr, bool isWarning);
    void resetAnswerButtons();
    QString calculateGrade();
    QString getGradeColor();
};
