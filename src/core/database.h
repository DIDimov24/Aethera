#pragma once
 
#include <QString>
#include <QMap>
 
struct User {
    QString username;
    QString password;
    QString grade;
};
 
class UserStore {
public:
    static UserStore& instance() {
        static UserStore store;
        return store;
    }
 
    bool registerUser(const QString &username, const QString &password, const QString &grade) {
        if (m_users.contains(username)) return false;
        User u;
        u.username = username;
        u.password = password;
        u.grade = grade;
        m_users.insert(username, u);
        return true;
    }
 
    bool validateUser(const QString &username, const QString &password) const {
        if (!m_users.contains(username)) return false;
        return m_users.value(username).password == password;
    }
 
    bool userExists(const QString &username) const {
        return m_users.contains(username);
    }
 
    bool getUser(const QString &username, User &out) const {
        if (!m_users.contains(username)) return false;
        out = m_users.value(username);
        return true;
    }
 
private:
    UserStore() {}
    QMap<QString, User> m_users;
};
 