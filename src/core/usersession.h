#pragma once

#include <QString>

class UserSession {
public:
    static UserSession& instance() {
        static UserSession session;
        return session;
    }

    void setUser(const QString &username) {
        m_username = username;
        m_isLoggedIn = true;
    }

    void clear() {
        m_username = "";
        m_isLoggedIn = false;
    }

    QString username() const  { return m_username; }
    bool isLoggedIn() const   { return m_isLoggedIn; }

private:
    UserSession() : m_isLoggedIn(false) {}

    QString m_username;
    bool m_isLoggedIn;
};
