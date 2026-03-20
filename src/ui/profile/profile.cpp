#include "profile.h"
#include "../../core/session/usersession.h"
#include <QRandomGenerator>
#include <ui_profile.h>

Profile::Profile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Profile)
    , avatarPaths({"", "", "", "", "", ""})
    , currentAvatarIndex(QRandomGenerator::global()->bounded(6)) {
    ui->setupUi(this);

    connect(ui->buttonEditBio, &QPushButton::clicked, this, &Profile::editProfileRequested);
    connect(ui->buttonEditUsername, &QPushButton::clicked, this, &Profile::editProfileRequested);
    connect(ui->buttonEditGrade, &QPushButton::clicked, this, &Profile::editProfileRequested);

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

void Profile::refresh() {
    QString username = UserSession::instance().getUsername();
    QString grade = UserSession::instance().getGrade();
    QString bio = UserSession::instance().getBio();

    ui->labelHeroUsername->setText(username.isEmpty() ? "-" : username);
    ui->labelHeroGrade->setText(grade.isEmpty() ? "Grade -" : QString("Grade %1").arg(grade));
    ui->labelAccountValueUsername->setText(username.isEmpty() ? "-" : username);
    ui->labelAccountValueGrade->setText(grade.isEmpty() ? "-" : grade);

    if (bio.isEmpty()) {
        ui->labelBioText->setText("No bio yet.");
        ui->labelBioText->setStyleSheet("color: #6b6b76; font-size: 14px;");
    } else {
        ui->labelBioText->setText(bio);
        ui->labelBioText->setStyleSheet("color: #ffffff; font-size: 14px;");
    }

    QString avatarPath = avatarPaths.value(currentAvatarIndex, "");
    if (!avatarPath.isEmpty()) {
        QPixmap avatar(avatarPath);
        if (!avatar.isNull()) {
            ui->avatarCircle->setStyleSheet("background-image: url(" + avatarPath + "); background-position: center; background-repeat: no-repeat; background-color: #000000; border-radius: 44px;");
            return;
        }
    }
    ui->avatarCircle->setStyleSheet("background-color: #000000; border-radius: 44px;");
}