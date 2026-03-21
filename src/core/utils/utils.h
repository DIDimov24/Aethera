#pragma once

#include <QString>
#include <QPixmap>

namespace Utils {

QString gradeFromScore(double score);
QString hashPassword(const QString &password);
bool verifyPassword(const QString &password, const QString &stored);
QPixmap createAvatarPixmap(const QString &name, int size);

}
