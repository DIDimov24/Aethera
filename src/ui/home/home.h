#pragma once

#include <QMainWindow>
#include <QString>
#include "profile.h"
#include "exam.h"
#include "settings.h"
#include "examhistory.h"

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
    Exam *examPage;
    ExamHistory *examHistoryPage;

    bool sidebarExpanded;
    int activeNavIndex;

    void updateSidebarButtons();
    void repositionSidebarButtons();
    void setNavActive(int index);

    void updateStatsCards();
};
