#include "inbox.h"
#include "ui_inbox.h"
#include "usersession.h"

#include <QCheckBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QVector>

Inbox::Inbox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Inbox) {

    ui->setupUi(this);

    const QString userClass = "Class " + UserSession::instance().getGrade();

    const QVector<InboxMessage> messages {
        {"Mentor Office", "Saturday Workshop Details", QString("%1, Mentors").arg(userClass), "Mar 21, 2026", false},
        {"Ralitsa Petkova", "Biology Project Update", QString("%1, Science Club").arg(userClass), "Mar 20, 2026", false},
        {"School Admin", "Exam Registration Window", "All Students", "Mar 19, 2026", true},
        {"Ivan Kolev", "Physics Lab Reminder", QString("%1, Lab Team").arg(userClass), "Mar 18, 2026", true},
        {"Maya Stefanova", "Weekly Algebra Plan", QString("%1, Parent Group").arg(userClass), "Mar 17, 2026", true}
    };

    addInboxMessages(ui->verticalLayout_2, messages);
}

Inbox::~Inbox() {
    delete ui;
}

QString Inbox::buildMessageHtml(const InboxMessage &message) {
    return QString(
            "<div style='line-height:1.35;'>"
                "<span style='color:#1a2440;font-weight:700;font-size:16px;'>%1</span><br/>"
                "<span style='color:#8898c0;font-weight:700;'>Author:</span> <span style='color:#1a2440;'>%2</span><br/>"
                "<span style='color:#8898c0;font-weight:700;'>Participants:</span> <span style='color:#1a2440;'>%3</span><br/>"
                "<span style='color:#8898c0;font-weight:700;'>Date:</span> <span style='color:#215ca8;'>%4</span>"
            "</div>")
        .arg(message.subject, message.author, message.participants, message.data);
}

void Inbox::addInboxMessages(QVBoxLayout *layout, const QVector<InboxMessage> &messages) {
    for (int i = 0; i < messages.size(); i++) {
        QFrame *cardFrame = new QFrame();
        cardFrame->setObjectName("messageCard");

        QVBoxLayout *cardLayout = new QVBoxLayout(cardFrame);
        cardLayout->setContentsMargins(16, 12, 16, 14);
        cardLayout->setSpacing(10);

        QHBoxLayout *topRowLayout = new QHBoxLayout();
        topRowLayout->setContentsMargins(0, 0, 0, 0);
        topRowLayout->setSpacing(10);

        QCheckBox *markAsRead = new QCheckBox("Mark as read", cardFrame);
        markAsRead->setChecked(messages[i].read);
        markAsRead->setStyleSheet(
            "QCheckBox { color: #1a2440; font-weight: 600; spacing: 8px; }"
            "QCheckBox:disabled { color: #8898c0; }"
            "QCheckBox::indicator { width: 14px; height: 14px; border: 1px solid #8898c0; border-radius: 4px; background: #ffffff; }"
            "QCheckBox::indicator:disabled { background: #eceef5; border: 1px solid #c3cbe3; }"
            "QCheckBox::indicator:checked { border: 1px solid #215ca8; background: #215ca8; image: url(:/icons/check.svg); }"
        );

        QLabel *stateLabel = new QLabel(cardFrame);
        stateLabel->setAlignment(Qt::AlignCenter);
        stateLabel->setMinimumWidth(64);

        QLabel *messageLabel = new QLabel(cardFrame);
        messageLabel->setTextFormat(Qt::RichText);
        messageLabel->setWordWrap(true);
        messageLabel->setText(buildMessageHtml(messages[i]));

        topRowLayout->addWidget(markAsRead);
        topRowLayout->addStretch();
        topRowLayout->addWidget(stateLabel);

        cardLayout->addLayout(topRowLayout);
        cardLayout->addWidget(messageLabel);

        auto applyCardState = [cardFrame, stateLabel](bool checked) {
            if (checked) {
                cardFrame->setStyleSheet("QFrame#messageCard { background-color: #ffffff; border: 1px solid #d4d8ea; border-radius: 12px; }");
                stateLabel->setText("Read");
                stateLabel->setStyleSheet("QLabel { color: #8898c0; background-color: #eceef5; border-radius: 8px; padding: 3px 8px; font-size: 12px; font-weight: 700; }");
            } else {
                cardFrame->setStyleSheet("QFrame#messageCard { background-color: #e8f4ff; border: 1px solid #215ca8; border-radius: 12px; }");
                stateLabel->setText("New");
                stateLabel->setStyleSheet("QLabel { color: #ffffff; background-color: #215ca8; border-radius: 8px; padding: 3px 8px; font-size: 12px; font-weight: 700; }");
            }
        };

        applyCardState(messages[i].read);
        if (messages[i].read) {
            markAsRead->setEnabled(false);
        }

        connect(markAsRead, &QCheckBox::toggled, cardFrame, [markAsRead, applyCardState](bool checked) {
            applyCardState(checked);
            if (checked) {
                markAsRead->setEnabled(false);
            }
        });

        layout->insertWidget(layout->count() - 1, cardFrame);
    }
}
