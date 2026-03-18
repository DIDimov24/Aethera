#include "review.h"
#include "ui_review.h"
#include "style.h"
#include "database.h"
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Review::Review(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Review)
{
    ui->setupUi(this);
    ui->buttonBack->setFlat(true);
    setStyleSheet(Style::page);

    connect(ui->buttonBack, &QPushButton::clicked, this, &Review::backToHistoryRequested);
}

Review::~Review()
{
    delete ui;
}

void Review::loadAndShow(int attemptId, const QString &subject, const QString &difficulty, int score) {
    QString resultsJson = Database::instance().loadExamResultsJson(attemptId);

    QJsonDocument doc = QJsonDocument::fromJson(resultsJson.toUtf8());
    QJsonArray resultsArray = doc.array();

    // clears previous items from layout
    while (QLayoutItem *item = ui->layoutReviewContent->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    ui->labelReviewTitle->setText(QString("Review • %1 • %2").arg(subject, difficulty));

    QLabel *summaryLabel = new QLabel(
        QString("Score: %1/20").arg(score),
        ui->scrollContents
    );
    summaryLabel->setStyleSheet(Style::subtitle);
    ui->layoutReviewContent->addWidget(summaryLabel);

    for (int i = 0; i < resultsArray.size(); i++) {
        QJsonObject qObj = resultsArray[i].toObject();

        int selectedIdx = qObj["selected"].toInt();
        int correctIdx = qObj["correct"].toInt();
        bool isCorrect = (selectedIdx == correctIdx);
        QString questionText = qObj["question_text"].toString();

        QString decisionText = isCorrect ? "Correct" : "Wrong";

        QFrame *card = new QFrame(ui->scrollContents);
        card->setObjectName("reviewCard");
        card->setStyleSheet(isCorrect ? Style::cardCorrect : Style::cardWrong);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 12, 14, 12);
        cardLayout->setSpacing(6);

        QLabel *verdictLabel = new QLabel(QString("Q%1 • %2").arg(i + 1).arg(decisionText), card);
        verdictLabel->setStyleSheet(isCorrect ? Style::verdictCorrect : Style::verdictWrong);

        QLabel *questionLabel = new QLabel(questionText, card);
        questionLabel->setStyleSheet(Style::question);
        questionLabel->setWordWrap(true);

        // selectedIdx = 2 -> 'A' + 2 = 'C'
        QString selectedText = QString(QChar('A' + selectedIdx));
        QString correctText = QString(QChar('A' + correctIdx));

        QLabel *yourAnswerLabel = new QLabel(QString("Your answer: %1").arg(selectedText), card);
        yourAnswerLabel->setStyleSheet(Style::meta);
        QLabel *correctAnswerLabel = new QLabel(QString("Correct answer: %1").arg(correctText), card);
        correctAnswerLabel->setStyleSheet(Style::meta);

        cardLayout->addWidget(verdictLabel);
        cardLayout->addWidget(questionLabel);
        cardLayout->addWidget(yourAnswerLabel);
        cardLayout->addWidget(correctAnswerLabel);

        ui->layoutReviewContent->addWidget(card);
    }

    ui->layoutReviewContent->addStretch();
}
