#include "history.h"
#include "ui_history.h"
#include <QFrame>
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

void History::populate(const QList<ExamRecord> &history) {
    QLayoutItem *item;
    while ((item = ui->layoutExamHistoryContent->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (history.isEmpty()) {
        QLabel *emptyLabel = new QLabel("No exams taken yet.");
        emptyLabel->setStyleSheet("color: #4a4a52; font-size: 14px;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        ui->layoutExamHistoryContent->addWidget(emptyLabel);
        ui->layoutExamHistoryContent->addStretch();
        return;
    }

    for (int i = history.size() - 1; i >= 0; i--) {
        const ExamRecord &rec = history[i];
        double pct = rec.total > 0 ? (double)rec.score / rec.total * 100.0 : 0.0;

        QString gradeStr;
        QString gradeColor;
        if (pct >= 90) { gradeStr = "Excellent (6)"; gradeColor = "#00D4AA"; }
        else if (pct >= 75) { gradeStr = "Very Good (5)"; gradeColor = "#00D4AA"; }
        else if (pct >= 60) { gradeStr = "Good (4)"; gradeColor = "#F5C518"; }
        else if (pct >= 45) { gradeStr = "Average (3)"; gradeColor = "#F5C518"; }
        else if (pct >= 30) { gradeStr = "Poor (2)"; gradeColor = "#FF6B6B"; }
        else { gradeStr = "Very Poor (2-)"; gradeColor = "#FF6B6B"; }

        QFrame *card = new QFrame;
        card->setObjectName("examHistoryCard");
        card->setStyleSheet(R"(
            QFrame#examHistoryCard {
                background-color: #1c1c1f;
                border: 1px solid #2a2a2d;
                border-radius: 10px;
            }
            QFrame#examHistoryCard:hover {
                background-color: #202023;
                border-color: #3a3a3e;
            }
        )");
        card->setMinimumHeight(80);
        card->setMaximumHeight(80);

        QHBoxLayout *cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(20, 12, 20, 12);
        cardLayout->setSpacing(16);

        QLabel *numLabel = new QLabel(QString("Exam #%1").arg(rec.number));
        numLabel->setStyleSheet("color: #6b6b76; font-size: 12px; font-weight: 600;");
        numLabel->setMinimumWidth(80);

        QLabel *scoreLabel = new QLabel(QString("%1 / %2").arg(rec.score).arg(rec.total));
        scoreLabel->setStyleSheet(QString("color: %1; font-size: 22px; font-weight: 700;").arg(gradeColor));
        scoreLabel->setMinimumWidth(80);

        QLabel *gradeLabel = new QLabel(gradeStr);
        gradeLabel->setStyleSheet(QString("color: %1; font-size: 13px; font-weight: 600;").arg(gradeColor));
        gradeLabel->setMinimumWidth(140);

        QProgressBar *bar = new QProgressBar;
        bar->setMinimum(0);
        bar->setMaximum(100);
        bar->setValue(static_cast<int>(pct));
        bar->setTextVisible(false);
        bar->setMaximumHeight(6);
        bar->setStyleSheet(QString(R"(
            QProgressBar {
                background-color: #2a2a2d;
                border-radius: 3px;
                border: none;
            }
            QProgressBar::chunk {
                background-color: %1;
                border-radius: 3px;
            }
        )").arg(gradeColor));

        QLabel *pctLabel = new QLabel(QString("%1%").arg(static_cast<int>(pct)));
        pctLabel->setStyleSheet("color: #6b6b76; font-size: 12px;");
        pctLabel->setMinimumWidth(40);
        pctLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QWidget *barContainer = new QWidget;
        QVBoxLayout *barLayout = new QVBoxLayout(barContainer);
        barLayout->setContentsMargins(0, 0, 0, 0);
        barLayout->addWidget(bar);

        cardLayout->addWidget(numLabel);
        cardLayout->addWidget(scoreLabel);
        cardLayout->addWidget(gradeLabel);
        cardLayout->addWidget(barContainer, 1);
        cardLayout->addWidget(pctLabel);

        ui->layoutExamHistoryContent->addWidget(card);
    }

    ui->layoutExamHistoryContent->addStretch();
}
