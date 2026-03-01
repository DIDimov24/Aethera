#include <QApplication>
#include "ui/home/home.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Home window;
    window.show();
    return app.exec();
}
