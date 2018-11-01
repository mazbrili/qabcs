#include "SoundEngine.h"

#include <QFile>
#include <QFileInfo>
#include <QProcess>

SoundEngine::SoundEngine(){

}

void SoundEngine::playSoundFromFile(QString filename, bool async){
    if (!QFile::exists(filename)) return;

    if (async){
        QProcess::startDetached("play "+filename);
    }else{
        QProcess::execute("play "+filename);
    }
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
