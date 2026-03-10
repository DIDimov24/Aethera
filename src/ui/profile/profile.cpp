
#include "profile.h"
#include <ui_profile.h>

Profile::Profile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Profile) {
    ui->setupUi(this);

    connect(ui->buttonLogoutFromProfile, &QPushButton::clicked, this, &Profile::logoutRequested);
}

Profile::~Profile() {
    delete ui;
}