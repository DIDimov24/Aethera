#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Inbox; }
QT_END_NAMESPACE

class Inbox : public QWidget {
    Q_OBJECT

public:
    explicit Inbox(QWidget *parent = nullptr);
    ~Inbox();

private:
    Ui::Inbox *ui;
};
