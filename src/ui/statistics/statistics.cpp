#include "statistics.h"
#include "ui_statistics.h"
#include "database.h"
#include <QtCharts>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QDebug>

Statistics::Statistics(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Statistics)
{
    ui->setupUi(this);
    loadAndDisplayStatistics();
}

Statistics::~Statistics() {
    delete ui;
}

void Statistics::refresh() {
    loadAndDisplayStatistics();
}

void Statistics::loadAndDisplayStatistics() {
    float averageScore = Database::instance().getAverageScore();
    int totalStudents = Database::instance().getTotalEvaluatedStudents();

    ui->averageScoreLabel->setText(QString::number(averageScore, 'f', 1));
    ui->totalStudentsLabel->setText(QString::number(totalStudents));

    createHighestScoresChart();
    createLowestScoresChart();
    createSubjectStatsChart();
    createDifficultyStatsChart();
}

void Statistics::createSubjectStatsChart() {
    QList<CategoryStat> subjectStats = Database::instance().getSubjectStats();

    QBarSet *barSet = new QBarSet("Average Score");
    barSet->setColor(QColor(0, 120, 215));

    QStringList categories;
    for (int i = 0; i < subjectStats.size(); i++) {
        *barSet << subjectStats[i].averageScore;
        categories << subjectStats[i].name;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    subjectStatsChart = new QChart();
    subjectStatsChart->addSeries(barSeries);
    subjectStatsChart->setTitle("Average Score by Subject");
    subjectStatsChart->setAnimationOptions(QChart::SeriesAnimations);
    subjectStatsChart->setBackgroundRoundness(0);
    subjectStatsChart->setBackgroundBrush(QColor(28, 28, 31));
    subjectStatsChart->setTitleBrush(QColor(224, 224, 228));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(QColor(224, 224, 228));
    axisX->setGridLineVisible(false);
    subjectStatsChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Average Score");
    axisY->setLabelsColor(QColor(224, 224, 228));
    axisY->setTitleBrush(QColor(224, 224, 228));
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(QPen(QColor(42, 42, 45)));
    subjectStatsChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    subjectStatsChart->legend()->setVisible(true);
    subjectStatsChart->legend()->setAlignment(Qt::AlignBottom);
    subjectStatsChart->legend()->setLabelColor(QColor(224, 224, 228));

    ui->subjectChartView->setChart(subjectStatsChart);
    ui->subjectChartView->setRenderHint(QPainter::Antialiasing);
}

void Statistics::createDifficultyStatsChart() {
    QList<CategoryStat> difficultyStats = Database::instance().getDifficultyStats();

    QBarSet *barSet = new QBarSet("Average Score");
    barSet->setColor(QColor(255, 140, 0));

    QStringList categories;
    for (int i = 0; i < difficultyStats.size(); i++) {
        *barSet << difficultyStats[i].averageScore;
        categories << difficultyStats[i].name;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    difficultyStatsChart = new QChart();
    difficultyStatsChart->addSeries(barSeries);
    difficultyStatsChart->setTitle("Average Score by Difficulty");
    difficultyStatsChart->setAnimationOptions(QChart::SeriesAnimations);
    difficultyStatsChart->setBackgroundRoundness(0);
    difficultyStatsChart->setBackgroundBrush(QColor(28, 28, 31));
    difficultyStatsChart->setTitleBrush(QColor(224, 224, 228));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(QColor(224, 224, 228));
    axisX->setGridLineVisible(false);
    difficultyStatsChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Average Score");
    axisY->setLabelsColor(QColor(224, 224, 228));
    axisY->setTitleBrush(QColor(224, 224, 228));
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(QPen(QColor(42, 42, 45)));
    difficultyStatsChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    difficultyStatsChart->legend()->setVisible(true);
    difficultyStatsChart->legend()->setAlignment(Qt::AlignBottom);
    difficultyStatsChart->legend()->setLabelColor(QColor(224, 224, 228));

    ui->difficultyChartView->setChart(difficultyStatsChart);
    ui->difficultyChartView->setRenderHint(QPainter::Antialiasing);
}

void Statistics::createHighestScoresChart() {
    QList<StudentScore> topScores = Database::instance().getHighestScores(10);

    QBarSet *barSet = new QBarSet("Highest Score");
    barSet->setColor(QColor(34, 139, 34));

    QStringList categories;
    for (int i = 0; i < topScores.size(); i++) {
        *barSet << topScores[i].score;
        categories << topScores[i].username;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    highestScoresChart = new QChart();
    highestScoresChart->addSeries(barSeries);
    highestScoresChart->setTitle("Students with Highest Scores");
    highestScoresChart->setAnimationOptions(QChart::SeriesAnimations);
    highestScoresChart->setBackgroundRoundness(0);
    highestScoresChart->setBackgroundBrush(QColor(28, 28, 31));
    highestScoresChart->setTitleBrush(QColor(224, 224, 228));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(QColor(224, 224, 228));
    axisX->setGridLineVisible(false);
    highestScoresChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Score");
    axisY->setLabelsColor(QColor(224, 224, 228));
    axisY->setTitleBrush(QColor(224, 224, 228));
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(QPen(QColor(42, 42, 45)));
    highestScoresChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    highestScoresChart->legend()->setVisible(true);
    highestScoresChart->legend()->setAlignment(Qt::AlignBottom);
    highestScoresChart->legend()->setLabelColor(QColor(224, 224, 228));

    ui->highestChartView->setChart(highestScoresChart);
    ui->highestChartView->setRenderHint(QPainter::Antialiasing);
}

void Statistics::createLowestScoresChart() {
    QList<StudentScore> bottomScores = Database::instance().getLowestScores(10);

    QBarSet *barSet = new QBarSet("Lowest Score");
    barSet->setColor(QColor(220, 20, 60));

    QStringList categories;
    for (int i = 0; i < bottomScores.size(); i++) {
        *barSet << bottomScores[i].score;
        categories << bottomScores[i].username;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    lowestScoresChart = new QChart();
    lowestScoresChart->addSeries(barSeries);
    lowestScoresChart->setTitle("Students with Lowest Scores");
    lowestScoresChart->setAnimationOptions(QChart::SeriesAnimations);
    lowestScoresChart->setBackgroundRoundness(0);
    lowestScoresChart->setBackgroundBrush(QColor(28, 28, 31));
    lowestScoresChart->setTitleBrush(QColor(224, 224, 228));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(QColor(224, 224, 228));
    axisX->setGridLineVisible(false);
    lowestScoresChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Score");
    axisY->setLabelsColor(QColor(224, 224, 228));
    axisY->setTitleBrush(QColor(224, 224, 228));
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(QPen(QColor(42, 42, 45)));
    lowestScoresChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    lowestScoresChart->legend()->setVisible(true);
    lowestScoresChart->legend()->setAlignment(Qt::AlignBottom);
    lowestScoresChart->legend()->setLabelColor(QColor(224, 224, 228));

    ui->lowestChartView->setChart(lowestScoresChart);
    ui->lowestChartView->setRenderHint(QPainter::Antialiasing);
}
