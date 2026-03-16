#include "difficulties.h"
#include "ui_difficulties.h"

Difficulties::Difficulties(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Difficulties) {
    ui->setupUi(this);
    connect(ui->buttonEasy,   &QPushButton::clicked, this, [this]() { emit difficultySelected(0); });
    connect(ui->buttonMedium, &QPushButton::clicked, this, [this]() { emit difficultySelected(1); });
    connect(ui->buttonHard,   &QPushButton::clicked, this, [this]() { emit difficultySelected(2); });
}

Difficulties::~Difficulties() {
    delete ui;
}
