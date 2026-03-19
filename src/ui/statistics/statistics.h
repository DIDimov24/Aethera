#pragma once

#include <QWidget>
#include <QLabel>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class Statistics; }
QT_END_NAMESPACE

class Statistics : public QWidget {
    Q_OBJECT

public:
    Statistics(QWidget *parent = nullptr);
    ~Statistics();
    void refresh();

private:
    Ui::Statistics *ui;

    QChart *highestScoresChart;
    QChart *lowestScoresChart;
    QChart *subjectStatsChart;
    QChart *difficultyStatsChart;

    void loadAndDisplayStatistics();
    void createHighestScoresChart();
    void createLowestScoresChart();
    void createSubjectStatsChart();
    void createDifficultyStatsChart();
};
