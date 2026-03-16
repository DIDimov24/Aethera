#pragma once

#include <QWidget>
#include <QList>
#include "session.h"

QT_BEGIN_NAMESPACE
namespace Ui { class History; }
QT_END_NAMESPACE

class History : public QWidget {
    Q_OBJECT

public:
    explicit History(QWidget *parent = nullptr);
    ~History();

    void populate(const QList<ExamRecord> &history);

signals:
    void newExamRequested();

private:
    Ui::History *ui;
};
