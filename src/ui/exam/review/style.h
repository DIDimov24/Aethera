#pragma once

namespace Style {

static const char *page = R"(
    QWidget#Review {
        background-color: #eceef5;
    }
    QScrollArea#scrollAreaReview {
        background: transparent;
        border: none;
    }
    QWidget#scrollContents {
        background: transparent;
    }
    QPushButton#buttonBack {
        background-color: #4f6fc4;
        color: #ffffff;
        font-size: 14px;
        font-weight: 600;
        border: none;
        border-radius: 8px;
        padding: 10px 18px;
    }
    QPushButton#buttonBack:hover {
        background-color: #3a5aae;
    }
    QPushButton#buttonBack:pressed {
        background-color: #2c4890;
    }
    QLabel#labelReviewTitle {
        color: #1a2440;
        font-size: 20px;
        font-weight: 700;
    }
)";

static const char *subtitle = "color: #8898c0; font-size: 12px; font-weight: 600;";
static const char *cardCorrect = "QFrame#reviewCard { background-color: #f0f8ec; border: 1px solid #a0cc80; border-radius: 10px; padding: 2px; }";
static const char *cardWrong   = "QFrame#reviewCard { background-color: #fdf0f0; border: 1px solid #e0a0a0; border-radius: 10px; padding: 2px; }";
static const char *question    = "color: #1a2440; font-size: 14px; font-weight: 600; background: transparent;";
static const char *meta        = "color: #5a6a90; font-size: 12px; background: transparent;";
static const char *verdictCorrect = "color: #2a6a20; font-size: 12px; font-weight: 700; background: transparent;";
static const char *verdictWrong   = "color: #7a2020; font-size: 12px; font-weight: 700; background: transparent;";

}
