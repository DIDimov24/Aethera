#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Results; }
QT_END_NAMESPACE

class Results : public QWidget {
    Q_OBJECT

public:
    explicit Results(QWidget *parent = nullptr);
    ~Results();

    void displayResults(int score, int total);

signals:
    void backToHomeRequested();
    void viewExamsRequested();

private:
    Ui::Results *ui;

    QString calculateGrade(int score, int total);
    QString getGradeColor(int score, int total);
};
