#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Subjects; }
QT_END_NAMESPACE

class Subjects : public QWidget {
    Q_OBJECT

public:
    explicit Subjects(QWidget *parent = nullptr);
    ~Subjects();

signals:
    void subjectSelected(QString subject);

private:
    Ui::Subjects *ui;
};
