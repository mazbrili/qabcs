#include <QApplication>

#include <QDir>
#include <QTextCodec>
#include <QTime>
#include <QCommandLineParser>

#include "config_qabcs.h"
#include "mainwindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    QApplication::setApplicationName("qABCs");
    QApplication::setApplicationVersion(QABCS_VERSION);
    QApplication::setOrganizationName("DanSoft");
    QApplication::setOrganizationDomain("dansoft.ru");

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.process(app);


    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QDir::setCurrent(QApplication::applicationDirPath());

    // initialization for rnd
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWindow *window = new MainWindow();
    window->show();

    return app.exec();
}
