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
 
private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onBackHomeClicked();
 
private:
    Ui::Login *ui;
};
