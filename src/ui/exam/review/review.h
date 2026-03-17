#pragma once

#include <QWidget>
#include "session.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Review; }
QT_END_NAMESPACE

class Review : public QWidget {
    Q_OBJECT

public:
    explicit Review(QWidget *parent = nullptr);
    ~Review();
    void showReview(const ExamRecord &record);

signals:
    void backToHistoryRequested();

private:
    Ui::Review *ui;
};
