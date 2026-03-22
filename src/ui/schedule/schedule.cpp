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
        {"1. Algebra", "Maya Stefanova", "Room B201", time[0], true},
        {"2. Physics", "Ivan Kolev", "Room B106", time[1], true},
        {"3. History", "Teodora Marinova", "Room A102", time[2], true},
        {"4. Biology", "Ralitsa Petkova", "Lab C104", time[3], true},
        {"5. Geometry", "Milena Asenova", "Room B201", time[4], true},
        {"6. Art Studio", "Neli Markova", "Room D001", time[5], true},
        {"7. English", "Dimitar Georgiev", "Room A205", time[6], true},
        {"8. Coding Lab", "Elena Petrova", "Lab A07", time[7], true}
    };

    const QVector<SubjectCard> tuesday {
        {"1. Chemistry", "Diana Georgieva", "Room C106", time[0], true},
        {"2. Literature", "Yana Todorova", "Room A204", time[1], true},
        {"3. Geography", "Viktor Simeonov", "Room B104", time[2], true},
        {"4. Music", "Petar Mitov", "Hall M01", time[3], true},
        {"5. Civics", "Asya Ivanova", "Room C107", time[4], true},
        {"6. Statistics", "Irina Yaneva", "Room B205", time[5], true},
        {"7. French", "Marina Velcheva", "Room A301", time[6], true},
        {"8. Robotics", "Nikolay Radev", "Lab R02", time[7], true}
    };

    const QVector<SubjectCard> wednesday {
        {"1. Math Apps", "Olga Dimitrova", "Room B210", time[0], true},
        {"2. Literature", "Yana Todorova", "Room A204", time[1]},
        {"3. Earth Science", "Tsvetelina Todorova", "Room C110", time[2]},
        {"4. Design Tech", "Rumen Naydenov", "Lab D12", time[3]},
        {"5. Spanish", "Silvia Mladenova", "Room A308", time[4]},
        {"6. Economics", "Stoyan Borisov", "Room C103", time[5]},
        {"7. PE Theory", "Radoslav Dimitrov", "Room G01", time[6]},
        {"8. Research", "Antonia Velinova", "Room B301", time[7]}
    };

    const QVector<SubjectCard> thursday {
        {"1. Calculus", "Georgi Iliev", "Room B202", time[0]},
        {"2. Physics Lab", "Ivan Kolev", "Lab P02", time[1]},
        {"3. Language Arts", "Maya Stefanova", "Room A207", time[2]},
        {"4. Chemistry Lab", "Diana Georgieva", "Lab C03", time[3]},
        {"5. Media", "Petar Mitov", "Hall M01", time[4]},
        {"6. Debate", "Asya Ivanova", "Room C201", time[5]},
        {"7. Ethics", "Stoyan Borisov", "Room B305", time[6]},
        {"8. Engineering", "Marin Bogdanov", "Lab E01", time[7]}
    };

    const QVector<SubjectCard> friday {
        {"1. Philosophy", "Kalina Dobreva", "Room A202", time[0]},
        {"2. Programming", "Elena Petrova", "Lab A07", time[1]},
        {"3. Biology", "Ralitsa Petkova", "Lab C104", time[2]},
        {"4. Drama", "Georgi Hadzhiev", "Hall T03", time[3]},
        {"5. Data Lab", "Marin Bogdanov", "Lab D15", time[4]},
        {"6. Team Project", "Ivaylo Nikolov", "Room B109", time[5]},
        {"7. Career Skills", "Petya Vladova", "Room C203", time[6]},
        {"8. Digital Arts", "Petar Dimitrov", "Studio S02", time[7]}
    };

    const QVector<SubjectCard> saturday {
        {"1. Workshop Math", "Mentor Team", "Room B101", time[0]},
        {"2. Reading Club", "Mentor Team", "Library", time[1]},
        {"3. Science Lab", "Mentor Team", "Lab S01", time[2]},
        {"4. Coding Intro", "Mentor Team", "Lab A07", time[3]},
        {"5. Art Practice", "Mentor Team", "Room D001", time[4]},
        {"6. Language Club", "Mentor Team", "Room A110", time[5]},
        {"7. Project Hour", "Mentor Team", "Room C101", time[6]},
        {"8. Sports Basics", "Mentor Team", "Hall G01", time[7]}
    };

    const QVector<SubjectCard> sunday {
        {"1. Reading Hour", "Open Session", "Library Hall", time[0]},
        {"2. Science Talk", "Open Session", "Room S01", time[1]},
        {"3. Math Games", "Open Session", "Room B102", time[2]},
        {"4. Coding Fun", "Open Session", "Lab A07", time[3]},
        {"5. Art Corner", "Open Session", "Room D002", time[4]},
        {"6. Story Time", "Open Session", "Library Hall", time[5]},
        {"7. Team Games", "Open Session", "Hall G01", time[6]},
        {"8. Review Hour", "Open Session", "Room C105", time[7]}
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
