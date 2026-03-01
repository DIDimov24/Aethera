#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class Home; }
QT_END_NAMESPACE

struct Question {
    QString text;
    QString optionA;
    QString optionB;
    QString optionC;
    QString optionD;
    int correctAnswer;
    QString category;
};

class Home : public QMainWindow {
    Q_OBJECT

public:
    Home(QWidget *parent = nullptr);
    ~Home();

private slots:
    void toggleSidebar();
    void startExam();
    void onAnswerSelected(int answer);
    void onNextClicked();
    void onTimerTick();
    void showResults();
    void onRetryClicked();
    void onBackHomeClicked();
    void onLogoutClicked();
    void onTimerBlink();

private:
    Ui::Home *ui;
    bool sidebarExpanded;
    bool timerBlinkState;

    QList<Question> allQuestions;
    QList<Question> examQuestions;
    int currentQuestionIndex;
    int selectedAnswer;
    int correctCount;
    int timeLeft;
    QTimer *examTimer;
    QTimer *blinkTimer;

    int totalExamsTaken;
    int bestScore;
    int totalCorrect;
    int activeNavIndex;

    void loadQuestions();
    void showQuestion(int index);
    void highlightAnswer(int answer);
    void updateTimerLabel();
    void updateStatsCards();
    void updateSidebarButtons();
    void repositionSidebarButtons();
    void setNavActive(int index);
    QString calculateGrade(int correct, int total);
    QString gradeColor(int correct, int total);
    void resetAnswerButtons();
};
