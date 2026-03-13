#pragma once

#include <QWidget>
#include <QList>
#include "exam.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ExamHistory; }
QT_END_NAMESPACE

class ExamHistory : public QWidget {
    Q_OBJECT

public:
    explicit ExamHistory(QWidget *parent = nullptr);
    ~ExamHistory();

    void populate(const QList<ExamRecord> &history);

signals:
    void newExamRequested();

private:
    Ui::ExamHistory *ui;
};
