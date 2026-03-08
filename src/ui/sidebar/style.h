#pragma once
 
namespace Style {
 
static const char *navExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #6b6b76;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 6px;
        padding: 8px 12px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #1c1c1f;
        color: #e0e0e4;
    }
)";
 
static const char *navExpandedActive = R"(
    QPushButton {
        background-color: #222224;
        color: #ffffff;
        font-size: 13px;
        font-weight: 600;
        border: none;
        border-radius: 6px;
        padding: 8px 12px;
        text-align: left;
    }
)";
 
static const char *navCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #6b6b76;
        font-size: 13px;
        border: none;
        border-radius: 6px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: #1c1c1f;
        color: #e0e0e4;
    }
)";
 
static const char *navCollapsedActive = R"(
    QPushButton {
        background-color: #222224;
        color: #ffffff;
        font-size: 13px;
        border: none;
        border-radius: 6px;
        padding: 8px;
    }
)";
 
static const char *logoutExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #6b6b76;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 6px;
        padding: 8px 12px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #1c1c1f;
        color: #e05555;
    }
)";
 
static const char *logoutCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #6b6b76;
        font-size: 13px;
        border: none;
        border-radius: 6px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: #1c1c1f;
        color: #e05555;
    }
)";
 
static const char *loginExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #6b6b76;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 6px;
        padding: 8px 12px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #1c1c1f;
        color: #e0e0e4;
    }
)";
 
static const char *loginCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #6b6b76;
        font-size: 13px;
        border: none;
        border-radius: 6px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: #1c1c1f;
        color: #e0e0e4;
    }
)";
 
static const char *navDisabledExpanded = R"(
    QPushButton {
        background-color: transparent;
        color: #3a3a40;
        font-size: 13px;
        font-weight: 500;
        border: none;
        border-radius: 6px;
        padding: 8px 12px;
        text-align: left;
    }
    QPushButton:hover {
        background-color: #181819;
        color: #3a3a40;
    }
)";
 
static const char *navDisabledCollapsed = R"(
    QPushButton {
        background-color: transparent;
        color: #3a3a40;
        font-size: 13px;
        border: none;
        border-radius: 6px;
        padding: 8px;
    }
    QPushButton:hover {
        background-color: #181819;
        color: #3a3a40;
    }
)";
 
}
