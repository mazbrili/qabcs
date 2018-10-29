#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QString>

class SoundEngine {
    public:
        SoundEngine();
        static void playSoundFromFile(QString filename, bool async);
        static void playSoundFromSpeechSynthesizer(QString cmdLine, bool async);
        static QString findSoundfile(QString folder, QString string);
};

#endif // SOUNDENGINE_H
