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

}