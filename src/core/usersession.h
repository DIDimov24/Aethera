#pragma once

#include "database.h"

class UserSession {
public:
    static UserSession& instance();

    void setUser(const User &user);
    void clear();
    bool isLoggedIn();
    QString getUsername();
    QString getGrade();

private:
    User _user;
};
