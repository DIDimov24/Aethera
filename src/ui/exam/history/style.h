#pragma once

namespace Style {

static const char *emptyLabel = "color: #8898c0; font-size: 14px;";

static const char *card = R"(
    QPushButton#examHistoryCard {
        background-color: #ffffff;
        border: 1px solid #d4d8ea;
        border-radius: 10px;
        text-align: left;
    }
    QPushButton#examHistoryCard:hover {
        background-color: #f4f5fa;
        border-color: #4f6fc4;
    }
)";

static const char *timeLabel = "color: #8898c0; font-size: 12px; font-weight: 600;";
static const char *detailsLabel = "color: #5a6a90; font-size: 12px; font-weight: 600;";
static const char *scoreLabel = "color: %1; font-size: 22px; font-weight: 700;";
static const char *gradeLabel = "color: %1; font-size: 13px; font-weight: 600;";
static const char *pctLabel = "color: #8898c0; font-size: 12px;";

static const char *progressBar = R"(
    QProgressBar {
        background-color: #dde3f0;
        border-radius: 3px;
        border: none;
    }
    QProgressBar::chunk {
        background-color: %1;
        border-radius: 3px;
    }
)";

}
