#include "Collection.h"

#include <QFile>

#include "config_qabcs.h"
#include "SoundEngine.h"

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

    listLetters[letter]={letter,name,pic,sound_pic,speak_method,espeak_params,espeak_words,noises};
}

void Collection::setLetter(QString letter,QString name,QString pic,QString sound_pic,QString speak_method,QString espeak_params,QString espeak_words,QString noises){
    listLetters[letter]={letter,name,pic,sound_pic,speak_method,espeak_params,espeak_words,noises};
}

void Collection::setLetter(QString letter,LETTER_CONFIG config){
    listLetters[letter]={letter,config.name,config.pic,config.sound_pic,config.speak_method,config.espeak_params,config.espeak_words,config.noises};
}

void Collection::setGlobalParam(QJsonObject params){
    if (!params.value("speak_method").isNull()){
        _speak_method = params.value("speak_method").toString();
    }
    if (!params.value("espeak_params").isNull()){
        _espeak_params = params.value("espeak_params").toString();
    }
}

void Collection::setGlobalParam(QString speak_method,QString espeak_params){
    _speak_method = speak_method;
    _espeak_params = espeak_params;
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

    listCombinationspaths.push_back(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/pics/"+listLetters[letter].pic.toLower());
    for (QString type:listTypes){
        listCombinationspaths.push_back(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/pics/"+type+"/"+listLetters[letter].pic.toLower());
        for (QString ext:listExtensionFiles){
            listCombinationspaths.push_back(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/pics/"+type+"/"+listLetters[letter].pic.toLower()+"."+ext);
        }
    }

    for (QString filename:listCombinationspaths){
        if (QFile::exists(filename)) return QPixmap(filename);
    }

    return QPixmap();
}

LETTER_CONFIG Collection::getLetterConfig(QString letter){
    return listLetters[letter];
}

void Collection::playSoundPicture(QString letter){
    QString speak_method = (listLetters[letter].speak_method.isEmpty()) ? _speak_method : listLetters[letter].speak_method;
    QString espeak_params = (listLetters[letter].espeak_params.isEmpty()) ? _espeak_params : listLetters[letter].espeak_params;   

    if (speak_method=="espeak"){
        if (!listLetters[letter].espeak_words.isEmpty()){
            SoundEngine::playSoundFromSpeechSynthesizer("espeak "+espeak_params+" \""+listLetters[letter].espeak_words+"\"");
        }

    } else if (speak_method=="properties"){
        QString folderWords = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+_abcLanguage+"/sounds/words";
        QString filename = SoundEngine::findSoundfile(folderWords,listLetters[letter].sound_pic.toLower().replace(" ","_"));

        if (QFile::exists(folderWords+"/"+filename)){
            SoundEngine::playSoundFromFile(folderWords+"/"+filename);
        }else{
            QString words = listLetters[letter].espeak_words;
            if (words.isEmpty()) words=letter;
            SoundEngine::playSoundFromSpeechSynthesizer("espeak "+espeak_params+" \""+words+"\"");
        }
    }else{
        QString folderWords = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+_abcLanguage+"/sounds/words";
        QString filename = SoundEngine::findSoundfile(folderWords,listLetters[letter].sound_pic.toLower().replace(" ","_"));
        SoundEngine::playSoundFromFile(folderWords+"/"+filename);
    }

    // play noises
    if (!listLetters[letter].noises.isEmpty()){
        QString folderNoises =  QString(GLOBAL_PATH_USERDATA)+"/abcs/all/noises";
        QString filename = SoundEngine::findSoundfile(folderNoises,listLetters[letter].noises.toLower());
        SoundEngine::playSoundFromFile(folderNoises+"/"+filename);
    }

}
