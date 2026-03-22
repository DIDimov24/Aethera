#pragma once

#include <QWidget>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class Lessons; }
QT_END_NAMESPACE

class Lessons : public QWidget {
    Q_OBJECT

public:
    explicit Lessons(QWidget *parent = nullptr);
    ~Lessons();

    void refresh();

signals:
    void deckSelected(const QString &subject);
    void studyRequested();

private:
    Ui::Lessons *ui;

    void populateDecks();
};