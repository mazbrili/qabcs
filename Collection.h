#ifndef COLLECTION_H
#define COLLECTION_H

#include <QString>
#include <QMap>
#include <QPixmap>
#include <QJsonObject>


struct LETTER_CONFIG {
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
        void setLetter(QString letter,QJsonObject params);
        void setLetter(QString letter,QString name,QString pic,QString sound_pic,QString speak_method,QString espeak_params,QString espeak_words,QString noises);
        void clear();
        void setAbcLanguage(QString abcLanguage);

        QString getPixmapFileName(QString letter);
        QString getName(QString letter);
        QString getSound(QString letter);

        QPixmap getPixmap(QString letter);

        void playSoundPicture(QString letter);

    private:
        QMap<QString,LETTER_CONFIG> listLetters;

        QString _abcLanguage;

        QString _speak_method;
        QString _espeak_params;

};

#endif // COLLECTION_H
