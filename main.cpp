#include <QApplication>

#include <QDir>
#include <QTextCodec>
#include <QTime>

#include "mainwindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QDir::setCurrent(QApplication::applicationDirPath());

    // initialization for rnd
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWindow *window = new MainWindow();
    window->show();

    return app.exec();
}
