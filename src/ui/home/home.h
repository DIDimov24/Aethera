#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QList>
#include "question.h"

struct ExamRecord {
    int number;
    int score;
    int total;
};

QT_BEGIN_NAMESPACE
namespace Ui { class Home; }
QT_END_NAMESPACE

class Home : public QMainWindow {
    Q_OBJECT

public:
    Home(QWidget *parent = nullptr);
    ~Home();

private slots:
    void toggleSidebar(bool hidden = false);
    void onLogoutClicked();
    void onExamTimerTick();
    void onExamTimerBlink();

private:
    Ui::Home *ui;
    bool sidebarExpanded;
    int activeNavIndex;

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

    void updateSidebarButtons();
    void repositionSidebarButtons();
    void setNavActive(int index);

    void loadQuestions();
    void startExam();
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
    void updateStatsCards();
    void resetAnswerButtons();
    QString calculateGrade();
    QString getGradeColor();

    void initSettingsPage();
    void onSaveUsername();
    void onSavePassword();
    void onSaveBio();
    void onDeleteAccountClicked();

    void populateExamHistory();
};
