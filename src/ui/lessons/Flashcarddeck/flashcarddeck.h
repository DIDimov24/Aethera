#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class FlashcardDeck; }
QT_END_NAMESPACE

class FlashcardDeck : public QWidget {
    Q_OBJECT

public:
    explicit FlashcardDeck(QWidget *parent = nullptr);
    ~FlashcardDeck();

    void loadDeck(const QString &subject);

signals:
    void backRequested();

private slots:
    void onAddCard();

private:
    Ui::FlashcardDeck *ui;

    QString currentSubject;

    void populateCards();
};
