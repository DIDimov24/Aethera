#include "passwordhasher.h"
#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QRandomGenerator>
#include <QStringList>

static constexpr int ITERATIONS  = 100000;
static constexpr int KEY_LENGTH  = 32;
static constexpr int SALT_LENGTH = 16;

QString PasswordHasher::hash(const QString &password) {
    QByteArray salt(SALT_LENGTH, '\0');

    for (int i = 0; i < SALT_LENGTH; i++) {
        salt[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    }

    QByteArray key = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        password.toUtf8(),
        salt,
        ITERATIONS,
        KEY_LENGTH
    );

    return QString("%1:%2:%3")
        .arg(ITERATIONS)
        .arg(QString::fromLatin1(salt.toHex()))
        .arg(QString::fromLatin1(key.toHex()));
}

bool PasswordHasher::verify(const QString &password, const QString &stored) {
    QStringList parts = stored.split(':');
    if (parts.size() != 3) return false;

    const int iterations = parts[0].toInt();
    const QByteArray salt = QByteArray::fromHex(parts[1].toLatin1());
    const QByteArray expected = QByteArray::fromHex(parts[2].toLatin1());

    const QByteArray actual = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        password.toUtf8(),
        salt,
        iterations,
        KEY_LENGTH
    );

    return actual == expected;
}
