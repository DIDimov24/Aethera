#include <QApplication>
#include "ui/login/login.h"
#include "ui/login/register.h"
#include "ui/home/home.h"
#include "core/usersession.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Login    *loginWindow    = new Login();
    Register *registerWindow = new Register();
    Home     *homeWindow     = new Home();

    QObject::connect(loginWindow, &Login::loginSuccessful,
                     [&](const QString &username) {
                         UserSession::instance().setUser(username);
                         homeWindow->setUsername(username);
                         loginWindow->hide();
                         homeWindow->show();
                     });

    QObject::connect(loginWindow, &Login::registerRequested,
                     [&]() {
                         loginWindow->hide();
                         registerWindow->show();
                     });

    QObject::connect(registerWindow, &Register::registerSuccessful,
                     [&](const QString &username) {
                         Q_UNUSED(username);
                         registerWindow->hide();
                         loginWindow->show();
                     });

    QObject::connect(registerWindow, &Register::backToLoginRequested,
                     [&]() {
                         registerWindow->hide();
                         loginWindow->show();
                     });

    QObject::connect(homeWindow, &Home::loggedOut,
                     [&]() {
                         UserSession::instance().clear();
                         homeWindow->hide();
                         loginWindow->show();
                     });

    QObject::connect(homeWindow, &Home::loginRequested,
                     [&]() {
                         homeWindow->hide();
                         loginWindow->show();
                     });

    homeWindow->show();

    return app.exec();
}
