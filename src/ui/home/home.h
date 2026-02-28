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
 
private:
    Ui::Home *ui;
    bool sidebarExpanded;
 
    QList<Question> allQuestions;
    QList<Question> examQuestions;
    int currentQuestionIndex;
    int selectedAnswer;
    int correctCount;
    int timeLeft;
    QTimer *examTimer;
 
    int totalExamsTaken;
    int bestScore;
    int totalCorrect;
 
    void loadQuestions();
    void showQuestion(int index);
    void highlightAnswer(int answer);
    void updateTimerLabel();
    void updateStatsCards();
    QString calculateGrade(int correct, int total);
    void resetAnswerButtons();
};
