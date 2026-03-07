#include <QApplication>
#include "home.h"
#include "database.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Database::instance().openDatabase();

    Home window;
    window.show();

    return app.exec();
}
