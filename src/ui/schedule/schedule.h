#pragma once

#include <QWidget>
#include <QVBoxLayout>

struct SubjectCard {
    QString title;
    QString teacher;
    QString room;
    QString time;
    bool completed = false;
};

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

    QString buildCardHtml(const SubjectCard &card);
    void addDayCards(QVBoxLayout *layout, const QVector<SubjectCard> &cards);
};
