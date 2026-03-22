#pragma once

#include <QWidget>
#include <QList>
#include <QPair>

QT_BEGIN_NAMESPACE
namespace Ui { class FlashcardSession; }
QT_END_NAMESPACE

class FlashcardSession : public QWidget {
    Q_OBJECT

public:
    explicit FlashcardSession(QWidget *parent = nullptr);
    ~FlashcardSession();

    void startSession(const QString &subject);

signals:
    void sessionFinished();

private slots:
    void onFlip();
    void onAgain();
    void onGood();
    void onEasy();

private:
    Ui::FlashcardSession *ui;

    QList<QPair<QString, QString>> cards;
    int currentIndex;
    bool flipped;
    int againCount;
    int goodCount;
    int easyCount;

    void loadCard();
    void nextCard();
    void showResults();
    void updateProgress();
};
