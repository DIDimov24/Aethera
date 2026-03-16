#include "subjects.h"
#include "ui_subjects.h"

Subjects::Subjects(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Subjects) {
    ui->setupUi(this);
    connect(ui->buttonHistory, &QPushButton::clicked, this, [this]() { emit subjectSelected("History"); });
}

Subjects::~Subjects() {
    delete ui;
}
