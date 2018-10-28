#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QKeyEvent>
#include <QMap>
#include <QSettings>
#include <QSoundEffect>
#include <QStatusBar>
#include <QLabel>

#include "Collection.h"


namespace Ui {
class MainWindow;
}

enum TYPE_GAME {
    TYPE_ABC,
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

        bool loadAbcConfigJson(QString filename);
        bool loadAbcConfigProperties(QString filename);

        QString typeGameToString(TYPE_GAME type);

        void setAbcLang(QString lang);
        void refreshViewer();
        void setPixmapViewer(QPixmap pixmap);

        void playSoundLetter(QString letter, bool async=false);

        QString globalPathUserResources;

        QStatusBar *statusbar;
        QToolBar *toolBar;
        QLabel *lblAbcPicture;
        QLabel *lblAbcLetter;
        QLabel *lblAbcText;


        QActionGroup *typeGameGroup;

        QAction *accAbc;
        QAction *accAnimals;
        QAction *accFood;
        QAction *accInstrument;
        QAction *accToys;

        QAction *accSound;
        QAction *accLang;
        QAction *accHelp;
        QAction *accInfo;
        QAction *accExit;

        TYPE_GAME typeGame;
        int currentIndexLetter;

        QString currentLanguageAbc;

        QSettings *confSettings;

        QSoundEffect soundEffect;

        bool gameAbcFinish;


        QString _speak_method;
        QString _espeak_params;
        QVector<LETTER_INFO> listLetters;

        QStringList listTypes;
        QMap<QString,Collection*> listCollections;

    protected:
       virtual void keyPressEvent(QKeyEvent *event);

    private slots:
        void clickButtonAbc();
        void clickButtonAnimals();
        void clickButtonFood();
        void clickButtonInstrument();
        void clickButtonToys();
        void clickButtonSound();
        void clickButtonLang();
        void clickButtonHelp();
        void clickButtonInfo();

};

#endif // MAINWINDOW_H
