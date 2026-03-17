#pragma once

namespace Style {

static const char *emptyLabel = "color: #4a4a52; font-size: 14px;";

static const char *card = R"(
    QPushButton#examHistoryCard {
        background-color: #1c1c1f;
        border: 1px solid #2a2a2d;
        border-radius: 10px;
        text-align: left;
    }
    QPushButton#examHistoryCard:hover {
        background-color: #202023;
        border-color: #3a3a3e;
    }
)";

static const char *timeLabel = "color: #6b6b76; font-size: 12px; font-weight: 600;";
static const char *detailsLabel = "color: #9b9ba3; font-size: 12px; font-weight: 600;";
static const char *scoreLabel = "color: %1; font-size: 22px; font-weight: 700;";
static const char *gradeLabel = "color: %1; font-size: 13px; font-weight: 600;";
static const char *pctLabel = "color: #6b6b76; font-size: 12px;";

static const char *progressBar = R"(
    QProgressBar {
        background-color: #2a2a2d;
        border-radius: 3px;
        border: none;
    }
    QProgressBar::chunk {
        background-color: %1;
        border-radius: 3px;
    }
)";

}
