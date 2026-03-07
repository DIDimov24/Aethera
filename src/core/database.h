#pragma once

#include <QString>
#include <QSqlDatabase>

struct User {
    QString username;
    QString password;
    QString grade;
};

class Database {
public:
    static Database& instance();

    bool openDatabase();
    bool registerUser(const QString &username, const QString &password, const QString &grade);
    bool validateUser(const QString &username, const QString &password);
    bool userExists(const QString &username);
    bool getUser(const QString &username, User &out);

private:
    Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase database;
};
