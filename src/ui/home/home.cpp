#include "./home.h"
#include "./ui_home.h"

Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Home)
    , sidebarExpanded(true) {
    ui->setupUi(this);

    connect(ui->buttonToggleSidebar, &QPushButton::clicked, this, &Home::toggleSidebar);

    connect(ui->buttonNavDashboard, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->buttonNavExams, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
}

Home::~Home() {
    delete ui;
}

void Home::toggleSidebar() {
    sidebarExpanded = !sidebarExpanded;

    int sidebarWidth = sidebarExpanded ? 200 : 50;
    int contentX = sidebarExpanded ? 200 : 50;
    int contentWidth = 1000 - contentX;

    ui->sidebar->setGeometry(0, 0, sidebarWidth, 750);
    ui->stackedWidget->setGeometry(contentX, 0, contentWidth, 750);

    ui->buttonNavDashboard->setVisible(sidebarExpanded);
    ui->buttonNavExams->setVisible(sidebarExpanded);
    ui->buttonSettings->setVisible(sidebarExpanded);
}
