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

signals:
    void logoutRequested();

private:
    Ui::Profile *ui;
};