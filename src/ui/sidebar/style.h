#pragma once

#include <QPixmap>
#include <QString>
#include <QPainter>
#include <QColor>
#include <QFont>
#include <QGuiApplication>
#include <QScreen>

namespace Style {

inline QPixmap createAvatarPixmap(const QString &name, int size) {
    int logicalSize = size > 0 ? size : 1;
    qreal dpr = 1.0;
    if (QGuiApplication::primaryScreen()) {
        dpr = QGuiApplication::primaryScreen()->devicePixelRatio();
    }

    int renderSize = qRound(logicalSize * dpr);
    if (renderSize < 1) renderSize = 1;

    QPixmap pixmap(renderSize, renderSize);
    pixmap.setDevicePixelRatio(dpr);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    painter.setBrush(QColor("#4f6fc4"));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, logicalSize, logicalSize);

    QString letter = name.isEmpty() ? "?" : name.left(1).toUpper();
    painter.setPen(QColor("#ffffff"));
    QFont font("Arial", logicalSize / 3, QFont::Bold);
    font.setStyleStrategy(QFont::PreferAntialias);
    painter.setFont(font);
    painter.drawText(0, 0, logicalSize, logicalSize, Qt::AlignCenter | Qt::AlignVCenter, letter);
    painter.end();

    return pixmap;
}

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