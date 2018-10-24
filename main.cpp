#include <QApplication>

#include <QDir>
#include <QFile>
#include <QTranslator>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QTime>

#include "mainwindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QDir::setCurrent(QApplication::applicationDirPath());

    QString locale = QLocale::system().name();

    //set translator for default widget's text (for example: QMessageBox's buttons)
    QTranslator qtTranslator;
    qtTranslator.load("qt_"+locale,QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // set translator for app
    QTranslator translator;
    if (translator.load(QString("qabcs_") + locale)==false){
        translator.load(QString("/usr/share/qabcs/qabcs_") + locale);
    }
    app.installTranslator(&translator);

    // Инициализация rnd
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWindow *window = new MainWindow();
    window->show();

    return app.exec();
}
