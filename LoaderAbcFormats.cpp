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


    QStringList listTypes = QStringList() << "misc" << "rand" << "food" << "animals" << "instrument" << "toys";

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
    result.view_letters = (root_general.value("view_letters").isString()) ? root_general.value("view_letters").toString(): QString("true");



    QJsonArray arrLetters = root.value("letters").toArray();
    for (int i=0;i<arrLetters.size();i++){
        QString letter = arrLetters.at(i).toObject().keys().at(0);
        QJsonObject objLetter = arrLetters.at(i).toObject().value(letter).toObject();

        ABC_CONFIG_ALPHA configAlpha;

        QString sound_letter = (objLetter.value("sound_letter").isString()) ? objLetter.value("sound_letter").toString().toUpper() : QString();
        QString speak_method = (objLetter.value("speak_method").isString()) ? objLetter.value("speak_method").toString().toUpper() : QString();
        QString espeak_params = (objLetter.value("espeak_params").isString()) ? objLetter.value("espeak_params").toString().toUpper() : QString();
        QString espeak_words = (objLetter.value("espeak_words").isString()) ? objLetter.value("espeak_words").toString().toUpper() : QString();

        configAlpha.letter=letter;
        configAlpha.sound_letter=sound_letter;
        configAlpha.speak_method=speak_method;
        configAlpha.espeak_params=espeak_params;
        configAlpha.espeak_words=espeak_words;

        for (QString type:listTypes){
            QJsonObject objTypeLetter = objLetter.value(type).toObject();

            QString name = (objTypeLetter.value("name").isString()) ? objTypeLetter.value("name").toString().toUpper() : QString();
            QString pic = (objTypeLetter.value("pic").isString()) ? objTypeLetter.value("pic").toString() : QString();
            QString sound_pic = (objTypeLetter.value("sound_pic").isString()) ? objTypeLetter.value("sound_pic").toString() : QString();
            QString speak_method = (objTypeLetter.value("speak_method").isString()) ? objTypeLetter.value("speak_method").toString() : QString();
            QString espeak_params = (objTypeLetter.value("espeak_params").isString()) ? objTypeLetter.value("espeak_params").toString() : QString();
            QString espeak_words = (objTypeLetter.value("espeak_words").isString()) ? objTypeLetter.value("espeak_words").toString() : QString();
            QString noises = (objTypeLetter.value("noises").isString()) ? objTypeLetter.value("noises").toString() : QString();

            configAlpha.games[type].name=name;
            configAlpha.games[type].pic=pic;
            configAlpha.games[type].sound_pic=sound_pic;
            configAlpha.games[type].speak_method=speak_method;
            configAlpha.games[type].espeak_params=espeak_params;
            configAlpha.games[type].espeak_words=espeak_words;
            configAlpha.games[type].noises=noises;
        }

        result.letters.push_back(configAlpha);
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
        if (pair.at(0)=="view_letters") result.view_letters = pair.at(1);


        if (pair.size()==2){
            QStringList params = pair.at(1).split("=");
            if (params.size()<4) continue;

            QString noises = "";
            QString type = pair.at(0);

            QString letter = params.at(0);
            QString str = params.at(1).toUpper();
            QString espeak_words = params.at(2);
            QString metka = params.at(3);
            if (params.size()>=5) noises=params.at(4);

            QString name = str;
            QString pic = metka;
            QString sound_pic = letter.toLower();
            QString espeak_params = "";

            int indexLetter=-1;
            for (int inx=0;inx<result.letters.size();inx++) {
                if (result.letters.at(inx).letter.toUpper()==letter.toUpper()){
                    indexLetter=inx;
                    break;
                }
            }


            if (indexLetter==-1){
                ABC_CONFIG_ALPHA configAlpha;
                configAlpha.letter=letter;
                configAlpha.sound_letter=letter;
                configAlpha.speak_method="";
                configAlpha.espeak_params="";
                configAlpha.espeak_words=letter;

                result.letters.push_back(configAlpha);
                indexLetter=result.letters.size()-1;
            }


            ABC_CONFIG_ALPHA configAlpha=result.letters.at(indexLetter);

            configAlpha.games[type].name=name;
            configAlpha.games[type].pic=pic;
            configAlpha.games[type].sound_pic=sound_pic;
            configAlpha.games[type].speak_method="";
            configAlpha.games[type].espeak_params="";
            configAlpha.games[type].espeak_words=espeak_words;
            configAlpha.games[type].noises=noises;

            result.letters[indexLetter]=configAlpha;
        }

    }


    return result;
}

QString LoaderAbcFormats::upperString(QString string){

    string.replace("ß","[C39F]");

    string = string.toUpper();

    string.replace("[C39F]","ẞ");

    return string;
}
