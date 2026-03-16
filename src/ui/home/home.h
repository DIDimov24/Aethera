#pragma once

#include <QMainWindow>
#include <QString>
#include "profile.h"
#include "session.h"
#include "results.h"
#include "difficulties.h"
#include "subjects.h"
#include "history.h"
#include "settings.h"

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

private:
    Ui::Home *ui;

    Profile *profilePage;
    Settings *settingsPage;
    Session *sessionPage;
    Results *resultsPage;
    Difficulties *difficultiesPage;
    Subjects *subjectsPage;
    History *historyPage;

    bool sidebarExpanded;
    int activeNavIndex;

    void updateSidebarButtons();
    void repositionSidebarButtons();
    void setNavActive(int index);

    void updateStatsCards();
};
