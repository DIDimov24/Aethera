#include "profile.h"
#include "usersession.h"
#include "utils.h"
#include <ui_profile.h>
#include <QSize>
#include <QIcon>

Profile::Profile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Profile) {
    ui->setupUi(this);
    
    connect(ui->buttonEditBio, &QPushButton::clicked, this, &Profile::editProfileRequested);
    connect(ui->buttonEditUsername, &QPushButton::clicked, this, &Profile::editProfileRequested);
    connect(ui->buttonEditGrade, &QPushButton::clicked, this, &Profile::editProfileRequested);
    connect(ui->buttonLogout, &QPushButton::clicked, this, &Profile::logoutRequested);

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
    QString grade    = UserSession::instance().getGrade();
    QString bio      = UserSession::instance().getBio();

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

    QPixmap avatar = Utils::createAvatarPixmap(username, 110);
    ui->avatarCircle->setIcon(QIcon(avatar));
    ui->avatarCircle->setIconSize(QSize(110, 110));
    ui->avatarCircle->setStyleSheet(
        "QPushButton#avatarCircle { background-color: transparent; border-radius: 55px; border: none; padding: 0px; }"
    );
}
