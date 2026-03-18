#pragma once

#include <QString>
#include <QSqlDatabase>
#include <QList>

struct User {
    QString username;
    QString grade;
    QString bio;
};

struct ExamAttempt {
    int id;
    QString subject;
    QString difficulty;
    int score;
    QString completedAt;
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

    bool saveExamAttempt(const QString &username,
                         const QString &subject,
                         const QString &difficulty,
                         int score,
                         const QString &completedAt,
                         const QString &resultsJson);

    QList<ExamAttempt> loadExamAttemptsForUser(const QString &username);

    QString loadExamResultsJson(int attemptId);

private:
    Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase database;
};
