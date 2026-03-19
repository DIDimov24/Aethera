#pragma once

#include <QWidget>

namespace Ui {
class Profile;
}

class Profile : public QWidget {
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

    void refresh();

signals:
    void logoutRequested();
    void editProfileRequested();

private:
    Ui::Profile *ui;
    QStringList avatarPaths;
    int currentAvatarIndex;
};