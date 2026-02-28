#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow {
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void loginSuccessful(const QString &username);
    void registerRequested();

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    Ui::Login *ui;
};
