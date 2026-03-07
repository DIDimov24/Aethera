#include "usersession.h"

UserSession& UserSession::instance() {
    static UserSession instance;
    return instance;
}

void UserSession::setUser(const User &user) {
    _user = user;
}

void UserSession::clear() {
    _user = User();
}

bool UserSession::isLoggedIn() {
    return _user.username != "";
}

QString UserSession::getUsername() {
    return _user.username;
}

QString UserSession::getGrade() {
    return _user.grade;
}
