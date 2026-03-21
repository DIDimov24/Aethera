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

QT_BEGIN_NAMESPACE
namespace Ui { class Home; }
QT_END_NAMESPACE

enum class NavPage {
    Home = 0,
    Exams = 1,
    Settings = 2,
    Profile = 3,
    Statistics = 4
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

    bool sidebarExpanded;
    NavPage activeNavIndex;
    QString selectedExamSubject;

    void updateSidebarButtons();
    void repositionSidebarButtons();
    void setNavActive(NavPage index);

    void updateStatsCards();

    void showAccountRequired();
};