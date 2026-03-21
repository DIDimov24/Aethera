#include "statistics.h"
#include "ui_statistics.h"
#include "database.h"
#include "utils.h"
#include <QtCharts>
#include <QVBoxLayout>
#include <QLabel>
#include <QColor>
#include <QPainter>
#include <QPen>

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

    ui->averageScoreLabel->setText(Utils::gradeFromScore(averageScore));
    ui->totalStudentsLabel->setText(QString::number(totalStudents));

    createHighestScoresChart();
    createLowestScoresChart();
    createSubjectStatsChart();
    createDifficultyStatsChart();
}

static QChart* makeChart(const QString &title) {
    QChart *chart = new QChart();
    chart->setTitle(title);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundRoundness(0);
    chart->setBackgroundBrush(QColor(255, 255, 255));
    chart->setTitleBrush(QColor(26, 36, 64));
    chart->setContentsMargins(0, 0, 0, 0);
    return chart;
}

static void styleAxes(QChart *chart, QBarSeries *series, const QStringList &cats, const QString &yTitle) {
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(cats);
    axisX->setLabelsColor(QColor(90, 106, 144));
    axisX->setGridLineVisible(false);
    axisX->setLinePen(QPen(QColor(212, 216, 234)));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(2, 6);
    axisY->setTickCount(5);
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText(yTitle);
    axisY->setLabelsColor(QColor(90, 106, 144));
    axisY->setTitleBrush(QColor(90, 106, 144));
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(QPen(QColor(220, 225, 240)));
    axisY->setLinePen(QPen(QColor(212, 216, 234)));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
}

void Statistics::createSubjectStatsChart() {
    QList<CategoryStat> subjectStats = Database::instance().getSubjectStats();

    QBarSet *barSet = new QBarSet("Average Grade");
    barSet->setColor(QColor(79, 111, 196));
    barSet->setBorderColor(QColor(58, 90, 174));

    QStringList categories;
    for (int i = 0; i < subjectStats.size(); i++) {
        *barSet << Utils::gradeFromScore(subjectStats[i].averageScore).toDouble();
        categories << subjectStats[i].name;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    subjectStatsChart = makeChart("Average Grade by Subject");
    subjectStatsChart->addSeries(barSeries);
    styleAxes(subjectStatsChart, barSeries, categories, "Grade");

    ui->subjectChartView->setChart(subjectStatsChart);
    ui->subjectChartView->setRenderHint(QPainter::Antialiasing);
}

void Statistics::createDifficultyStatsChart() {
    QList<CategoryStat> difficultyStats = Database::instance().getDifficultyStats();

    QBarSet *barSet = new QBarSet("Average Grade");
    barSet->setColor(QColor(138, 180, 204));
    barSet->setBorderColor(QColor(79, 111, 196));

    QStringList categories;
    for (int i = 0; i < difficultyStats.size(); i++) {
        *barSet << Utils::gradeFromScore(difficultyStats[i].averageScore).toDouble();
        categories << difficultyStats[i].name;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    difficultyStatsChart = makeChart("Average Grade by Difficulty");
    difficultyStatsChart->addSeries(barSeries);
    styleAxes(difficultyStatsChart, barSeries, categories, "Grade");

    ui->difficultyChartView->setChart(difficultyStatsChart);
    ui->difficultyChartView->setRenderHint(QPainter::Antialiasing);
}

void Statistics::createHighestScoresChart() {
    QList<StudentScore> topScores = Database::instance().getHighestScores(10);

    QBarSet *barSet = new QBarSet("Highest Grade");
    barSet->setColor(QColor(42, 106, 32));
    barSet->setBorderColor(QColor(30, 80, 22));

    QStringList categories;
    for (int i = 0; i < topScores.size(); i++) {
        *barSet << Utils::gradeFromScore(topScores[i].score).toDouble();
        categories << topScores[i].username;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    highestScoresChart = makeChart("Students with Highest Grades");
    highestScoresChart->addSeries(barSeries);
    styleAxes(highestScoresChart, barSeries, categories, "Grade");

    ui->highestChartView->setChart(highestScoresChart);
    ui->highestChartView->setRenderHint(QPainter::Antialiasing);
}

void Statistics::createLowestScoresChart() {
    QList<StudentScore> bottomScores = Database::instance().getLowestScores(10);

    QBarSet *barSet = new QBarSet("Lowest Grade");
    barSet->setColor(QColor(192, 57, 43));
    barSet->setBorderColor(QColor(150, 40, 30));

    QStringList categories;
    for (int i = 0; i < bottomScores.size(); i++) {
        *barSet << Utils::gradeFromScore(bottomScores[i].score).toDouble();
        categories << bottomScores[i].username;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    lowestScoresChart = makeChart("Students with Lowest Grades");
    lowestScoresChart->addSeries(barSeries);
    styleAxes(lowestScoresChart, barSeries, categories, "Grade");

    ui->lowestChartView->setChart(lowestScoresChart);
    ui->lowestChartView->setRenderHint(QPainter::Antialiasing);
}
