#ifndef COLLECTION_H
#define COLLECTION_H

#include <QString>
#include <QMap>
#include <QPixmap>
#include <QJsonObject>


struct LETTER_CONFIG {
    QString letter;
    QString name;
    QString pic;
    QString sound_pic;
    QString speak_method;
    QString espeak_params;
    QString espeak_words;
    QString noises;
};


class Collection{
    public:
        explicit Collection(QString abcLanguage);

        void setGlobalParam(QJsonObject params);
        void setGlobalParam(QString speak_method,QString espeak_params);
        void setLastFileName(QString filename);
        void setLetter(QString letter, QString folderLang, QJsonObject params);
        void setLetter(QString letter, QString folderLang, LETTER_CONFIG config);
        void setLetter(QString letter, QString folderLang, QString name,QString pic,QString sound_pic,QString speak_method,QString espeak_params,QString espeak_words,QString noises);
        void clearLetter(QString letter);

        int countLetters();

        void clear();
        void setAbcLanguage(QString abcLanguage);

        QString getPixmapFileName(QString letter);
        QString getName(QString letter);
        QString getSound(QString letter);

        QString getPixmap(QString letter);

        LETTER_CONFIG getLetterConfig(QString letter);

        void playSoundPicture(QString letter,bool playNoises=true);
        void playSoundNoises(QString letter);

private:
        QMap<QString,LETTER_CONFIG> listLetters;

        QString _abcLanguage;

        QString _speak_method;
        QString _espeak_params;

        QString _lastFileName;

};

#endif // COLLECTION_H
