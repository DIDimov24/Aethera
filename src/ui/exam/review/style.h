#pragma once

namespace Style {

static const char *page = R"(
    QWidget#Review {
        background-color: #161618;
    }
    QScrollArea#scrollAreaReview {
        background: transparent;
        border: none;
    }
    QWidget#scrollContents {
        background: transparent;
    }
    QPushButton#buttonBack {
        background-color: #ffffff;
        color: #111113;
        font-size: 14px;
        font-weight: 600;
        border: none;
        border-radius: 8px;
        padding: 10px 18px;
    }
    QPushButton#buttonBack:hover {
        background-color: #e8e8e8;
    }
    QPushButton#buttonBack:pressed {
        background-color: #d0d0d0;
    }
    QLabel#labelReviewTitle {
        color: #ffffff;
        font-size: 20px;
        font-weight: 700;
    }
)";

static const char *subtitle = "color: #9b9ba3; font-size: 12px; font-weight: 600;";
static const char *cardCorrect = "QFrame#reviewCard { background-color: #0f1f17; border: 1px solid #1a5c38; border-radius: 10px; }";
static const char *cardWrong = "QFrame#reviewCard { background-color: #1f1111; border: 1px solid #5c1a1a; border-radius: 10px; }";
static const char *question = "color: #ffffff; font-size: 14px; font-weight: 600;";
static const char *meta = "color: #e0e0e4; font-size: 12px;";
static const char *verdictCorrect = "color: #3dd68c; font-size: 12px; font-weight: 700;";
static const char *verdictWrong = "color: #ff8a8a; font-size: 12px; font-weight: 700;";

}
