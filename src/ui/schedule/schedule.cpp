#include "schedule.h"
#include "ui_schedule.h"

Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Schedule) {

    ui->setupUi(this);
}

Schedule::~Schedule() {
    delete ui;
}
