#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE

class Settings : public QWidget {
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    void refresh();

signals:
    void accountDeleted();

private:
    Ui::Settings *ui;

    void onSaveUsername();
    void onSavePassword();
    void onSaveGrade();
    void onSaveBio();
    void onDeleteAccountClicked();
};
