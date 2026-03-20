#include "history.h"
#include "ui_history.h"
#include "style.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>

History::History(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::History)
{
    ui->setupUi(this);
    connect(ui->buttonNewExam, &QPushButton::clicked, this, &History::newExamRequested);
}

History::~History() {
    delete ui;
}

void History::loadAndPopulate(const QString &username) {
    QList<ExamAttempt> attempts = Database::instance().loadExamAttemptsForUser(username);
    ui->labelReviewHint->setVisible(!attempts.isEmpty());

    while (QLayoutItem *item = ui->layoutExamHistoryContent->takeAt(0)) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (attempts.isEmpty()) {
        QWidget *emptyContainer = new QWidget;
        QVBoxLayout *emptyLayout = new QVBoxLayout(emptyContainer);
        emptyLayout->setAlignment(Qt::AlignCenter);
        emptyLayout->setSpacing(12);

        QLabel *emptyLabel = new QLabel("No exams taken yet.");
        emptyLabel->setStyleSheet("color: #6b6b76; font-size: 15px; font-weight: 500;");
        emptyLabel->setAlignment(Qt::AlignCenter);

        QLabel *emptyHint = new QLabel("Take your first exam to start tracking your progress.");
        emptyHint->setStyleSheet("color: #3a3a3e; font-size: 13px;");
        emptyHint->setAlignment(Qt::AlignCenter);

        QPushButton *startBtn = new QPushButton("Take First Exam");
        startBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #1c1c1f; color: #e0e0e4; font-size: 13px; font-weight: 600;
                border: 1px solid #2a2a2d; border-radius: 8px; padding: 10px 24px;
            }
            QPushButton:hover { background-color: #222224; border-color: #3a3a3e; color: #ffffff; }
        )");
        startBtn->setFixedWidth(180);
        startBtn->setCursor(Qt::PointingHandCursor);
        connect(startBtn, &QPushButton::clicked, this, &History::newExamRequested);

        emptyLayout->addWidget(emptyLabel);
        emptyLayout->addWidget(emptyHint);
        emptyLayout->addSpacing(8);
        emptyLayout->addWidget(startBtn, 0, Qt::AlignCenter);

        ui->layoutExamHistoryContent->addStretch();
        ui->layoutExamHistoryContent->addWidget(emptyContainer);
        ui->layoutExamHistoryContent->addStretch();
        return;
    }

    for (int i = 0; i < attempts.size(); i++) {
        ExamAttempt attempt = attempts[i];
        double pct = (double)attempt.score / 20.0 * 100.0;

        QString gradeStr;
        QString gradeColor;
        if (pct >= 90)      { gradeStr = "Excellent (6)";  gradeColor = "#00D4AA"; }
        else if (pct >= 75) { gradeStr = "Very Good (5)";  gradeColor = "#00D4AA"; }
        else if (pct >= 60) { gradeStr = "Good (4)";       gradeColor = "#F5C518"; }
        else if (pct >= 45) { gradeStr = "Average (3)";    gradeColor = "#F5C518"; }
        else if (pct >= 30) { gradeStr = "Poor (2)";       gradeColor = "#FF6B6B"; }
        else                { gradeStr = "Very Poor (2-)"; gradeColor = "#FF6B6B"; }

        QPushButton *card = new QPushButton;
        card->setCursor(Qt::PointingHandCursor);
        card->setFlat(true);
        card->setObjectName("examHistoryCard");
        card->setStyleSheet(QString(R"(
            QPushButton#examHistoryCard { %1 }
            QPushButton#examHistoryCard:hover { background-color: #202023; border-color: #3a3a3e; }
        )").arg(Style::card));
        card->setMinimumHeight(80);
        card->setMaximumHeight(80);

        QHBoxLayout *cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(20, 12, 20, 12);
        cardLayout->setSpacing(16);

        QLabel *timeLabel = new QLabel(attempt.completedAt);
        timeLabel->setStyleSheet(Style::timeLabel);
        timeLabel->setMinimumWidth(130);

        QLabel *detailsLabel = new QLabel(QString("%1 • %2").arg(attempt.subject, attempt.difficulty));
        detailsLabel->setStyleSheet(Style::detailsLabel);
        detailsLabel->setMinimumWidth(170);

        QLabel *scoreLabel = new QLabel(QString("%1 / 20").arg(attempt.score));
        scoreLabel->setStyleSheet(QString(Style::scoreLabel).arg(gradeColor));
        scoreLabel->setMinimumWidth(80);

        QLabel *gradeLabel = new QLabel(gradeStr);
        gradeLabel->setStyleSheet(QString(Style::gradeLabel).arg(gradeColor));
        gradeLabel->setMinimumWidth(140);

        QProgressBar *bar = new QProgressBar;
        bar->setMinimum(0);
        bar->setMaximum(100);
        bar->setValue(static_cast<int>(pct));
        bar->setTextVisible(false);
        bar->setMaximumHeight(6);
        bar->setStyleSheet(QString(Style::progressBar).arg(gradeColor));

        QLabel *pctLabel = new QLabel(QString("%1%").arg(static_cast<int>(pct)));
        pctLabel->setStyleSheet(Style::pctLabel);
        pctLabel->setMinimumWidth(40);
        pctLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QWidget *barContainer = new QWidget;
        QVBoxLayout *barLayout = new QVBoxLayout(barContainer);
        barLayout->setContentsMargins(0, 0, 0, 0);
        barLayout->addWidget(bar);

        cardLayout->addWidget(timeLabel);
        cardLayout->addWidget(detailsLabel);
        cardLayout->addWidget(scoreLabel);
        cardLayout->addWidget(gradeLabel);
        cardLayout->addWidget(barContainer, 1);
        cardLayout->addWidget(pctLabel);

        connect(card, &QPushButton::clicked, this, [this, attemptId = attempt.id]() {
            emit reviewRequested(attemptId);
        });

        ui->layoutExamHistoryContent->addWidget(card);
    }

    ui->layoutExamHistoryContent->addStretch();
}