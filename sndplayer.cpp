#include "sndplayer.h"

#if defined(_WIN32)
#include <windows.h>
#endif


SndPlayer::SndPlayer(QObject *parent, const QString &file) : QThread(parent){
    fileName = file;
    mp = 0;

    moveToThread(this);

    connect(this, &QThread::started, this, &SndPlayer::open);
    connect(this, &QThread::finished, this, &SndPlayer::close);

    start();
}

SndPlayer::~SndPlayer(){
}

void SndPlayer::open() {
#if defined(_WIN32)
    CoInitialize(0);
#endif

    mp = new QMediaPlayer(this);
    connect(mp, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)), Qt::QueuedConnection);
    mp->setMedia(QUrl::fromLocalFile(fileName));
}

void SndPlayer::close(){
    if (mp){
        delete mp;
        mp = 0;
    }
#if defined(_WIN32)
    CoUninitialize();
#endif
}

void SndPlayer::mediaStatusChanged(QMediaPlayer::MediaStatus status){
    switch (status) {
        case QMediaPlayer::LoadedMedia:
            mp->play();
            break;
        case QMediaPlayer::InvalidMedia:
            exit(-1);
            break;
        case QMediaPlayer::EndOfMedia:
            exit(0);
            break;
        default:
            break;
    }
}
