#include "database.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Database::Database() {
    database = QSqlDatabase::addDatabase("QSQLITE");

    QString sourcePath = QString(__FILE__);
    QFileInfo sourceInfo(sourcePath);

    QDir dir = sourceInfo.absoluteDir();
    dir.cdUp();
    QString databasePath = dir.absoluteFilePath("assets/aethera.db");

    QFileInfo dbInfo(databasePath);
    if (dbInfo.exists()) {
        database.setDatabaseName(databasePath);
        qDebug() << "Using database at:" << databasePath;

        if (!database.open()) {
            qDebug() << "Error opening database:" << database.lastError().text();
        }
    } else {
        qDebug() << "Could not find aethera.db at:" << databasePath;
    }
}

Database& Database::instance() {
    static Database instance;
    return instance;
}

bool Database::openDatabase() {
    if (!database.isOpen()) {
        if (!database.open()) {
            qDebug() << "Failed to open database: " << database.lastError().text();
            return false;
        }
    }
    return true;
}

bool Database::registerUser(const QString &username, const QString &password, const QString &grade) {
    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare("INSERT INTO users (username, password, grade) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(grade);
    return query.exec();
}

bool Database::validateUser(const QString &username, const QString &password) {
    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec() || !query.next()) {
        return false;
    }

    return query.value(0).toString() == password;
}

bool Database::userExists(const QString &username) {
    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare("SELECT 1 FROM users WHERE username = ?");
    query.addBindValue(username);
    return query.exec() && query.next();
}

bool Database::getUser(const QString &username, User &user) {
    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare("SELECT username, password, grade FROM users WHERE username = ?");
    query.addBindValue(username);
    if (!query.exec() || !query.next()) {
        return false;
    }

    user.username = query.value(0).toString();
    user.password = query.value(1).toString();
    user.grade = query.value(2).toString();
    return true;
}
