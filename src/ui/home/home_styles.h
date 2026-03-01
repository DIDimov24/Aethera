#pragma once

namespace Style {

static const char *navExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #7a7a9a;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 8px;
        padding: 10px 16px;
        text-align: left;
        icon-size: 18px;
    }
    QPushButton:hover {
        background-color: #252340;
        color: #E8E8F0;
    }
)";

static const char *navExpandedActive = R"(
    QPushButton {
        background-color: #1a1836;
        color: #00D4AA;
        font-size: 13px;
        font-weight: 600;
        border: none;
        border-left: 3px solid #00D4AA;
        border-radius: 0px 8px 8px 0px;
        padding: 10px 16px;
        text-align: left;
        icon-size: 18px;
    }
    QPushButton:hover {
        background-color: #252340;
        color: #00D4AA;
    }
)";

static const char *navCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #7a7a9a;
        font-size: 13px;
        border: none;
        border-radius: 8px;
        padding: 10px 0px;
        text-align: center;
        icon-size: 18px;
        min-height: 0px;
        max-height: 40px;
    }
    QPushButton:hover {
        background-color: #252340;
        color: #E8E8F0;
    }
)";

static const char *navCollapsedActive = R"(
    QPushButton {
        background-color: #1a1836;
        color: #00D4AA;
        font-size: 13px;
        border: none;
        border-left: 3px solid #00D4AA;
        border-radius: 0px 8px 8px 0px;
        padding: 10px 0px;
        text-align: center;
        icon-size: 18px;
        min-height: 0px;
        max-height: 40px;
    }
    QPushButton:hover {
        background-color: #252340;
    }
)";

static const char *logoutExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #FF6B6B;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 8px;
        padding: 10px 16px;
        text-align: left;
        icon-size: 18px;
    }
    QPushButton:hover {
        background-color: #2e1520;
        color: #FF8F8F;
    }
)";

static const char *logoutCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #FF6B6B;
        font-size: 13px;
        border: none;
        border-radius: 8px;
        padding: 10px 0px;
        text-align: center;
        icon-size: 18px;
        min-height: 0px;
        max-height: 40px;
    }
    QPushButton:hover {
        background-color: #2e1520;
        color: #FF8F8F;
    }
)";

static const char *loginExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #7a7a9a;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 8px;
        padding: 10px 16px;
        text-align: left;
        icon-size: 18px;
    }
    QPushButton:hover {
        background-color: #252340;
        color: #E8E8F0;
    }
)";

static const char *loginCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #7a7a9a;
        font-size: 13px;
        border: none;
        border-radius: 8px;
        padding: 10px 0px;
        text-align: center;
        icon-size: 18px;
        min-height: 0px;
        max-height: 40px;
    }
    QPushButton:hover {
        background-color: #252340;
        color: #E8E8F0;
    }
)";

static const char *answerNormal = R"(
    QPushButton {
        background-color: #1e1c35;
        color: #C0C0D8;
        font-size: 14px;
        border: 1px solid #2e2c50;
        border-radius: 10px;
        padding: 14px 20px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #252340;
        border-color: #7C5CFC;
        color: #E8E8F0;
    }
)";

static const char *answerCorrect = R"(
    QPushButton {
        background-color: #0d2e22;
        color: #00D4AA;
        font-size: 14px;
        border: 1px solid #00D4AA;
        border-radius: 10px;
        padding: 14px 20px;
        text-align: left;
    }
)";

static const char *answerWrong = R"(
    QPushButton {
        background-color: #2e0d0d;
        color: #FF6B6B;
        font-size: 14px;
        border: 1px solid #FF6B6B;
        border-radius: 10px;
        padding: 14px 20px;
        text-align: left;
    }
)";

static const char *progressBar = R"(
    QProgressBar {
        background-color: #1e1c35;
        border: none;
        border-radius: 4px;
        max-height: 6px;
    }
    QProgressBar::chunk {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #7C5CFC, stop:1 #00D4AA);
        border-radius: 4px;
    }
)";

static const char *timerNormal = "color: #E8E8F0; font-size: 20px; font-weight: bold;";
static const char *timerWarnOn = "color: #FF6B6B; font-size: 20px; font-weight: bold;";
static const char *timerWarnOff = "color: #7a3535; font-size: 20px; font-weight: bold;";

static const char *startExamBtn = R"(
    QPushButton {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #7C5CFC, stop:1 #00D4AA);
        color: #FFFFFF;
        font-size: 15px;
        font-weight: 700;
        border: none;
        border-radius: 12px;
        padding: 14px 36px;
    }
    QPushButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #9575FF, stop:1 #00EFC0);
    }
    QPushButton:pressed {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #6040e0, stop:1 #00b890);
    }
)";

static const char *primaryBtn = R"(
    QPushButton {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #7C5CFC, stop:1 #00D4AA);
        color: #FFFFFF;
        font-size: 14px;
        font-weight: 700;
        border: none;
        border-radius: 10px;
        padding: 12px 28px;
    }
    QPushButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #9575FF, stop:1 #00EFC0);
    }
    QPushButton:disabled {
        background-color: #252340;
        color: #4a4a6a;
    }
)";

static const char *secondaryBtn = R"(
    QPushButton {
        background-color: transparent;
        color: #7a7a9a;
        font-size: 14px;
        font-weight: 500;
        border: 1px solid #2e2c50;
        border-radius: 10px;
        padding: 12px 28px;
    }
    QPushButton:hover {
        background-color: #1e1c35;
        color: #E8E8F0;
        border-color: #7C5CFC;
    }
)";

static const char *welcomeLabel = R"(
    QLabel {
        color: #7a7a9a;
        font-size: 14px;
    }
)";

static const char *statValue = R"(
    QLabel {
        color: #7C5CFC;
        font-size: 28px;
        font-weight: 700;
    }
)";

}
