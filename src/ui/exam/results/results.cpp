#include "results.h"
#include "ui_results.h"

Results::Results(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Results) {
    ui->setupUi(this);
    connect(ui->buttonBackHome, &QPushButton::clicked, this, &Results::backToHomeRequested);
    connect(ui->buttonViewExams, &QPushButton::clicked, this, &Results::viewExamsRequested);
}

Results::~Results() {
    delete ui;
}

void Results::displayResults(int score, int total) {
    QString color = getGradeColor(score, total);
    QString grade = calculateGrade(score, total);

    ui->labelScore->setText(QString("%1 / %2").arg(score).arg(total));
    ui->labelScore->setStyleSheet(QString("color: %1; font-size: 42px; font-weight: 800;").arg(color));
    ui->labelGrade->setText("Grade: " + grade);
    ui->labelGrade->setStyleSheet(QString("color: %1; font-size: 16px; font-weight: 600;").arg(color));
}

QString Results::calculateGrade(int score, int total) {
    double pct = (double)score / total * 100.0;
    if (pct >= 90) return "Excellent (6)";
    if (pct >= 75) return "Very Good (5)";
    if (pct >= 60) return "Good (4)";
    if (pct >= 45) return "Average (3)";
    if (pct >= 30) return "Poor (2)";
    return "Very Poor (2-)";
}

QString Results::getGradeColor(int score, int total) {
    double pct = (double)score / total * 100.0;
    if (pct >= 75) return "#00D4AA";
    if (pct >= 45) return "#F5C518";
    return "#FF6B6B";
}
