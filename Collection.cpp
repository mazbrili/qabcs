#include "Collection.h"

#include <QProcess>

Collection::Collection(QString pathUserResources) : _pathUserResources(pathUserResources){

}

void Collection::setLetter(QString letter,QJsonObject params){
    QString name = params.value("name").toString();
    QString pic = params.value("pic").toString();
    QString sound_pic = params.value("sound_pic").toString();
    QString speak_method = params.value("speak_method").toString();
    QString espeak_params = params.value("espeak_params").toString();
    QString espeak_words = params.value("espeak_words").toString();

    listLetters[letter]={name,pic,sound_pic,speak_method,espeak_params,espeak_words};
}

void Collection::setGlobalParam(QJsonObject params){
    _speak_method = params.value("speak_method").toString();
    _espeak_params = params.value("espeak_params").toString();
}

void Collection::clear(){
    listLetters.clear();
}

void Collection::setPathUserResources(QString pathUserResources){
    _pathUserResources=pathUserResources;
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
    return QPixmap(_pathUserResources+"/"+listLetters[letter].pic);
}

void Collection::playSoundPicture(QString letter,bool async){
    QString speak_method = (listLetters[letter].speak_method.isEmpty()) ? _speak_method : listLetters[letter].speak_method;
    QString espeak_params = (listLetters[letter].espeak_params.isEmpty()) ? _espeak_params : listLetters[letter].espeak_params;

    if (speak_method=="espeak"){

    }else{
        QString filename = _pathUserResources+"/"+listLetters[letter].sound_pic;
        if (async){
            QProcess::startDetached("play "+filename);
        }else{
            QProcess::execute("play "+filename);
        }
    }

}
