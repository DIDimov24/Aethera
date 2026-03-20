#pragma once

#include <QWidget>
#include <QPixmap>

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

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::Profile *ui;
    QPixmap _normalAvatar;
    QPixmap _hoveredAvatar;

    void applyAvatar(const QString &avatarName);
    QPixmap makeCircularPixmap(const QPixmap &src, int size);
    void randomizeAvatar();
};