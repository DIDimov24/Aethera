#pragma once

#include <QMainWindow>
#include <QTimer>
#include "exam.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Home; }
QT_END_NAMESPACE

class Home : public QMainWindow {
    Q_OBJECT

public:
    Home(QWidget *parent = nullptr);
    ~Home();

private slots:
    void toggleSidebar();
    void onStartExamClicked();
    void onExamQuestionChanged(int index);
    void onProgressUpdated(int current, int total);
    void onExamAnswerSubmitted(int selected, int correct);
    void onExamTimerUpdated(QString timeStr, bool isWarning);
    void onExamTimerBlinked(bool blinking);
    void onExamFinished();
    void onBackHomeClicked();
    void onLogoutClicked();
    void onRetryExamClicked();

private:
    Ui::Home *ui;
    Exam *examLogic;
    bool sidebarExpanded;

    int activeNavIndex;

    void showQuestion(int index);
    void highlightAnswer(int answer, int correct);
    void updateTimerLabel(QString timeStr, bool isWarning);
    void updateStatsCards();
    void updateSidebarButtons();
    void repositionSidebarButtons();
    void setNavActive(int index);
    void resetAnswerButtons();
};
