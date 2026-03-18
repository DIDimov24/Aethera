#pragma once

#include <QWidget>
#include <QList>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class History; }
QT_END_NAMESPACE

class History : public QWidget {
    Q_OBJECT

public:
    explicit History(QWidget *parent = nullptr);
    ~History();

    void loadAndPopulate(const QString &username);

signals:
    void newExamRequested();
    void reviewRequested(int attemptId);

private:
    Ui::History *ui;
};
