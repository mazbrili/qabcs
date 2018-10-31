#include "SoundEngine.h"

#include <QFile>
#include <QFileInfo>
#include <QProcess>

#include "ThreadMediaPlayer.h"

ThreadMediaPlayer *mediaPlayer=nullptr;

SoundEngine::SoundEngine(){

}

void SoundEngine::init(){
    mediaPlayer = new ThreadMediaPlayer;
    mediaPlayer->start();
}

QMediaPlayer::State SoundEngine::state(){
    return mediaPlayer->state();
}

void SoundEngine::playSoundFromFile(QString filename, bool async){
    if (!QFile::exists(filename)) return;

    QFileInfo soundFile(filename);
    mediaPlayer->playSoundFromFile(soundFile.absoluteFilePath(),async);
}

void SoundEngine::playSoundFromSpeechSynthesizer(QString cmdLine, bool async){
    if (cmdLine.isEmpty()) return;

    if (async){
        QProcess::startDetached(cmdLine);
    }else{
        QProcess::execute(cmdLine);
    }
}

QString SoundEngine::findSoundfile(QString folder,QString string){
    QStringList listExtensionFiles = QStringList() << "wav" << "ogg" << "mp3";

    string = string.toLower();

    if (QFile::exists(folder+"/"+string)) return string;

    for (QString ext:listExtensionFiles){
        QString filename = string+"."+ext;
        if (QFile::exists(folder+"/"+filename)){
            return filename;
        }
    }

    return "";
}
