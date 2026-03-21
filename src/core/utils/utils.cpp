#include "utils.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QRandomGenerator>
#include <QStringList>
#include <QPainter>
#include <QColor>
#include <QFont>
#include <QGuiApplication>
#include <QScreen>
#include <QtGlobal>

namespace Utils {

QString gradeFromScore(double score) {
    double grade = 2.0 + (score / 20.0) * 4.0;
    if (grade < 2.0) grade = 2.0;
    if (grade > 6.0) grade = 6.0;
    return QString::number(grade, 'f', 2);
}

QString hashPassword(const QString &password) {
    static constexpr int iterations = 100000;
    static constexpr int keyLength = 32;
    static constexpr int saltLength = 16;

    QByteArray salt(saltLength, '\0');
    for (int i = 0; i < saltLength; i++) {
        salt[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    }

    QByteArray key = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        password.toUtf8(),
        salt,
        iterations,
        keyLength
    );

    return QString("%1:%2:%3")
        .arg(iterations)
        .arg(QString::fromLatin1(salt.toHex()))
        .arg(QString::fromLatin1(key.toHex()));
}

bool verifyPassword(const QString &password, const QString &stored) {
    static constexpr int keyLength = 32;

    QStringList parts = stored.split(':');
    if (parts.size() != 3) return false;

    int iterations = parts[0].toInt();
    QByteArray salt = QByteArray::fromHex(parts[1].toLatin1());
    QByteArray expected = QByteArray::fromHex(parts[2].toLatin1());

    QByteArray actual = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        password.toUtf8(),
        salt,
        iterations,
        keyLength
    );

    return actual == expected;
}

QPixmap createAvatarPixmap(const QString &name, int size) {
    int logicalSize = size > 0 ? size : 1;
    qreal dpr = 1.0;
    if (QGuiApplication::primaryScreen()) {
        dpr = QGuiApplication::primaryScreen()->devicePixelRatio();
    }

    int renderSize = qRound(logicalSize * dpr);
    if (renderSize < 1) renderSize = 1;

    QPixmap pixmap(renderSize, renderSize);
    pixmap.setDevicePixelRatio(dpr);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    painter.setBrush(QColor("#4f6fc4"));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, logicalSize, logicalSize);

    QString letter = name.isEmpty() ? "?" : name.left(1).toUpper();
    painter.setPen(QColor("#ffffff"));
    QFont font("Arial", logicalSize / 3, QFont::Bold);
    font.setStyleStrategy(QFont::PreferAntialias);
    painter.setFont(font);
    painter.drawText(0, 0, logicalSize, logicalSize, Qt::AlignCenter | Qt::AlignVCenter, letter);
    painter.end();

    return pixmap;
}

}
