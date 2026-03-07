#pragma once

#include <QString>
#include <QSqlDatabase>

struct User {
    QString username;
    QString password;
    QString grade;
    QString bio;
};

class Database {
public:
    static Database& instance();

    bool openDatabase();
    bool registerUser(const QString &username, const QString &password, const QString &grade);
    bool validateUser(const QString &username, const QString &password);
    bool userExists(const QString &username);
    bool getUser(const QString &username, User &out);
    bool updateUsername(const QString &oldUsername, const QString &newUsername);
    bool updatePassword(const QString &username, const QString &newPassword);
    bool updateBio(const QString &username, const QString &bio);
    bool deleteUser(const QString &username);

private:
    Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase database;
};
