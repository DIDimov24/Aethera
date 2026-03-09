#include "home.h"
#include "ui_home.h"
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>

void Home::populateExamHistory() {
    QLayout *layout = ui->layoutExamHistoryContent;

    while (layout->count() > 0) {
        QLayoutItem *item = layout->takeAt(0);
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    if (examHistory.isEmpty()) {
        QLabel *empty = new QLabel("No exams taken yet. Start your first exam!");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color: #3a3a3e; font-size: 14px;");
        layout->addWidget(empty);

        QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addItem(spacer);
        return;
    }

    for (int i = examHistory.size() - 1; i >= 0; i--) {
        const ExamRecord &rec = examHistory[i];
        double pct = (double)rec.score / rec.total * 100.0;

        QString gradeText;
        QString color;
        if (pct >= 90)      { gradeText = "Excellent (6)";  color = "#00D4AA"; }
        else if (pct >= 75) { gradeText = "Very Good (5)";  color = "#00D4AA"; }
        else if (pct >= 60) { gradeText = "Good (4)";       color = "#F5C518"; }
        else if (pct >= 45) { gradeText = "Average (3)";    color = "#F5C518"; }
        else if (pct >= 30) { gradeText = "Poor (2)";       color = "#FF6B6B"; }
        else                { gradeText = "Very Poor (2-)"; color = "#FF6B6B"; }

        QFrame *card = new QFrame();
        card->setFixedHeight(88);
        card->setStyleSheet(
            "QFrame { background-color: #1c1c1f; border-radius: 10px; border: 1px solid #2a2a2d; }"
            );

        QHBoxLayout *cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(22, 0, 22, 0);
        cardLayout->setSpacing(0);

        QLabel *labelNum = new QLabel(QString("Exam #%1").arg(rec.number));
        labelNum->setFixedWidth(90);
        labelNum->setStyleSheet("color: #4a4a52; font-size: 11px; font-weight: 600; letter-spacing: 1px; border: none;");

        QVBoxLayout *centerLayout = new QVBoxLayout();
        centerLayout->setSpacing(6);
        centerLayout->setContentsMargins(0, 0, 0, 0);

        QProgressBar *bar = new QProgressBar();
        bar->setMinimum(0);
        bar->setMaximum(rec.total);
        bar->setValue(rec.score);
        bar->setTextVisible(false);
        bar->setFixedHeight(4);
        bar->setStyleSheet(QString(R"(
            QProgressBar {
                background-color: #222224;
                border: none;
                border-radius: 2px;
            }
            QProgressBar::chunk {
                background-color: %1;
                border-radius: 2px;
            }
        )").arg(color));

        QLabel *labelGrade = new QLabel(gradeText);
        labelGrade->setStyleSheet(QString("color: %1; font-size: 11px; font-weight: 600; border: none;").arg(color));

        centerLayout->addWidget(bar);
        centerLayout->addWidget(labelGrade);

        QLabel *labelScore = new QLabel(QString("%1 / %2").arg(rec.score).arg(rec.total));
        labelScore->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        labelScore->setFixedWidth(70);
        labelScore->setStyleSheet(QString("color: %1; font-size: 22px; font-weight: 700; letter-spacing: -0.5px; border: none;").arg(color));

        QLabel *labelPct = new QLabel(QString("%1%").arg((int)pct));
        labelPct->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        labelPct->setFixedWidth(54);
        labelPct->setStyleSheet("color: #4a4a52; font-size: 13px; font-weight: 500; border: none;");

        cardLayout->addWidget(labelNum);
        cardLayout->addLayout(centerLayout);
        cardLayout->addSpacing(20);
        cardLayout->addWidget(labelScore);
        cardLayout->addSpacing(8);
        cardLayout->addWidget(labelPct);

        layout->addWidget(card);
    }

    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addItem(spacer);
}
