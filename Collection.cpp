#include "Collection.h"

#include <QProcess>
#include <QFile>
#include "config_qabcs.h"

Collection::Collection(QString abcLanguage) : _abcLanguage(abcLanguage){

}

void Collection::setLetter(QString letter,QJsonObject params){
    QString name = params.value("name").toString().toUpper();
    QString pic = params.value("pic").toString();
    QString sound_pic = params.value("sound_pic").toString();
    QString speak_method = params.value("speak_method").toString();
    QString espeak_params = params.value("espeak_params").toString();
    QString espeak_words = params.value("espeak_words").toString();
    QString noises = params.value("noises").toString();

    listLetters[letter]={name,pic,sound_pic,speak_method,espeak_params,espeak_words,noises};
}

void Collection::setLetter(QString letter,QString name,QString pic,QString sound_pic,QString speak_method,QString espeak_params,QString espeak_words,QString noises){
    listLetters[letter]={name,pic,sound_pic,speak_method,espeak_params,espeak_words,noises};
}


void Collection::setGlobalParam(QJsonObject params){
    _speak_method = params.value("speak_method").toString();
    _espeak_params = params.value("espeak_params").toString();
}

void Collection::clear(){
    listLetters.clear();

    _speak_method="";
    _espeak_params="";
}

void Collection::setAbcLanguage(QString abcLanguage){
    _abcLanguage=abcLanguage;
}

QString Collection::getPixmapFileName(QString letter){
    return listLetters[letter].pic;
}

QString Collection::getName(QString letter){
    return listLetters[letter].name;

}

QString Collection::getSound(QString letter){
    return listLetters[letter].sound_pic;
}

QPixmap Collection::getPixmap(QString letter){
    QStringList listCombinationspaths;

    QStringList listTypes = QStringList() << "misc" << "food" << "animals" << "music" << "toys";
    QStringList listExtensionFiles = QStringList() << "png" << "jpg" << "svg";

    listCombinationspaths.push_back(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/pics/"+listLetters[letter].pic);
    for (QString type:listTypes){
        listCombinationspaths.push_back(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/pics/"+type+"/"+listLetters[letter].pic);
        for (QString ext:listExtensionFiles){
            listCombinationspaths.push_back(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/pics/"+type+"/"+listLetters[letter].pic+"."+ext);
        }
    }

    for (QString filename:listCombinationspaths){
        if (QFile::exists(filename)) return QPixmap(filename);
    }

    return QPixmap();
}

void Collection::playSoundPicture(QString letter){
    QString speak_method = (listLetters[letter].speak_method.isEmpty()) ? _speak_method : listLetters[letter].speak_method;
    QString espeak_params = (listLetters[letter].espeak_params.isEmpty()) ? _espeak_params : listLetters[letter].espeak_params;   

    if (speak_method=="espeak"){
        if (!listLetters[letter].espeak_words.isEmpty()){
            QProcess::execute("espeak "+espeak_params+" \""+listLetters[letter].espeak_words+"\"");
        }
    }else{
        QString filename = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+_abcLanguage+"/sounds/words/"+listLetters[letter].sound_pic;
        if (QFile::exists(filename)) QProcess::execute("play "+filename);
    }


    // play noises
    if (!listLetters[letter].noises.isEmpty()){
        QString filename = QString(GLOBAL_PATH_USERDATA)+"/abcs/all/noises/"+listLetters[letter].noises;
        if (QFile::exists(filename)) QProcess::execute("play "+filename);
    }

}
