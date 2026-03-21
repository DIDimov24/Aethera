#include "history.h"
#include "ui_history.h"
#include "style.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>

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
        emptyLabel->setStyleSheet("color: #8898c0; font-size: 15px; font-weight: 500;");
        emptyLabel->setAlignment(Qt::AlignCenter);

        QLabel *emptyHint = new QLabel("Take your first exam to start tracking your progress.");
        emptyHint->setStyleSheet("color: #a0aac8; font-size: 13px;");
        emptyHint->setAlignment(Qt::AlignCenter);

        QPushButton *startBtn = new QPushButton("Take First Exam");
        startBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #4f6fc4; color: #ffffff; font-size: 13px; font-weight: 600;
                border: none; border-radius: 8px; padding: 10px 24px;
            }
            QPushButton:hover { background-color: #3a5aae; }
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
        QString iconBg;
        QString iconText;
        if (pct >= 90)      { gradeStr = "Excellent (6)"; gradeColor = "#2a6a20"; iconBg = "#e8f5e3"; iconText = "#2a6a20"; }
        else if (pct >= 75) { gradeStr = "Very Good (5)"; gradeColor = "#2a6a20"; iconBg = "#e8f5e3"; iconText = "#2a6a20"; }
        else if (pct >= 60) { gradeStr = "Good (4)";      gradeColor = "#7a5010"; iconBg = "#fef6e4"; iconText = "#7a5010"; }
        else if (pct >= 45) { gradeStr = "Average (3)";   gradeColor = "#7a5010"; iconBg = "#fef6e4"; iconText = "#7a5010"; }
        else if (pct >= 30) { gradeStr = "Poor (2)";      gradeColor = "#7a2020"; iconBg = "#fbeaea"; iconText = "#7a2020"; }
        else                { gradeStr = "Very Poor (2-)";gradeColor = "#7a2020"; iconBg = "#fbeaea"; iconText = "#7a2020"; }

        QPushButton *card = new QPushButton;
        card->setCursor(Qt::PointingHandCursor);
        card->setFlat(true);
        card->setObjectName("examHistoryCard");
        card->setMinimumHeight(64);
        card->setMaximumHeight(64);
        card->setStyleSheet(Style::card);

        QHBoxLayout *cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(16, 0, 16, 0);
        cardLayout->setSpacing(14);

        QWidget *iconBox = new QWidget;
        iconBox->setFixedSize(36, 36);
        iconBox->setStyleSheet(QString("background-color: %1; border-radius: 9px;").arg(iconBg));
        iconBox->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        QLabel *iconLabel = new QLabel(iconBox);
        iconLabel->setText(QString::number(attempt.score));
        iconLabel->setStyleSheet(QString("color: %1; font-size: 12px; font-weight: 700; background: transparent;").arg(iconText));
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setGeometry(0, 0, 36, 36);
        iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        QWidget *textBlock = new QWidget;
        textBlock->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        QVBoxLayout *textLayout = new QVBoxLayout(textBlock);
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(1);

        QLabel *titleLabel = new QLabel(QString("%1  ·  %2").arg(attempt.subject, attempt.difficulty));
        titleLabel->setStyleSheet("color: #1a2440; font-size: 13px; font-weight: 600;");
        titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        QLabel *subLabel = new QLabel(QString("%1  ·  %2 / 20  ·  %3%").arg(attempt.completedAt).arg(attempt.score).arg(static_cast<int>(pct)));
        subLabel->setStyleSheet("color: #8898c0; font-size: 11px;");
        subLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        textLayout->addWidget(titleLabel);
        textLayout->addWidget(subLabel);

        QWidget *rightBlock = new QWidget;
        rightBlock->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        QVBoxLayout *rightLayout = new QVBoxLayout(rightBlock);
        rightLayout->setContentsMargins(0, 0, 0, 0);
        rightLayout->setSpacing(2); // tighter
        rightLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QLabel *gradeChip = new QLabel(gradeStr);
        gradeChip->setStyleSheet(QString("color: %1; font-size: 12px; font-weight: 600;").arg(gradeColor));
        gradeChip->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        gradeChip->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        QProgressBar *bar = new QProgressBar;
        bar->setMinimum(0);
        bar->setMaximum(100);
        bar->setValue(static_cast<int>(pct));
        bar->setTextVisible(false);
        bar->setFixedHeight(4);
        bar->setFixedWidth(80);
        bar->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        bar->setStyleSheet(QString(R"(
            QProgressBar { background-color: #dde3f0; border-radius: 2px; border: none; }
            QProgressBar::chunk { background-color: %1; border-radius: 2px; }
        )").arg(gradeColor));

        rightLayout->addWidget(gradeChip, 0, Qt::AlignRight);
        rightLayout->addWidget(bar, 0, Qt::AlignRight);

        QLabel *arrowLabel = new QLabel("›");
        arrowLabel->setStyleSheet("color: #c0c8e0; font-size: 18px;");
        arrowLabel->setFixedWidth(16);
        arrowLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        cardLayout->addWidget(iconBox);
        cardLayout->addWidget(textBlock, 1);
        cardLayout->addWidget(rightBlock);
        cardLayout->addWidget(arrowLabel);

        connect(card, &QPushButton::clicked, this, [this, attemptId = attempt.id]() {
            emit reviewRequested(attemptId);
        });

        ui->layoutExamHistoryContent->addWidget(card);
    }

    ui->layoutExamHistoryContent->addStretch();
}