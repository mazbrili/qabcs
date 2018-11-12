#include "SoundEngine.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>

#include "config_qabcs.h"

SoundEngine::SoundEngine(){

}

void SoundEngine::playSoundFromFile(QString filename, bool async){
    if (!QFile::exists(filename)) return;
    QFileInfo fileInfo(filename);

    QProcess proc;
    QString cmd_to_play = global_path_to_play+" "+fileInfo.absoluteFilePath();

    qDebug() << "[DEBUG] " << cmd_to_play;

#if defined(_WIN32)
    cmd_to_play=global_path_to_play+" \""+fileInfo.absoluteFilePath()+"\"  -t waveaudio";

    QStringList listEvn = proc.environment();
    listEvn.push_back("AUDIODEV=0");
    proc.setEnvironment(listEvn);

    QString workingDirectory = [](){
        QString path = global_path_to_play;
        path.replace("\"","");
        QStringList s = path.split(QRegExp("(/|\\\\)"));
        int lenFilename = s.at(s.size()-1).length();
        return path.mid(0,path.length()-lenFilename);
    }();


    proc.setWorkingDirectory(workingDirectory);
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

    if (QFile::exists(string)) return string;
    if (QFile::exists(folder+"/"+string)) return folder+"/"+string;

    for (QString ext:listExtensionFiles){
        QString filename = string+"."+ext;
        if (QFile::exists(folder+"/"+filename)){
            return folder+"/"+filename;
        }
    }

    return "";
}
