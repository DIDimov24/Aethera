#include "flashcarddeck.h"
#include "ui_flashcarddeck.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QFrame>
#include "database.h"

struct CardEntry {
    QString front;
    QString back;
};

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

    QList<CardEntry> cards;
    QList<QPair<QString, QString>> fromDb = Database::instance().loadFlashcardsForSubject(currentSubject);
    for (int i = 0; i < fromDb.size(); i++) {
        cards.append({ fromDb[i].first, fromDb[i].second });
    }

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

        QPushButton *deleteBtn = new QPushButton("✕");
        deleteBtn->setFixedSize(28, 28);
        deleteBtn->setCursor(Qt::PointingHandCursor);
        deleteBtn->setStyleSheet(R"(
            QPushButton {
                background-color: transparent;
                color: #c0392b;
                font-size: 13px;
                font-weight: 600;
                border: 1px solid #e0a0a0;
                border-radius: 6px;
            }
            QPushButton:hover {
                background-color: #fbeaea;
                border-color: #c0392b;
            }
        )");

        connect(deleteBtn, &QPushButton::clicked, this, [this, front = entry.front]() {
            Database::instance().deleteFlashcard(currentSubject, front);
            populateCards();
            emit deckChanged();
        });

        rowLayout->addWidget(frontLabel, 1);
        rowLayout->addWidget(backLabel, 2);
        rowLayout->addWidget(deleteBtn);

        ui->layoutCardsContent->addWidget(row);
    }

    ui->layoutCardsContent->addStretch();
}

void FlashcardDeck::onAddCard() {
    QString front = ui->inputFront->text().trimmed();
    QString back = ui->inputBack->text().trimmed();

    if (front.isEmpty() || back.isEmpty()) {
        ui->labelAddError->setText("Please fill in both fields.");
        ui->labelAddError->setVisible(true);
        return;
    }

    ui->labelAddError->setVisible(false);
    if (!Database::instance().saveFlashcard(currentSubject, front, back)) {
        ui->labelAddError->setText("Failed to save card.");
        ui->labelAddError->setVisible(true);
        return;
    }

    ui->inputFront->clear();
    ui->inputBack->clear();
    ui->inputFront->setFocus();

    populateCards();
    emit deckChanged();
}
