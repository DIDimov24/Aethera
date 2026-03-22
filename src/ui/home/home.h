#pragma once

#include <QMainWindow>
#include <QString>
#include <QPixmap>
#include "profile.h"
#include "session.h"
#include "results.h"
#include "difficulties.h"
#include "subjects.h"
#include "history.h"
#include "review.h"
#include "settings.h"
#include "statistics.h"
#include "inbox.h"
#include "schedule.h"
#include "lessons.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Home; }
QT_END_NAMESPACE

enum class NavPage {
    Home = 0,
    Exams = 1,
    Statistics = 2,
    Lessons = 3,
    Schedule = 4,
    Inbox = 5,
    Settings = 6,
    Profile = 7
};

class Home : public QMainWindow {
    Q_OBJECT

public:
    Home(QWidget *parent = nullptr);
    ~Home();

private slots:
    void toggleSidebar(bool hidden = false);
    void onLogoutClicked();

private:
    Ui::Home *ui;

    Profile *profilePage;
    Settings *settingsPage;
    Session *sessionPage;
    Results *resultsPage;
    Difficulties *difficultiesPage;
    Subjects *subjectsPage;
    History *historyPage;
    Review *reviewPage;
    Statistics *statisticsPage;
    Inbox *inboxPage;
    Schedule *schedulePage;
    Lessons *lessonsPage;

    bool sidebarExpanded;
    NavPage activeNavIndex;
    QString selectedExamSubject;

    void updateSidebarButtons();
    void repositionSidebarButtons();
    void setNavActive(NavPage index);

    void updateStatsCards();

    void showAccountRequired();
};