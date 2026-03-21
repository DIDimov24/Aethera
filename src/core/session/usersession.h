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
    QString getAvatar();

    void setUsername(const QString &username);
    void setGrade(const QString &grade);
    void setBio(const QString &bio);
    void setAvatar(const QString &avatar);

private:
    User _user;
};