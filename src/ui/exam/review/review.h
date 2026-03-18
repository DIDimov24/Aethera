#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Review; }
QT_END_NAMESPACE

class Review : public QWidget {
    Q_OBJECT

public:
    explicit Review(QWidget *parent = nullptr);
    ~Review();

    void loadAndShow(int attemptId, const QString &subject, const QString &difficulty, int score);

signals:
    void backToHistoryRequested();

private:
    Ui::Review *ui;
};
