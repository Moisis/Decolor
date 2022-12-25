#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include "mainwindow.h"


int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    QPixmap pixmap(":/img/splash.png");
    pixmap.scaled(400,0);
    QSplashScreen splash(pixmap);
    splash.show();
    MainWindow w;
    QTimer::singleShot(700, &splash, SLOT(close()));
    QTimer::singleShot(500, &w, SLOT(showMaximized()));


    return app.exec();
}
