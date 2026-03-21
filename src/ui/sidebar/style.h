#pragma once

namespace Style {

static const char *navExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #1a2440;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 8px;
        padding: 8px 10px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #eceef5;
        color: #1a2440;
    }
)";

static const char *navExpandedActive = R"(
    QPushButton {
        background-color: #dbe4ff;
        color: #1a2440;
        font-size: 13px;
        font-weight: 600;
        border: none;
        border-radius: 8px;
        padding: 8px 10px;
        text-align: left;
    }
)";

static const char *navCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #1a2440;
        font-size: 13px;
        border: none;
        border-radius: 8px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: #eceef5;
        color: #1a2440;
    }
)";

static const char *navCollapsedActive = R"(
    QPushButton {
        background-color: #dbe4ff;
        color: #1a2440;
        font-size: 13px;
        border: none;
        border-radius: 8px;
        padding: 8px;
    }
)";

static const char *logoutExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #1a2440;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 8px;
        padding: 8px 10px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #eceef5;
        color: #1a2440;
    }
)";

static const char *logoutCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #1a2440;
        font-size: 13px;
        border: none;
        border-radius: 8px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: #eceef5;
        color: #1a2440;
    }
)";

static const char *loginExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #1a2440;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 8px;
        padding: 8px 10px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #eceef5;
        color: #1a2440;
    }
)";

static const char *loginCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #1a2440;
        font-size: 13px;
        border: none;
        border-radius: 8px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: #eceef5;
        color: #1a2440;
    }
)";

static const char *navDisabledExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #b0bcd8;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 8px;
        padding: 8px 10px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #f4f5fa;
        color: #b0bcd8;
    }
)";

static const char *navDisabledCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #b0bcd8;
        font-size: 13px;
        border: none;
        border-radius: 8px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: #f4f5fa;
        color: #b0bcd8;
    }
)";

}