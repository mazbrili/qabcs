#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QString>

#include "ThreadMediaPlayer.h"

class SoundEngine {
    public:
        SoundEngine();
        static void init();
        static void playSoundFromFile(QString filename, bool async=false);
        static void playSoundFromSpeechSynthesizer(QString cmdLine, bool async=false);
        static QString findSoundfile(QString folder, QString string);

        static QMediaPlayer::State state();

};

#endif // SOUNDENGINE_H
