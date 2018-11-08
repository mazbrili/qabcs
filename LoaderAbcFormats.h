#ifndef LOADERABCFORMATS_H
#define LOADERABCFORMATS_H

#include <QString>
#include <QVector>
#include <QMap>

struct ABC_CONFIG_LETTER {
    QString letter;
    QString sound_letter;
    QString speak_method;
    QString espeak_params;
    QString espeak_words;
};

struct ABC_CONFIG_GAME {
    QString pic;
    QString name;
    QString sound_pic;
    QString speak_method;
    QString espeak_words;
    QString espeak_params;
    QString noises;
};

struct ABC_CONFIG_ALPHA {
    ABC_CONFIG_LETTER config_letter;
    QMap<QString,ABC_CONFIG_GAME> games;
};

struct ABC_CONFIG {
    QString format;
    QString folder_lang;
    QString filename;
    QString language;
    QString author;
    QString speak_method;
    QString espeak_params;
    QString inheritsFrom;
    QString visible;

    QVector<ABC_CONFIG_ALPHA> letters;
};

class LoaderAbcFormats {
    public:
        static ABC_CONFIG LoadFilename(const QString &fileName);
        static ABC_CONFIG loadAbcConfigJson(const QString &fileName);
        static ABC_CONFIG loadAbcConfigProperties(const QString &fileName);
};

#endif // LOADERABCFORMATS_H
