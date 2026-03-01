#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public QMainWindow {
    Q_OBJECT

public:
    Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void onCreateClicked();
    void onBackClicked();
    void onBackHomeClicked();

private:
    Ui::Register *ui;
};
