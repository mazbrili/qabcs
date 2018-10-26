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

void Collection::setGlobalParam(QJsonObject params){
    _speak_method = params.value("speak_method").toString();
    _espeak_params = params.value("espeak_params").toString();
}

void Collection::clear(){
    listLetters.clear();
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

    listCombinationspaths.push_back(GLOBAL_PATH_USERDATA+"/abcs/all/pics/"+listLetters[letter].pic);
    for (QString type:listTypes){
        listCombinationspaths.push_back(GLOBAL_PATH_USERDATA+"/abcs/all/pics/"+type+"/"+listLetters[letter].pic);
        listCombinationspaths.push_back(GLOBAL_PATH_USERDATA+"/abcs/all/pics/"+type+"/"+listLetters[letter].pic+".png");
        listCombinationspaths.push_back(GLOBAL_PATH_USERDATA+"/abcs/all/pics/"+type+"/"+listLetters[letter].pic+".jpg");
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
        QString filename = GLOBAL_PATH_USERDATA+"/abcs/"+_abcLanguage+"/sounds/words/"+listLetters[letter].sound_pic;
        QProcess::execute("play "+filename);
    }


    // play noises
    if (!listLetters[letter].noises.isEmpty()){
        QString filename = GLOBAL_PATH_USERDATA+"/abcs/all/noises/"+listLetters[letter].noises;
        QProcess::execute("play "+filename);
    }

}
