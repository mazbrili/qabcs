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
};


class Collection{
    public:
        explicit Collection(QString pathUserResources);

        void setGlobalParam(QJsonObject params);
        void setLetter(QString letter,QJsonObject params);
        void clear();
        void setPathUserResources(QString pathUserResources);

        QString getPixmapFileName(QString letter);
        QString getName(QString letter);
        QString getSound(QString letter);

        QPixmap getPixmap(QString letter);

        void playSoundPicture(QString letter, bool async=false);

    private:
        QMap<QString,LETTER_CONFIG> listLetters;

        QString _pathUserResources;

        QString _speak_method;
        QString _espeak_params;

};

#endif // COLLECTION_H
