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
    QString getBio();

    void setUsername(const QString &username);
    void setGrade(const QString &grade);
    void setBio(const QString &bio);

private:
    User _user;
};