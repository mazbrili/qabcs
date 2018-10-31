#include "ThreadMediaPlayer.h"

#include <chrono>
#include <QFileInfo>

ThreadMediaPlayer::ThreadMediaPlayer(){
    mediaPlayer = new QMediaPlayer;
    connect(mediaPlayer,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));

}

void ThreadMediaPlayer::run(){
    while(waitEvent()){
        mtx.lock();
        if (!_filename.isEmpty()){

            QString filename=_filename;
            _filename.clear();
            QFileInfo fileInfo(filename);
            mediaPlayer->setMedia(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
            mediaPlayer->play();
        }
        mtx.unlock();
    }
}

QMediaPlayer::State ThreadMediaPlayer::state(){
    return mediaPlayer->state();
}

void ThreadMediaPlayer::playSoundFromFile(QString filename,bool async){
    if (filename.isEmpty()) return;
    if (!QFile::exists(filename)) return;

    mtx.lock();
    _filename =filename;
    mtx.unlock();

    cvMediaPlayer.notify_all();

    if (!async) loop.exec();
}

bool ThreadMediaPlayer::waitEvent(){
    std::unique_lock<std::mutex> lkm(mutexMediaPlayer);
    cvMediaPlayer.wait_for(lkm,std::chrono::milliseconds(1000));
    return true;
}

void ThreadMediaPlayer::mediaStatusChanged(QMediaPlayer::MediaStatus status){
    if (status==QMediaPlayer::EndOfMedia){
        loop.exit();
    }
}
