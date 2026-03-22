#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Lessons; }
QT_END_NAMESPACE

class Lessons : public QWidget {
    Q_OBJECT

public:
    explicit Lessons(QWidget *parent = nullptr);
    ~Lessons();

private:
    Ui::Lessons *ui;
};
