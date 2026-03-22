#include "lessons.h"
#include "ui_lessons.h"

Lessons::Lessons(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Lessons) {

    ui->setupUi(this);
}

Lessons::~Lessons() {
    delete ui;
}
