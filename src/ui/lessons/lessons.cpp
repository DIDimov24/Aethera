#include "lessons.h"
#include "ui_lessons.h"
#include "database.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

struct DeckInfo {
    QString subject;
    QString iconBg;
    QString iconPath;
};

Lessons::Lessons(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Lessons) {
    ui->setupUi(this);
    populateDecks();
}

Lessons::~Lessons() {
    delete ui;
}

void Lessons::refresh() {
    populateDecks();
}

void Lessons::populateDecks() {
    while (QLayoutItem *item = ui->layoutDecksContent->takeAt(0)) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    QPushButton *studyBanner = new QPushButton;
    studyBanner->setObjectName("studyBanner");
    studyBanner->setCursor(Qt::PointingHandCursor);
    studyBanner->setFlat(true);
    studyBanner->setMinimumHeight(64);
    studyBanner->setMaximumHeight(64);
    studyBanner->setStyleSheet(R"(
        QWidget#studyBanner {
            background-color: #ffffff;
            border: 1px solid #d4d8ea;
            border-radius: 12px;
        }
        QWidget#studyBanner:hover {
            border-color: #4f6fc4;
            background-color: #f4f5fa;
        }
    )");

    connect(studyBanner, &QPushButton::clicked, this, &Lessons::studyRequested);

    QWidget *bannerIcon = new QWidget(studyBanner);
    bannerIcon->setGeometry(16, 12, 40, 40);
    bannerIcon->setStyleSheet("background-color: #eef1fb; border-radius: 10px;");
    bannerIcon->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    QLabel *bannerIconLabel = new QLabel(bannerIcon);
    bannerIconLabel->setPixmap(QPixmap(":/icons/lessons.svg").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    bannerIconLabel->setGeometry(10, 10, 20, 20);

    QLabel *bannerTitle = new QLabel("Study flashcards", studyBanner);
    bannerTitle->setGeometry(88, 10, 260, 22);
    bannerTitle->setStyleSheet("color: #1a2440; font-size: 14px; font-weight: 600;");
    bannerTitle->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    QLabel *bannerSub = new QLabel("Pick a subject and start a session", studyBanner);
    bannerSub->setGeometry(88, 33, 320, 18);
    bannerSub->setStyleSheet("color: #8898c0; font-size: 11px;");
    bannerSub->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    ui->layoutDecksContent->addWidget(studyBanner);

    QLabel *sectionLabel = new QLabel("YOUR DECKS");
    sectionLabel->setStyleSheet("color: #8898c0; font-size: 9px; font-weight: 700; letter-spacing: 2.5px; margin-top: 10px;");
    ui->layoutDecksContent->addWidget(sectionLabel);

    const QList<DeckInfo> decks = {
        { "English", "#eef1fb", ":/icons/exam-2.svg" },
        { "Math", "#fef6e4", ":/icons/diagram.svg" },
        { "History", "#f0f8ec", ":/icons/history.svg" }
    };

    QWidget *gridRow = new QWidget;
    QHBoxLayout *gridLayout = new QHBoxLayout(gridRow);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(14);

    for (int i = 0; i < decks.size(); i++) {
        const DeckInfo &deck = decks[i];

        QPushButton *card = new QPushButton;
        card->setObjectName("deckCard");
        card->setCursor(Qt::PointingHandCursor);
        card->setFlat(true);
        card->setMinimumHeight(130);
        card->setStyleSheet(R"(
            QPushButton#deckCard {
                background-color: #ffffff;
                border: 1px solid #d4d8ea;
                border-radius: 12px;
                text-align: left;
                padding: 0px;
            }
            QPushButton#deckCard:hover {
                background-color: #f9f9fd;
                border-color: #4f6fc4;
            }
        )");

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(18, 18, 18, 14);
        cardLayout->setSpacing(4);

        QWidget *iconBox = new QWidget;
        iconBox->setFixedSize(36, 36);
        iconBox->setStyleSheet(QString("background-color: %1; border-radius: 9px;").arg(deck.iconBg));
        iconBox->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        QLabel *iconLabel = new QLabel(iconBox);
        iconLabel->setPixmap(QPixmap(deck.iconPath).scaled(18, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconLabel->setGeometry(9, 9, 18, 18);
        iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        QLabel *titleLabel = new QLabel(deck.subject);
        titleLabel->setStyleSheet("color: #1a2440; font-size: 14px; font-weight: 600;");
        titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        int cardCount = Database::instance().loadFlashcardsForSubject(deck.subject).size();
        QString cardsText = QString("%1 cards").arg(cardCount);
        QLabel *metaLabel = new QLabel(cardsText);
        metaLabel->setStyleSheet("color: #8898c0; font-size: 13px;");
        metaLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        QWidget *footer = new QWidget;
        footer->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        QHBoxLayout *footerLayout = new QHBoxLayout(footer);
        footerLayout->setContentsMargins(0, 6, 0, 0);
        footerLayout->setSpacing(0);

        QLabel *arrowLabel = new QLabel("›");
        arrowLabel->setStyleSheet("color: #c0c8e0; font-size: 16px;");

        footerLayout->addStretch();
        footerLayout->addWidget(arrowLabel);

        cardLayout->addWidget(iconBox);
        cardLayout->addSpacing(6);
        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(metaLabel);
        cardLayout->addWidget(footer);

        connect(card, &QPushButton::clicked, this, [this, subject = deck.subject]() {
            emit deckSelected(subject);
        });

        gridLayout->addWidget(card);
    }

    ui->layoutDecksContent->addWidget(gridRow);
    ui->layoutDecksContent->addStretch();
}
