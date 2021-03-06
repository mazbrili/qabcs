#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QKeyEvent>
#include <QMap>
#include <QSettings>
#include <QMediaPlayer>
#include <QStatusBar>
#include <QLabel>
#include <QTranslator>
#include <QMovie>

#include "Collection.h"


namespace Ui {
class MainWindow;
}

enum TYPE_GAME {
    TYPE_ABC,
    TYPE_RAND,
    TYPE_FOOD,
    TYPE_ANIMALS,
    TYPE_INSTRUMENT,
    TYPE_TOYS
};

struct LETTER_INFO {
    QString letter;
    QString sound_letter;
    QString speak_method;
    QString espeak_params;
    QString espeak_words;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        void initGUI();
        void initToolBar();
        void initLanguageAbc();

        void refreshTranslate();

        void loadAbcConfig(QString abcFilename);
        bool loadAbcConfigJson(QString filename);
        bool loadAbcConfigProperties(QString filename);

        QString typeGameToString(TYPE_GAME type);

        void setAbcLang(QString lang, QString filename);
        void readLetterVariants();
        void refreshViewer();        
        void setLetterAndText(QString letter, QString text);
        void setPixmapViewer(QPixmap pixmap);
        void setPixmapViewer(QString filename);
        void normalPixmapViewer();

        void updateletterToList(QString folder_lang, LETTER_INFO letter);
        void playSoundLetter(QString letter, bool async=false);

        bool isKeypressLetter(QString letter,QString key);

        bool isExistSox();
        bool isExistEspeak();

        // for random type of game
        int gameRandomGenerateNextIndex();

        QString globalPathUserResources;

        QStatusBar *statusbar;
        QToolBar *toolBar;
        QLabel *lblAbcPicture;
        QMovie *moviePicture;
        QLabel *lblAbcLetter;
        QLabel *lblAbcText;

        QLabel *blockForm;


        QActionGroup *typeGameGroup;

        QAction *accGameAbc;
        QAction *accGameRand;
        QAction *accGameAnimals;
        QAction *accGameFood;
        QAction *accGameInstrument;
        QAction *accGameToys;
        QAction *accAlphabetTable;

        QAction *accSound;
        QAction *accLang;
        QAction *accTyping;
        QAction *accHelp;
        QAction *accInfo;
        QAction *accExit;

        TYPE_GAME typeGame;
        int currentIndexLetter;

        QString currentLanguageAbc;
        QString currentFilenameAbc;
        QMap<QString,QVector<QString>> listLetterVariants;

        QSettings *confSettings;

        bool soundStatus;
        QMediaPlayer soundEffect;

        bool blockButtonTyping;

        bool gameAbcFinish;

        // for random type of game
        int gameRandomCurrentIndex;
        QVector<LETTER_CONFIG> listLettersGameRand;

        QString _speak_method;
        QString _espeak_params;
        QString _view_letters;
        bool _disable_additional_keys;
        QString _lastFormatLoaded;
        QVector<LETTER_INFO> listLetters;

        QStringList listTypes;
        QMap<QString,Collection*> listCollections;

        QTranslator translator;
        QTranslator qtTranslator;

        QMap<QWidget*,QMap<QString,QString>> listWidgetsRetranslateUi;
        QMap<QAction*,QMap<QString,QString>> listActionsRetranslateUi;

    private:
        bool eventFilter(QObject *obj, QEvent *event);

    protected:
       virtual void keyPressEvent(QKeyEvent *event);

    private slots:
        void clickButtonGameAbc();
        void clickButtonGameRand();
        void clickButtonGameAnimals();
        void clickButtonGameFood();
        void clickButtonGameInstrument();
        void clickButtonGameToys();
        void clickButtonSound();
        void clickButtonTyping();
        void clickButtonAlphabetTable();
        void clickButtonLang();
        void clickButtonHelp();
        void clickButtonInfo();

};

#endif // MAINWINDOW_H
