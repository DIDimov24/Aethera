#pragma once

#include <QString>

class PasswordHasher {
public:
    static QString hash(const QString &password);
    static bool verify(const QString &password, const QString &stored);
};
