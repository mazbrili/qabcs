#include "SoundEngine.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>

SoundEngine::SoundEngine(){

}

void SoundEngine::playSoundFromFile(QString filename, bool async){
    if (!QFile::exists(filename)) return;
    QFileInfo fileInfo(filename);

    QProcess proc;
    QString cmd_to_play = "play "+fileInfo.absoluteFilePath();

#if defined(_WIN32)
    cmd_to_play=QApplication::applicationDirPath()+"/3rdparty/sox/play.exe \""+fileInfo.absoluteFilePath()+"\"  -t waveaudio";

    QStringList listEvn = proc.environment();
    listEvn.push_back("AUDIODEV=0");
    proc.setEnvironment(listEvn);

    proc.setWorkingDirectory(QApplication::applicationDirPath()+"/3rdparty/sox/");
#endif

    if (async){
        proc.startDetached(cmd_to_play);
    }else{
        proc.execute(cmd_to_play);
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
