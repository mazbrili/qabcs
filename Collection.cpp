#include "Collection.h"

#include <QFile>

#include "config_qabcs.h"
#include "SoundEngine.h"

Collection::Collection(QString abcLanguage) : _abcLanguage(abcLanguage){

}

void Collection::setLetter(QString letter,QString folderLang,QJsonObject params){
    QString name = (params.value("name").isString()) ? params.value("name").toString().toUpper() : QString();
    QString pic = (params.value("pic").isString()) ? params.value("pic").toString() : QString();
    QString sound_pic = (params.value("sound_pic").isString()) ? params.value("sound_pic").toString() : QString();
    QString speak_method = (params.value("speak_method").isString()) ? params.value("speak_method").toString() : QString();
    QString espeak_params = (params.value("espeak_params").isString()) ? params.value("espeak_params").toString() : QString();
    QString espeak_words = (params.value("espeak_words").isString()) ? params.value("espeak_words").toString() : QString();
    QString noises = (params.value("noises").isString()) ? params.value("noises").toString() : QString();

    setLetter(letter,folderLang,name,pic,sound_pic,speak_method,espeak_params,espeak_words,noises);
}


void Collection::setLetter(QString letter, QString folderLang, LETTER_CONFIG config){
    setLetter(letter,folderLang,config.name,config.pic,config.sound_pic,config.speak_method,config.espeak_params,config.espeak_words,config.noises);
}

void Collection::setLetter(QString letter,QString folderLang,QString name,QString pic,QString sound_pic,QString speak_method,QString espeak_params,QString espeak_words,QString noises){

    if (listLetters.contains(letter)){
        if (name.isEmpty()) name=listLetters[letter].name;
        if (pic.isEmpty()) pic=listLetters[letter].pic;
        if (sound_pic.isEmpty()) sound_pic=listLetters[letter].sound_pic;
        if (speak_method.isEmpty()) speak_method=listLetters[letter].speak_method;
        if (espeak_params.isEmpty()) espeak_params=listLetters[letter].espeak_params;
        if (espeak_words.isEmpty()) espeak_words=listLetters[letter].espeak_words;
        if (noises.isEmpty()) noises=listLetters[letter].noises;
    }


    // find sounds file for letter
    if (!sound_pic.isEmpty() and !folderLang.isEmpty()){
        QString folderAlpha = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+folderLang+"/sounds/words";
        QString letterSoundLetterFilename =  SoundEngine::findSoundfile(folderAlpha,sound_pic.toLower());
        if (!letterSoundLetterFilename.isEmpty() and QFile::exists(letterSoundLetterFilename)){
            sound_pic=letterSoundLetterFilename;
        }else{
            if (listLetters.contains(letter)) sound_pic=listLetters[letter].sound_pic;
        }
    }else{
        if (listLetters.contains(letter)) sound_pic=listLetters[letter].sound_pic;
    }

    listLetters[letter]={letter,name,pic,sound_pic,speak_method,espeak_params,espeak_words,noises};
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

        if (!filename.isEmpty() and QFile::exists(filename)){
            SoundEngine::playSoundFromFile(filename);
        }else{
            QString words = listLetters[letter].espeak_words;
            if (words.isEmpty()) words=letter;
            SoundEngine::playSoundFromSpeechSynthesizer("espeak "+espeak_params+" \""+words+"\"");
        }
    }else{
        QString folderWords = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+_abcLanguage+"/sounds/words";
        QString filename = SoundEngine::findSoundfile(folderWords,listLetters[letter].sound_pic.toLower().replace(" ","_"));
        SoundEngine::playSoundFromFile(filename);
    }

    // play noises
    if (!listLetters[letter].noises.isEmpty()){
        QString folderNoises =  QString(GLOBAL_PATH_USERDATA)+"/abcs/all/noises";
        QString filename = SoundEngine::findSoundfile(folderNoises,listLetters[letter].noises.toLower());
        SoundEngine::playSoundFromFile(filename);
    }

}
