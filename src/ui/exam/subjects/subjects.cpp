#include "subjects.h"
#include "ui_subjects.h"

Subjects::Subjects(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Subjects) {
    ui->setupUi(this);

    connect(ui->buttonEnglish, &QPushButton::clicked, this, [this]() { emit subjectSelected("English"); });
    connect(ui->buttonMath, &QPushButton::clicked, this, [this]() { emit subjectSelected("Math"); });
    connect(ui->buttonHistory, &QPushButton::clicked, this, [this]() { emit subjectSelected("History"); });
}

Subjects::~Subjects() {
    delete ui;
}
