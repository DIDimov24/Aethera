#pragma once

#include <QString>
#include <QSqlDatabase>
#include <QList>
#include <QMap>

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

struct StudentScore {
    QString username;
    int score;
};

struct CategoryStat {
    QString name;
    float averageScore;
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
    bool updateGrade(const QString &username, const QString &grade);
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

    QList<StudentScore> getHighestScores(int limit = 10);
    QList<StudentScore> getLowestScores(int limit = 10);
    float getAverageScore();
    int getTotalEvaluatedStudents();
    QList<CategoryStat> getSubjectStats();
    QList<CategoryStat> getDifficultyStats();

private:
    Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase database;
};