#include "review.h"
#include "ui_review.h"
#include "style.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

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

void Review::showReview(const ExamRecord &record)
{
    while (QLayoutItem *item = ui->layoutReviewContent->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    ui->labelReviewTitle->setText(QString("Review • %1 • %2").arg(record.subject, record.difficulty));

    QLabel *summaryLabel = new QLabel(
        QString("Score: %1/%2 • %3")
            .arg(record.score)
            .arg(record.total)
            .arg(record.completedAt.toString("yyyy-MM-dd HH:mm")),
        ui->scrollContents
    );
    summaryLabel->setStyleSheet(Style::subtitle);
    ui->layoutReviewContent->addWidget(summaryLabel);

    for (int i = 0; i < record.questionResults.size(); ++i) {
        const ExamQuestionResult &result = record.questionResults[i];
        const bool isCorrect = (result.selectedAnswer == result.correctAnswer);

        QString verdictText = isCorrect ? "Correct" : "Wrong";

        QFrame *card = new QFrame(ui->scrollContents);
        card->setObjectName("reviewCard");
        card->setStyleSheet(isCorrect ? Style::cardCorrect : Style::cardWrong);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 12, 14, 12);
        cardLayout->setSpacing(6);

        QLabel *verdictLabel = new QLabel(QString("Q%1 • %2").arg(i + 1).arg(verdictText), card);
        verdictLabel->setStyleSheet(isCorrect ? Style::verdictCorrect : Style::verdictWrong);

        QLabel *questionLabel = new QLabel(result.questionText, card);
        questionLabel->setStyleSheet(Style::question);
        questionLabel->setWordWrap(true);

        auto optionToText = [&](int index) {
            if (index == 0) return QString("A) %1").arg(result.optionA);
            if (index == 1) return QString("B) %1").arg(result.optionB);
            if (index == 2) return QString("C) %1").arg(result.optionC);
            return QString("D) %1").arg(result.optionD);
        };

        QString yourAnswer = result.selectedAnswer >= 0
            ? optionToText(result.selectedAnswer)
            : QString("Not answered");
        QString correctAnswer = optionToText(result.correctAnswer);

        QLabel *yourAnswerLabel = new QLabel(QString("Your answer: %1").arg(yourAnswer), card);
        yourAnswerLabel->setStyleSheet(Style::meta);
        QLabel *correctAnswerLabel = new QLabel(QString("Correct answer: %1").arg(correctAnswer), card);
        correctAnswerLabel->setStyleSheet(Style::meta);

        cardLayout->addWidget(verdictLabel);
        cardLayout->addWidget(questionLabel);
        cardLayout->addWidget(yourAnswerLabel);
        cardLayout->addWidget(correctAnswerLabel);

        ui->layoutReviewContent->addWidget(card);
    }

    ui->layoutReviewContent->addStretch();
}
