#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Schedule; }
QT_END_NAMESPACE

class Schedule : public QWidget {
    Q_OBJECT

public:
    explicit Schedule(QWidget *parent = nullptr);
    ~Schedule();

private:
    Ui::Schedule *ui;
};
