#pragma once

namespace Style {

static const char *answerNormal = R"(
    QPushButton {
        background-color: #1c1c1f;
        color: #a0a0aa;
        font-size: 14px;
        border: 1px solid #2a2a2d;
        border-radius: 8px;
        padding: 14px 20px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #222224;
        border-color: #3a3a3e;
        color: #ffffff;
    }
)";

static const char *answerCorrect = R"(
    QPushButton {
        background-color: #0d2318;
        color: #3dd68c;
        font-size: 14px;
        border: 1px solid #1a5c38;
        border-radius: 8px;
        padding: 14px 20px;
        text-align: left;
    }
)";

static const char *answerWrong = R"(
    QPushButton {
        background-color: #230d0d;
        color: #e05555;
        font-size: 14px;
        border: 1px solid #5c1a1a;
        border-radius: 8px;
        padding: 14px 20px;
        text-align: left;
    }
)";

static const char *progressBar = R"(
    QProgressBar {
        background-color: #222224;
        border: none;
        border-radius: 2px;
        max-height: 3px;
    }
    QProgressBar::chunk {
        background-color: #ffffff;
        border-radius: 2px;
    }
)";

static const char *timerNormal = "color: #6b6b76; font-size: 17px; font-weight: 600;";
static const char *timerWarnOn = "color: #e05555; font-size: 17px; font-weight: 600;";
static const char *timerWarnOff = "color: #5c1a1a; font-size: 17px; font-weight: 600;";

static const char *startExamBtn = R"(
    QPushButton {
        background-color: #ffffff;
        color: #111113;
        font-size: 13px;
        font-weight: 600;
        border: none;
        border-radius: 8px;
        padding: 10px 22px;
    }
    QPushButton:hover {
        background-color: #e8e8e8;
    }
    QPushButton:pressed {
        background-color: #d0d0d0;
    }
)";

static const char *primaryBtn = R"(
    QPushButton {
        background-color: #ffffff;
        color: #111113;
        font-size: 14px;
        font-weight: 600;
        border: none;
        border-radius: 8px;
        padding: 12px 28px;
    }
    QPushButton:hover {
        background-color: #e8e8e8;
    }
    QPushButton:disabled {
        background-color: #1c1c1f;
        color: #3a3a3e;
        border: 1px solid #2a2a2d;
    }
)";

static const char *secondaryBtn = R"(
    QPushButton {
        background-color: transparent;
        color: #6b6b76;
        font-size: 14px;
        font-weight: 500;
        border: 1px solid #2a2a2d;
        border-radius: 8px;
        padding: 12px 28px;
    }
    QPushButton:hover {
        background-color: #1c1c1f;
        color: #ffffff;
        border-color: #3a3a3e;
    }
)";

static const char *welcomeLabel = R"(
    QLabel {
        color: #6b6b76;
        font-size: 13px;
    }
)";

static const char *statValue = R"(
    QLabel {
        color: #ffffff;
        font-size: 32px;
        font-weight: 700;
        letter-spacing: -1px;
    }
)";

}
