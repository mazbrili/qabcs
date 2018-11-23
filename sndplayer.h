#ifndef SNDPLAYER_H
#define SNDPLAYER_H

#include <QThread>
#include <QMediaPlayer>

class SndPlayer : public QThread
{
    Q_OBJECT

public:
    SndPlayer(QObject *parent, const QString &file);
    ~SndPlayer();

protected slots:
    void open();
    void close();
    void mediaStatusChanged(QMediaPlayer::MediaStatus);

private:
    QString fileName;
    QMediaPlayer *mp;

};

#endif // SNDPLAYER_H
