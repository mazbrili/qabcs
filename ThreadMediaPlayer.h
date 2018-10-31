#ifndef THREADMEDIAPLAYER_H
#define THREADMEDIAPLAYER_H

#include <mutex>
#include <condition_variable>

#include <QMutex>
#include <QMediaPlayer>
#include <QThread>
#include <QEventLoop>

class ThreadMediaPlayer : public QThread{
    Q_OBJECT
    public:
        ThreadMediaPlayer();
        void run();

        void playSoundFromFile(QString filename,bool async);
        QMediaPlayer::State state();

    private:
        bool waitEvent();

        QEventLoop loop;

        QMutex mtx;
        std::mutex mutexMediaPlayer;
        std::condition_variable cvMediaPlayer;

        QMediaPlayer *mediaPlayer;
        QString _filename;

    private slots:
        void mediaStatusChanged(QMediaPlayer::MediaStatus);
};

#endif // THREADMEDIAPLAYER_H
