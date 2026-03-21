#pragma once

namespace Style {

static const char *answerNormal = R"(
    QPushButton {
        background-color: #ffffff;
        color: #3a4a70;
        font-size: 14px;
        border: 1px solid #d4d8ea;
        border-radius: 8px;
        padding: 14px 20px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #f4f5fa;
        border-color: #4f6fc4;
        color: #1a2440;
    }
)";

static const char *answerCorrect = R"(
    QPushButton {
        background-color: #e8f5e3;
        color: #2a6a20;
        font-size: 14px;
        border: 1px solid #a0cc80;
        border-radius: 8px;
        padding: 14px 20px;
        text-align: left;
    }
)";

static const char *answerWrong = R"(
    QPushButton {
        background-color: #fbeaea;
        color: #7a2020;
        font-size: 14px;
        border: 1px solid #e0a0a0;
        border-radius: 8px;
        padding: 14px 20px;
        text-align: left;
    }
)";

static const char *progressBar = R"(
    QProgressBar {
        background-color: #dde3f0;
        border: none;
        border-radius: 2px;
        max-height: 3px;
    }
    QProgressBar::chunk {
        background-color: #4f6fc4;
        border-radius: 2px;
    }
)";

static const char *timerNormal = "color: #8898c0; font-size: 17px; font-weight: 600;";
static const char *timerWarnOn = "color: #c0392b; font-size: 17px; font-weight: 600;";
static const char *timerWarnOff = "color: #e8c0c0; font-size: 17px; font-weight: 600;";

static const char *startExamBtn = R"(
    QPushButton {
        background-color: #4f6fc4;
        color: #ffffff;
        font-size: 13px;
        font-weight: 600;
        border: none;
        border-radius: 8px;
        padding: 10px 22px;
    }
    QPushButton:hover { background-color: #3a5aae; }
    QPushButton:pressed { background-color: #2c4890; }
)";

static const char *primaryBtn = R"(
    QPushButton {
        background-color: #4f6fc4;
        color: #ffffff;
        font-size: 14px;
        font-weight: 600;
        border: none;
        border-radius: 8px;
        padding: 12px 28px;
    }
    QPushButton:hover { background-color: #3a5aae; }
    QPushButton:disabled {
        background-color: #dde3f0;
        color: #a0aac8;
        border: none;
    }
)";

static const char *secondaryBtn = R"(
    QPushButton {
        background-color: transparent;
        color: #8898c0;
        font-size: 14px;
        font-weight: 500;
        border: 1px solid #d4d8ea;
        border-radius: 8px;
        padding: 12px 28px;
    }
    QPushButton:hover {
        background-color: #eceef5;
        color: #1a2440;
        border-color: #b0bcd8;
    }
)";

static const char *welcomeLabel = R"(
    QLabel {
        color: #8898c0;
        font-size: 13px;
    }
)";

static const char *statValue = R"(
    QLabel {
        color: #1a2440;
        font-size: 32px;
        font-weight: 700;
        letter-spacing: -1px;
    }
)";

}
