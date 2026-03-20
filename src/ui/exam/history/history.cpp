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
            QPushButton:hover { background-color: #202023; border-color: #3a3a3e; color: #ffffff; }
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
        if (pct >= 90)      { gradeStr = "Excellent (6)";  gradeColor = "#00D4AA"; iconBg = "#0a2e26"; }
        else if (pct >= 75) { gradeStr = "Very Good (5)";  gradeColor = "#00D4AA"; iconBg = "#0a2e26"; }
        else if (pct >= 60) { gradeStr = "Good (4)";       gradeColor = "#F5C518"; iconBg = "#2a2410"; }
        else if (pct >= 45) { gradeStr = "Average (3)";    gradeColor = "#F5C518"; iconBg = "#2a2410"; }
        else if (pct >= 30) { gradeStr = "Poor (2)";       gradeColor = "#FF6B6B"; iconBg = "#2a0f0f"; }
        else                { gradeStr = "Very Poor (2-)";  gradeColor = "#FF6B6B"; iconBg = "#2a0f0f"; }

        QPushButton *card = new QPushButton;
        card->setCursor(Qt::PointingHandCursor);
        card->setFlat(true);
        card->setObjectName("examHistoryCard");
        card->setMinimumHeight(76);
        card->setMaximumHeight(76);
        card->setStyleSheet(R"(
            QPushButton#examHistoryCard {
                background-color: #1c1c1f;
                border: 1px solid #2a2a2d;
                border-radius: 12px;
                text-align: left;
                padding: 0px;
            }
            QPushButton#examHistoryCard:hover {
                background-color: #202023;
                border-color: #3a3a3e;
            }
            QPushButton#examHistoryCard:pressed {
                background-color: #252528;
            }
        )");

        QHBoxLayout *cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(20, 0, 20, 0);
        cardLayout->setSpacing(16);

        QWidget *iconBox = new QWidget;
        iconBox->setFixedSize(40, 40);
        iconBox->setStyleSheet(QString("background-color: %1; border-radius: 10px;").arg(iconBg));

        QLabel *iconLabel = new QLabel(iconBox);
        iconLabel->setText(QString::number(attempt.score));
        iconLabel->setStyleSheet(QString("color: %1; font-size: 13px; font-weight: 700; background: transparent;").arg(gradeColor));
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setGeometry(0, 0, 40, 40);

        QWidget *textBlock = new QWidget;
        QVBoxLayout *textLayout = new QVBoxLayout(textBlock);
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(1);

        QLabel *titleLabel = new QLabel(QString("%1  •  %2").arg(attempt.subject, attempt.difficulty));
        titleLabel->setStyleSheet("color: #e0e0e4; font-size: 13px; font-weight: 600;");

        QLabel *subLabel = new QLabel(QString("%1  •  %2 / 20  •  %3%").arg(attempt.completedAt).arg(attempt.score).arg(static_cast<int>(pct)));
        subLabel->setStyleSheet("color: #6b6b76; font-size: 11px;");

        textLayout->addWidget(titleLabel);
        textLayout->addWidget(subLabel);

        QWidget *rightBlock = new QWidget;
        QVBoxLayout *rightLayout = new QVBoxLayout(rightBlock);
        rightLayout->setContentsMargins(0, 0, 0, 0);
        rightLayout->setSpacing(4);
        rightLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QLabel *gradeChip = new QLabel(gradeStr);
        gradeChip->setStyleSheet(QString("color: %1; font-size: 12px; font-weight: 600;").arg(gradeColor));
        gradeChip->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QProgressBar *bar = new QProgressBar;
        bar->setMinimum(0);
        bar->setMaximum(100);
        bar->setValue(static_cast<int>(pct));
        bar->setTextVisible(false);
        bar->setFixedHeight(4);
        bar->setFixedWidth(80);
        bar->setStyleSheet(QString(R"(
            QProgressBar { background-color: #2a2a2d; border-radius: 2px; border: none; }
            QProgressBar::chunk { background-color: %1; border-radius: 2px; }
        )").arg(gradeColor));

        rightLayout->addWidget(gradeChip, 0, Qt::AlignRight);
        rightLayout->addWidget(bar, 0, Qt::AlignRight);

        QLabel *arrowLabel = new QLabel("›");
        arrowLabel->setStyleSheet("color: #3a3a3e; font-size: 18px;");
        arrowLabel->setFixedWidth(16);

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