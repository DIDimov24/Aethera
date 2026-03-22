#include "flashcardsession.h"
#include "ui_flashcardsession.h"
#include <QPushButton>

static QList<QPair<QString, QString>> cardsForSubject(const QString &subject) {
    if (subject == "English") {
        return {
            { "Ephemeral",   "Lasting for a very short time" },
            { "Laconic",     "Using very few words" },
            { "Perfidious",  "Deceitful and untrustworthy" },
            { "Sanguine",    "Optimistic, especially in a difficult situation" }
        };
    }
    if (subject == "Math") {
        return {
            { "Derivative",  "Rate of change of a function" },
            { "Integral",    "Area under a curve" },
            { "Prime",       "Divisible only by 1 and itself" },
            { "Hypotenuse",  "Longest side of a right triangle" }
        };
    }
    return {
        { "Renaissance",  "Cultural rebirth in 14th-17th century Europe" },
        { "Imperialism",  "Policy of extending power over other nations" },
        { "Revolution",   "Fundamental change in power or structure" },
        { "Monarchy",     "Government ruled by a king or queen" }
    };
}

FlashcardSession::FlashcardSession(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FlashcardSession)
    , currentIndex(0)
    , flipped(false)
    , againCount(0)
    , goodCount(0)
    , easyCount(0) {
    ui->setupUi(this);

    connect(ui->buttonFlip,  &QPushButton::clicked, this, &FlashcardSession::onFlip);
    connect(ui->buttonAgain, &QPushButton::clicked, this, &FlashcardSession::onAgain);
    connect(ui->buttonGood,  &QPushButton::clicked, this, &FlashcardSession::onGood);
    connect(ui->buttonEasy,  &QPushButton::clicked, this, &FlashcardSession::onEasy);
    connect(ui->buttonBackToLessons, &QPushButton::clicked, this, &FlashcardSession::sessionFinished);
    connect(ui->buttonStudyAgain, &QPushButton::clicked, this, [this]() {
        startSession(ui->labelSessionTitle->text());
    });
}

FlashcardSession::~FlashcardSession() {
    delete ui;
}

void FlashcardSession::startSession(const QString &subject) {
    cards = cardsForSubject(subject);
    currentIndex = 0;
    flipped = false;
    againCount = 0;
    goodCount = 0;
    easyCount = 0;

    ui->labelSessionTitle->setText(subject);
    ui->stackedWidgetSession->setCurrentIndex(0);
    ui->recallButtons->setVisible(false);
    ui->labelFlipHint->setVisible(true);

    loadCard();
    updateProgress();
}

void FlashcardSession::loadCard() {
    if (currentIndex >= cards.size()) {
        showResults();
        return;
    }

    flipped = false;
    ui->labelCardFront->setText(cards[currentIndex].first);
    ui->labelCardBack->setText(cards[currentIndex].second);
    ui->cardFace->setCurrentIndex(0);
    ui->recallButtons->setVisible(false);
    ui->labelFlipHint->setVisible(true);
    updateProgress();
}

void FlashcardSession::updateProgress() {
    int total = cards.size();
    int current = qMin(currentIndex + 1, total);
    ui->labelProgress->setText(QString("%1 / %2").arg(current).arg(total));
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

void FlashcardSession::onFlip() {
    if (flipped) return;
    flipped = true;
    ui->cardFace->setCurrentIndex(1);
    ui->recallButtons->setVisible(true);
    ui->labelFlipHint->setVisible(false);
}

void FlashcardSession::onAgain() {
    againCount++;
    nextCard();
}

void FlashcardSession::onGood() {
    goodCount++;
    nextCard();
}

void FlashcardSession::onEasy() {
    easyCount++;
    nextCard();
}

void FlashcardSession::nextCard() {
    currentIndex++;
    loadCard();
}

void FlashcardSession::showResults() {
    ui->stackedWidgetSession->setCurrentIndex(1);
    ui->labelResultScore->setText(QString("%1 / %2").arg(goodCount + easyCount).arg(cards.size()));
    ui->labelAgainCount->setText(QString::number(againCount));
    ui->labelGoodCount->setText(QString::number(goodCount));
    ui->labelEasyCount->setText(QString::number(easyCount));
}
