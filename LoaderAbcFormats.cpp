#include "LoaderAbcFormats.h"

#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


ABC_CONFIG LoaderAbcFormats::LoadFilename(const QString &fileName){
    if (fileName.indexOf(QRegExp("(.*)(.json)$"))!=-1){
        if (QFile::exists(fileName)){
            return loadAbcConfigJson(fileName);
        }
    }

    if (fileName.indexOf(QRegExp("(.*)(.properties)$"))!=-1){
        if (QFile::exists(fileName)){
            return loadAbcConfigProperties(fileName);
        }
    }

    return ABC_CONFIG();
}

ABC_CONFIG LoaderAbcFormats::loadAbcConfigJson(const QString &fileName){
    ABC_CONFIG result;

    QByteArray val;
    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(0,"qABCs",QObject::tr("Error while opening")+" "+fileName+"\n"+file.errorString());
        return ABC_CONFIG();
    }
    val = file.readAll();
    file.close();


    QStringList folders = fileName.split(QRegExp("[/\\\\]"));
    QString folder_lang = folders.at(folders.size()-2);

    QJsonDocument document = QJsonDocument::fromJson(val);
    if (document.isEmpty()){
        QMessageBox::critical(0,"qABCs",fileName+" "+QObject::tr("is not valid"));
        return ABC_CONFIG();
    }
    QJsonObject root = document.object();
    QJsonObject root_general = root.value("general").toObject();


    result.format = "json";
    result.folder_lang = folder_lang;
    result.filename = fileName;
    result.author = (root_general.value("author").isString()) ? root_general.value("author").toString(): QString();
    result.language = (root_general.value("language").isString()) ? root_general.value("language").toString(): QString();
    result.speak_method = (root_general.value("speak_method").isString()) ? root_general.value("speak_method").toString(): QString();
    result.espeak_params = (root_general.value("espeak_params").isString()) ? root_general.value("espeak_params").toString(): QString();
    result.inheritsFrom = (root_general.value("inheritsFrom").isString()) ? root_general.value("inheritsFrom").toString(): QString();
    result.visible = (root_general.value("visible").isString()) ? root_general.value("visible").toString(): QString();



    QJsonArray arrLetters = root.value("letters").toArray();
    for (int i=0;i<arrLetters.size();i++){
        QString letter = arrLetters.at(i).toObject().keys().at(0);
        QJsonObject objLetter = arrLetters.at(i).toObject().value(letter).toObject();



    }


    return result;

}

ABC_CONFIG LoaderAbcFormats::loadAbcConfigProperties(const QString &fileName){
    ABC_CONFIG result;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(0,"qABCs",QObject::tr("Error while opening")+" "+fileName+"\n"+file.errorString());
        return ABC_CONFIG();
    }

    QStringList folders = fileName.split(QRegExp("[/\\\\]"));
    QString folder_lang = folders.at(folders.size()-2);


    result.format = "properties";
    result.folder_lang = folder_lang;
    result.filename = fileName;

    while (!file.atEnd()) {
        QString line = file.readLine();
        line.replace(QRegExp("(\\s*)$"),"");

        // ignoring lines
        if (line.indexOf(QRegExp("^([#|;])(.*)"))!=-1) continue;

        QStringList pair = line.split(":");

        if (pair.at(0)=="author") result.author = pair.at(1);
        if (pair.at(0)=="language") result.language = pair.at(1);
        if (pair.at(0)=="visible") result.visible = pair.at(1);
        if (pair.at(0)=="inheritsFrom") result.inheritsFrom = pair.at(1);
        if (pair.at(0)=="speak_method") result.speak_method = pair.at(1);
        if (pair.at(0)=="espeak_params") result.espeak_params = pair.at(1);
        if (pair.at(0)=="visible") result.visible = pair.at(1);


        if (pair.size()==2){
            QStringList params = pair.at(1).split("=");
            if (params.size()<4) continue;


        }

    }


    return result;
}
