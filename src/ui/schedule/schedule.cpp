#include "schedule.h"
#include "ui_schedule.h"

#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QVector>

Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Schedule) {

    ui->setupUi(this);

    const QVector<QString> time = {
        "08:15 - 09:00",
        "09:05 - 09:50",
        "09:10 - 10:45",
        "10:50 - 11:35",
        "12:00 - 12:45",
        "12:50 - 13:35",
        "13:15 - 14:30",
        "14:35 - 15:20"
    };

    const QVector<SubjectCard> monday {
        {"1. English", "Maya Stefanova", "Room B201", time[0], true},
        {"2. English", "Ivan Kolev", "Room B106", time[1], true},
        {"3. History", "Teodora Marinova", "Room A102", time[2], true},
        {"4. History", "Ralitsa Petkova", "Lab C104", time[3], true},
        {"5. Math", "Milena Asenova", "Room B201", time[4], true},
        {"6. Math", "Neli Markova", "Room D001", time[5], true},
        {"7. English", "Dimitar Georgiev", "Room A205", time[6], true},
        {"8. Philosophy", "Elena Petrova", "Lab A07", time[7], true}
    };

    const QVector<SubjectCard> tuesday {
        {"1. Chemistry", "Diana Georgieva", "Room C106", time[0], true},
        {"2. Chemistry", "Yana Todorova", "Room A204", time[1], true},
        {"3. Literature", "Viktor Simeonov", "Room B104", time[2], true},
        {"4. Geography", "Petar Mitov", "Hall M01", time[3], true},
        {"5. English", "Asya Ivanova", "Room C107", time[4], true},
        {"6. English", "Irina Yaneva", "Room B205", time[5], true},
        {"7. Philosophy", "Marina Velcheva", "Room A301", time[6], true},
        {"8. German", "Nikolay Radev", "Lab R02", time[7], true}
    };

    const QVector<SubjectCard> wednesday {
        {"1. Math", "Olga Dimitrova", "Room B210", time[0], true},
        {"2. Literature", "Yana Todorova", "Room A204", time[1]},
        {"3. Math", "Tsvetelina Todorova", "Room C110", time[2]},
        {"4. German", "Rumen Naydenov", "Lab D12", time[3]},
        {"5. German", "Silvia Mladenova", "Room A308", time[4]},
        {"6. PE", "Stoyan Borisov", "Room C103", time[5]},
        {"7. English", "Radoslav Dimitrov", "Room G01", time[6]},
        {"8. English", "Antonia Velinova", "Room B301", time[7]}
    };

    const QVector<SubjectCard> thursday {
        {"1. Math", "Georgi Iliev", "Room B202", time[0]},
        {"2. Physics", "Ivan Kolev", "Lab P02", time[1]},
        {"3. German", "Maya Stefanova", "Room A207", time[2]},
        {"4. Chemistry", "Diana Georgieva", "Lab C03", time[3]},
        {"5. Programming", "Petar Mitov", "Hall M01", time[4]},
        {"6. English", "Asya Ivanova", "Room C201", time[5]},
        {"7. English", "Stoyan Borisov", "Room B305", time[6]},
        {"8. Programming", "Marin Bogdanov", "Lab E01", time[7]}
    };

    const QVector<SubjectCard> friday {
        {"1. Philosophy", "Kalina Dobreva", "Room A202", time[0]},
        {"2. Programming", "Elena Petrova", "Lab A07", time[1]},
        {"3. Programming", "Ralitsa Petkova", "Lab C104", time[2]},
        {"4. Biology", "Georgi Hadzhiev", "Hall T03", time[3]},
        {"5. Biology", "Marin Bogdanov", "Lab D15", time[4]},
        {"6. PE", "Ivaylo Nikolov", "Room B109", time[5]},
        {"7. Chemistry", "Petya Vladova", "Room C203", time[6]},
        {"8. Digital Art", "Petar Dimitrov", "Studio S02", time[7]}
    };

    addDayCards(ui->layoutSubjectsMon, monday);
    addDayCards(ui->layoutSubjectsTue, tuesday);
    addDayCards(ui->layoutSubjectsWed, wednesday);
    addDayCards(ui->layoutSubjectsThu, thursday);
    addDayCards(ui->layoutSubjectsFri, friday);
}

Schedule::~Schedule() {
    delete ui;
}

QString Schedule::buildCardHtml(const SubjectCard &card) {
    return QString(
            "<div>"
               "<img src=':/icons/exam.svg' width='12' height='12'/> "
               "<span style='color:#215ca8;font-weight:700;font-size:16px;'>%1</span><br/>"
               "<img src=':/icons/glasses.svg' width='11' height='11'/> %2<br/>"
               "<img src=':/icons/key-round.svg' width='11' height='11'/> %3<br/>"
               "<img src=':/icons/clock.svg' width='11' height='11'/> %4"
            "</div>")
        .arg(card.title, card.teacher, card.room, card.time);
}

void Schedule::addDayCards(QVBoxLayout *layout, const QVector<SubjectCard> &cards) {
    for (int i = 0; i < cards.size(); i++) {
        QLabel *cardLabel = new QLabel();
        cardLabel->setTextFormat(Qt::RichText);
        cardLabel->setText(buildCardHtml(cards[i]));
        if (cards[i].completed) cardLabel->setStyleSheet("background-color: #9fcd8a;");
        layout->addWidget(cardLabel);
    }
}