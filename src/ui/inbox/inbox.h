#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>

struct InboxMessage {
    QString author;
    QString subject;
    QString participants;
    QString data;
    bool read = false;
};

QT_BEGIN_NAMESPACE
namespace Ui { class Inbox; }
QT_END_NAMESPACE

class Inbox : public QWidget {
    Q_OBJECT

public:
    explicit Inbox(QWidget *parent = nullptr);
    ~Inbox();

private:
    Ui::Inbox *ui;

    QString buildMessageHtml(const InboxMessage &message);
    void addInboxMessages(QVBoxLayout *layout, const QVector<InboxMessage> &messages);
};
