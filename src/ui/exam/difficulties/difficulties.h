#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Difficulties; }
QT_END_NAMESPACE

class Difficulties : public QWidget {
    Q_OBJECT

public:
    explicit Difficulties(QWidget *parent = nullptr);
    ~Difficulties();

signals:
    void difficultySelected(int difficulty);

private:
    Ui::Difficulties *ui;
};
