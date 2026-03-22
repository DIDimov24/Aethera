#include "flashcarddeck.h"
#include "ui_flashcarddeck.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QFrame>

struct CardEntry {
    QString front;
    QString back;
};

static QList<CardEntry> cardsForSubject(const QString &subject) {
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

FlashcardDeck::FlashcardDeck(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FlashcardDeck) {
    ui->setupUi(this);

    connect(ui->buttonBack, &QPushButton::clicked, this, &FlashcardDeck::backRequested);
    connect(ui->buttonAddCard, &QPushButton::clicked, this, &FlashcardDeck::onAddCard);
}

FlashcardDeck::~FlashcardDeck() {
    delete ui;
}

void FlashcardDeck::loadDeck(const QString &subject) {
    currentSubject = subject;
    ui->labelDeckTitle->setText(subject);
    populateCards();
}

void FlashcardDeck::populateCards() {
    while (QLayoutItem *item = ui->layoutCardsContent->takeAt(0)) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    QList<CardEntry> cards = cardsForSubject(currentSubject);

    for (int i = 0; i < cards.size(); i++) {
        const CardEntry &entry = cards[i];

        QFrame *row = new QFrame;
        row->setObjectName("cardRow");
        row->setMinimumHeight(56);
        row->setMaximumHeight(56);
        row->setStyleSheet(R"(
            QFrame#cardRow {
                background-color: #ffffff;
                border: 1px solid #d4d8ea;
                border-radius: 10px;
            }
        )");

        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(16, 0, 16, 0);
        rowLayout->setSpacing(14);

        QLabel *frontLabel = new QLabel(entry.front);
        frontLabel->setStyleSheet("color: #1a2440; font-size: 13px; font-weight: 600;");

        QLabel *backLabel = new QLabel(entry.back);
        backLabel->setStyleSheet("color: #8898c0; font-size: 12px;");

        QLabel *arrowLabel = new QLabel("›");
        arrowLabel->setStyleSheet("color: #c0c8e0; font-size: 18px;");
        arrowLabel->setFixedWidth(16);

        rowLayout->addWidget(frontLabel, 1);
        rowLayout->addWidget(backLabel, 2);
        rowLayout->addWidget(arrowLabel);

        ui->layoutCardsContent->addWidget(row);
    }

    ui->layoutCardsContent->addStretch();
}

void FlashcardDeck::onAddCard() {
    QString front = ui->inputFront->text().trimmed();
    QString back  = ui->inputBack->text().trimmed();

    if (front.isEmpty() || back.isEmpty()) {
        ui->labelAddError->setText("Please fill in both fields.");
        ui->labelAddError->setVisible(true);
        return;
    }

    ui->labelAddError->setVisible(false);
    ui->inputFront->clear();
    ui->inputBack->clear();
    ui->inputFront->setFocus();

    populateCards();
}
