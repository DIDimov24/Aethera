#include "profile.h"
#include "../../core/session/usersession.h"
#include <ui_profile.h>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QEvent>
#include "database.h"

static const int AVATAR_SIZE = 110;

Profile::Profile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Profile) {
    ui->setupUi(this);

    ui->avatarCircle->installEventFilter(this);

    connect(ui->buttonEditBio,      &QPushButton::clicked, this, &Profile::editProfileRequested);
    connect(ui->buttonEditUsername, &QPushButton::clicked, this, &Profile::editProfileRequested);
    connect(ui->buttonEditGrade,    &QPushButton::clicked, this, &Profile::editProfileRequested);
    connect(ui->avatarCircle,       &QPushButton::clicked, this, &Profile::randomizeAvatar);
    connect(ui->buttonLogout,       &QPushButton::clicked, this, &Profile::logoutRequested);

    ui->labelSectionBio->setText("BIO");
    ui->labelSectionAccount->setText("ACCOUNT");
    ui->labelFieldUsername->setText("Username");
    ui->labelFieldGrade->setText("Grade");

    ui->labelBioText->setWordWrap(true);
    ui->labelBioText->setMaximumHeight(60);

    refresh();
}

Profile::~Profile() {
    delete ui;
}

bool Profile::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->avatarCircle) {
        if (event->type() == QEvent::Enter && !_hoveredAvatar.isNull()) {
            ui->avatarCircle->setIcon(QIcon(_hoveredAvatar));
        } else if (event->type() == QEvent::Leave && !_normalAvatar.isNull()) {
            ui->avatarCircle->setIcon(QIcon(_normalAvatar));
        }
    }
    return QWidget::eventFilter(obj, event);
}

QPixmap Profile::makeCircularPixmap(const QPixmap &src, int size) {
    QPixmap scaled = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    int x = (scaled.width()  - size) / 2;
    int y = (scaled.height() - size) / 2;
    scaled = scaled.copy(x, y, size, size);

    QPixmap result(size, size);
    result.fill(Qt::transparent);

    QPainter p(&result);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    p.setClipPath(path);
    p.drawPixmap(0, 0, scaled);
    return result;
}

void Profile::applyAvatar(const QString &avatarName) {
    _normalAvatar  = QPixmap();
    _hoveredAvatar = QPixmap();

    if (avatarName.isEmpty()) {
        ui->avatarCircle->setIcon(QIcon());
        ui->avatarCircle->setStyleSheet(
            "QPushButton#avatarCircle { background-color: #1c1c1f; border-radius: 55px; border: none; padding: 0px; }"
        );
        return;
    }

    QString path = QString(":/icons/%1.png").arg(avatarName);
    QPixmap src(path);
    if (src.isNull()) {
        ui->avatarCircle->setIcon(QIcon());
        return;
    }

    _normalAvatar = makeCircularPixmap(src, AVATAR_SIZE);

    _hoveredAvatar = QPixmap(AVATAR_SIZE, AVATAR_SIZE);
    _hoveredAvatar.fill(Qt::transparent);
    QPainter hp(&_hoveredAvatar);
    hp.setRenderHint(QPainter::Antialiasing);
    hp.drawPixmap(0, 0, _normalAvatar);

    QPainterPath cp;
    cp.addEllipse(0, 0, AVATAR_SIZE, AVATAR_SIZE);
    hp.setClipPath(cp);
    hp.setBrush(QColor(0, 0, 0, 140));
    hp.setPen(Qt::NoPen);
    hp.drawEllipse(0, 0, AVATAR_SIZE, AVATAR_SIZE);

    hp.setPen(QColor(255, 255, 255, 230));
    QFont f("Arial", 10, QFont::Bold);
    hp.setFont(f);
    hp.drawText(QRect(0, AVATAR_SIZE / 2 - 22, AVATAR_SIZE, 20), Qt::AlignCenter, "Randomize");
    hp.drawText(QRect(0, AVATAR_SIZE / 2 + 2,  AVATAR_SIZE, 20), Qt::AlignCenter, "avatar");

    ui->avatarCircle->setIcon(QIcon(_normalAvatar));
    ui->avatarCircle->setIconSize(QSize(AVATAR_SIZE, AVATAR_SIZE));
    ui->avatarCircle->setStyleSheet(
        "QPushButton#avatarCircle { background-color: transparent; border-radius: 55px; border: none; padding: 0px; }"
    );
}

void Profile::randomizeAvatar() {
    int index = QRandomGenerator::global()->bounded(1, 7);
    QString avatarName = QString("pfp%1").arg(index);
    applyAvatar(avatarName);
    UserSession::instance().setAvatar(avatarName);
    Database::instance().updateAvatar(UserSession::instance().getUsername(), avatarName);
}

void Profile::refresh() {
    QString username = UserSession::instance().getUsername();
    QString grade    = UserSession::instance().getGrade();
    QString bio      = UserSession::instance().getBio();
    QString avatar   = UserSession::instance().getAvatar();

    ui->labelHeroUsername->setText(username.isEmpty() ? "-" : username);
    ui->labelHeroGrade->setText(grade.isEmpty() ? "Grade -" : QString("Grade %1").arg(grade));
    ui->labelAccountValueUsername->setText(username.isEmpty() ? "-" : username);
    ui->labelAccountValueGrade->setText(grade.isEmpty() ? "-" : grade);

    if (bio.isEmpty()) {
        ui->labelBioText->setText("No bio yet.");
        ui->labelBioText->setStyleSheet("color: #8898c0; font-size: 14px; background: transparent;");
    } else {
        ui->labelBioText->setText(bio);
        ui->labelBioText->setStyleSheet("color: #3a4a70; font-size: 14px; background: transparent;");
    }

    applyAvatar(avatar);
}