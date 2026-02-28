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

signals:
    void registerSuccessful(const QString &username);
    void backToLoginRequested();

private slots:
    void onCreateClicked();
    void onBackClicked();

private:
    Ui::Register *ui;
};
