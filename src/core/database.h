#pragma once

#include <QString>
#include <QMap>

class UserStore {
public:
    static UserStore& instance() {
        static UserStore store;
        return store;
    }

    bool registerUser(const QString &username, const QString &password) {
        if (m_users.contains(username)) return false;
        m_users.insert(username, password);
        return true;
    }

    bool validateUser(const QString &username, const QString &password) {
        if (!m_users.contains(username)) return false;
        return m_users.value(username) == password;
    }

    bool userExists(const QString &username) {
        return m_users.contains(username);
    }

private:
    UserStore() {}
    QMap<QString, QString> m_users;
};
