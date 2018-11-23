#include "SoundEngine.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>

#include "config_qabcs.h"
#include "LoaderAbcFormats.h"
#include "sndplayer.h"

QMediaPlayer *playerBackgroud=nullptr;

SoundEngine::SoundEngine(){

}

void SoundEngine::init(){
    playerBackgroud = new QMediaPlayer();
}

void SoundEngine::playSoundFromFile(QString filename, bool async){
    if (!QFile::exists(filename)) return;
    QFileInfo fileInfo(filename);

#if defined(_WIN32)
    QCoreApplication::instance()->processEvents();

    if (async){
        playerBackgroud->stop();
        playerBackgroud->setMedia(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
        playerBackgroud->play();
    }else{
        SndPlayer player(0,fileInfo.absoluteFilePath());
        player.wait();
    }
#else
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
#endif

}

void SoundEngine::playSoundFromSpeechSynthesizer(QString cmdLine, bool async){
    if (cmdLine.isEmpty()) return;

    qDebug() << "[DEBUG] " << cmdLine;

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



QString SoundEngine::findSoundFile(QString filename,QString soundFile,QString typeSearch){
    ABC_CONFIG config_current = LoaderAbcFormats::LoadFilename(filename);

    QString searchFolder="words";

    if (typeSearch=="letter"){
        searchFolder="alpha";
    }

    {
        QString folderWords = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+config_current.folder_lang+"/sounds/"+searchFolder;
        QString filename = SoundEngine::findSoundfile(folderWords,soundFile.toLower().replace(" ","_"));
        if (!filename.isEmpty() and QFile::exists(filename)){
            return filename;
        }
    }

    {
        ABC_CONFIG config_inherits;
        if (!config_current.inheritsFrom.isEmpty()){
            config_inherits = LoaderAbcFormats::LoadFilename( QString(GLOBAL_PATH_USERDATA)+"/abcs/"+config_current.inheritsFrom);
        }
        if (!config_inherits.filename.isEmpty()){
            QString folderForSearch = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+config_inherits.folder_lang+"/sounds/"+searchFolder;
            QString filename =  SoundEngine::findSoundfile(folderForSearch,soundFile.toLower().replace(" ","_"));
            if (!filename.isEmpty() and QFile::exists(filename)){
                return filename;
            }
        }
    }


    return "";
}


