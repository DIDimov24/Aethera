#include "database.h"
#include "passwordhasher.h"
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
    dir.cdUp();
    QString databasePath = dir.absoluteFilePath("assets/aethera.db");

    QFileInfo dbInfo(databasePath);
    if (dbInfo.exists()) {
        database.setDatabaseName(databasePath);
        qDebug() << "Using database at:" << databasePath;

        if (!database.open()) {
            qDebug() << "Error opening database:" << database.lastError().text();
        } else {
            QSqlQuery migration(database);
            migration.exec("ALTER TABLE users ADD COLUMN avatar TEXT DEFAULT ''");
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

bool Database::registerUser(const QString &username, const QString &password, const QString &grade, const QString &avatar) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("INSERT INTO users (username, password, grade, avatar) VALUES (?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(PasswordHasher::hash(password));
    query.addBindValue(grade);
    query.addBindValue(avatar);
    return query.exec();
}

bool Database::validateUser(const QString &username, const QString &password) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec() || !query.next()) return false;

    return PasswordHasher::verify(password, query.value(0).toString());
}

bool Database::userExists(const QString &username) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("SELECT 1 FROM users WHERE username = ?");
    query.addBindValue(username);
    return query.exec() && query.next();
}

bool Database::getUser(const QString &username, User &user) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("SELECT username, grade, bio, avatar FROM users WHERE username = ?");
    query.addBindValue(username);
    if (!query.exec() || !query.next()) return false;

    user.username = query.value(0).toString();
    user.grade    = query.value(1).toString();
    user.bio      = query.value(2).toString();
    user.avatar   = query.value(3).toString();
    return true;
}

bool Database::updateUsername(const QString &oldUsername, const QString &newUsername) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("UPDATE users SET username = ? WHERE username = ?");
    query.addBindValue(newUsername);
    query.addBindValue(oldUsername);
    return query.exec();
}

bool Database::updatePassword(const QString &username, const QString &newPassword) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("UPDATE users SET password = ? WHERE username = ?");
    query.addBindValue(PasswordHasher::hash(newPassword));
    query.addBindValue(username);
    return query.exec();
}

bool Database::updateBio(const QString &username, const QString &bio) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("UPDATE users SET bio = ? WHERE username = ?");
    query.addBindValue(bio);
    query.addBindValue(username);
    return query.exec();
}

bool Database::updateAvatar(const QString &username, const QString &avatar) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("UPDATE users SET avatar = ? WHERE username = ?");
    query.addBindValue(avatar);
    query.addBindValue(username);
    return query.exec();
}

bool Database::deleteUser(const QString &username) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("DELETE FROM users WHERE username = ?");
    query.addBindValue(username);
    return query.exec();
}

bool Database::saveExamAttempt(const QString &username,
                               const QString &subject,
                               const QString &difficulty,
                               int score,
                               const QString &completedAt,
                               const QString &resultsJson) {
    if (!openDatabase()) return false;

    QSqlQuery query(database);
    query.prepare("INSERT INTO exam_attempts (username, subject, difficulty, score, completed_at, results_json) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(subject);
    query.addBindValue(difficulty);
    query.addBindValue(score);
    query.addBindValue(completedAt);
    query.addBindValue(resultsJson);

    return query.exec();
}

QList<ExamAttempt> Database::loadExamAttemptsForUser(const QString &username) {
    QList<ExamAttempt> attempts;
    if (!openDatabase()) return attempts;

    QSqlQuery query(database);
    query.prepare("SELECT id, subject, difficulty, score, completed_at FROM exam_attempts WHERE username = ? ORDER BY datetime(completed_at) DESC");
    query.addBindValue(username);

    if (!query.exec()) return attempts;

    while (query.next()) {
        attempts.append({
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toInt(),
            query.value(4).toString()
        });
    }

    return attempts;
}

QString Database::loadExamResultsJson(int attemptId) {
    if (!openDatabase()) return "";

    QSqlQuery query(database);
    query.prepare("SELECT results_json FROM exam_attempts WHERE id = ?");
    query.addBindValue(attemptId);

    if (!query.exec() || !query.next()) return "";

    return query.value(0).toString();
}

QList<StudentScore> Database::getHighestScores(int limit) {
    QList<StudentScore> scores;
    if (!openDatabase()) return scores;

    QSqlQuery query(database);
    query.prepare("SELECT username, MAX(score) as max_score FROM exam_attempts GROUP BY username ORDER BY max_score DESC LIMIT ?");
    query.addBindValue(limit);

    if (!query.exec()) return scores;

    while (query.next()) {
        scores.append({
            query.value(0).toString(),
            query.value(1).toInt()
        });
    }

    return scores;
}

QList<StudentScore> Database::getLowestScores(int limit) {
    QList<StudentScore> scores;
    if (!openDatabase()) return scores;

    QSqlQuery query(database);
    query.prepare("SELECT username, MIN(score) as min_score FROM exam_attempts GROUP BY username ORDER BY min_score ASC LIMIT ?");
    query.addBindValue(limit);

    if (!query.exec()) return scores;

    while (query.next()) {
        scores.append({
            query.value(0).toString(),
            query.value(1).toInt()
        });
    }

    return scores;
}

float Database::getAverageScore() {
    if (!openDatabase()) return 0.0f;

    QSqlQuery query(database);
    query.prepare("SELECT AVG(score) FROM exam_attempts");

    if (!query.exec() || !query.next()) return 0.0f;

    return query.value(0).toFloat();
}

int Database::getTotalEvaluatedStudents() {
    if (!openDatabase()) return 0;

    QSqlQuery query(database);
    query.prepare("SELECT COUNT(DISTINCT username) FROM exam_attempts");

    if (!query.exec() || !query.next()) return 0;

    return query.value(0).toInt();
}

QList<CategoryStat> Database::getSubjectStats() {
    QList<CategoryStat> stats;
    if (!openDatabase()) return stats;

    QSqlQuery query(database);
    query.prepare("SELECT subject, AVG(score) FROM exam_attempts GROUP BY subject ORDER BY AVG(score) DESC");

    if (!query.exec()) return stats;

    while (query.next()) {
        CategoryStat stat;
        stat.name = query.value(0).toString();
        stat.averageScore = query.value(1).toFloat();
        stats.append(stat);
    }

    return stats;
}

QList<CategoryStat> Database::getDifficultyStats() {
    QList<CategoryStat> stats;
    if (!openDatabase()) return stats;

    QSqlQuery query(database);
    query.prepare("SELECT difficulty, AVG(score) FROM exam_attempts GROUP BY difficulty ORDER BY AVG(score) DESC");

    if (!query.exec()) return stats;

    while (query.next()) {
        CategoryStat stat;
        stat.name = query.value(0).toString();
        stat.averageScore = query.value(1).toFloat();
        stats.append(stat);
    }

    return stats;
}