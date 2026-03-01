#pragma once
 
#include "database.h"
 
class UserSession {
public:
    static UserSession& instance() {
        static UserSession session;
        return session;
    }
 
    void setUser(const User &user) {
        m_user = user;
    }
 
    void clear() {
        m_user = User();
    }

    bool isLoggedIn() {
        return m_user.username != "";
    }
 
    QString username() const { return m_user.username; }
    QString grade() const { return m_user.grade; }
 
private:
    UserSession() {}
 
    User m_user;
};
 
