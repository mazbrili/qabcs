#include "mainwindow.h"

#include "config_qabcs.h"
#include "FormAbout.h"
#include "FormHelp.h"
#include "FormSelectLanguage.h"
#include "SoundEngine.h"
#include "LoaderAbcFormats.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QRegExp>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QTranslator>
#include <QLibraryInfo>

QString global_path_to_espeak;
QString global_path_to_play;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setFixedSize(645,645);
    this->setContextMenuPolicy(Qt::PreventContextMenu);
    this->setWindowIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"));

    currentIndexLetter=0;   
    gameAbcFinish=false;
    _speak_method="";
    _espeak_params="";
    _disable_additional_keys=true;
    soundStatus=true;       // sound on
    blockButtonTyping=false;    //

    listTypes = QStringList() << "misc" << "rand" << "food" << "animals" << "instrument" << "toys";

    // open ini user config
    QDir dirConfig(QDir::homePath()+"/.qabcs/");
    if (dirConfig.exists()==false) dirConfig.mkpath(QDir::homePath()+"/.qabcs/");
    confSettings = new QSettings(dirConfig.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    confSettings->setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());

    // init collections
    for (QString type:listTypes) listCollections[type] = new Collection(confSettings->value("abc/language","en").toString());

    SoundEngine::init();

    initGUI();
    initToolBar();

    refreshTranslate();

    qDebug() << tr("Using path:")+" "+QString(GLOBAL_PATH_USERDATA);

    // read status of sound from config
    if (confSettings->value("global/sound","true").toString()=="false"){
        accSound->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/sound_off.png"));
        soundStatus=false;
    }

    // read status of typing from config
    if (confSettings->value("global/typing","true").toString()=="false"){
        accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_off.png"));
        _disable_additional_keys=false;
    }

    if (isExistSox()==false){
        QMessageBox::critical(this,"qABCs",tr("the play command from the sox package was not found. Sound will be muted."));
        accSound->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/sound_off.png"));
        soundStatus=false;
        accSound->setEnabled(false);
    }

    if (isExistEspeak()==false){
        QMessageBox::critical(this,"qABCs",tr("espeak was not found. Some features will not be available."));
    }

    setAbcLang(confSettings->value("abc/language","en").toString(),confSettings->value("abc/filename","abc.json").toString());
}

MainWindow::~MainWindow(){

}

void MainWindow::initGUI(){

    statusbar = new QStatusBar(this);
    this->setStatusBar(statusbar);


    QFont fontSizeLetter;
    fontSizeLetter.setPointSize(45);

    QFont fontSizeText;
    fontSizeText.setPointSize(20);

    lblAbcPicture =  new QLabel(this);
    lblAbcPicture->setGeometry(QRect(130, 20, 290, 290));
    lblAbcPicture->setScaledContents(true);
    //lblAbcPicture->setFrameShape(QFrame::Box);

    lblAbcText = new QLabel(this);
    lblAbcText->setFont(fontSizeText);
    lblAbcText->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    lblAbcText->setGeometry(0, this->height()-statusbar->height()-101, this->width(), 91);
    //lblAbcText->setFrameShape(QFrame::Box);

    lblAbcLetter = new QLabel(this);
    lblAbcLetter->setFont(fontSizeLetter);
    lblAbcLetter->setAlignment(Qt::AlignCenter);
    lblAbcLetter->setGeometry(0,lblAbcText->y()-50,this->width(),101);
    //lblAbcLetter->setFrameShape(QFrame::Box);

    blockForm = new QLabel(this);
    blockForm->setGeometry(0,0,this->width(),this->height());
    blockForm->hide();
}


void MainWindow::initToolBar(){
    toolBar = new QToolBar("User");
    toolBar->setMovable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    addToolBar(Qt::TopToolBarArea, toolBar);

    toolBar->setIconSize(QSize(40, 40));

    typeGameGroup = new QActionGroup(this);

    accGameAbc = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"), tr("Find the letter on the keyboard (alphabetically)"), this);
    accGameAbc->setStatusTip(tr("Find the letter on the keyboard (alphabetically)"));
    accGameAbc->setCheckable(true);
    accGameAbc->setActionGroup(typeGameGroup);

    accGameRand = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/random.png"), tr("Find the letter on the keyboard (randomly)"), this);
    accGameRand->setStatusTip(tr("Find the letter on the keyboard (randomly)"));
    accGameRand->setCheckable(true);
    accGameRand->setActionGroup(typeGameGroup);

    accGameFood = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/food.png"), tr("Show foods for each letter"), this);
    accGameFood->setStatusTip(tr("Show foods for each letter"));
    accGameFood ->setCheckable(true);
    accGameFood->setActionGroup(typeGameGroup);

    accGameAnimals = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/animals.png"), tr("Show animals for each letter"), this);
    accGameAnimals->setStatusTip(tr("Show animals for each letter"));
    accGameAnimals->setCheckable(true);
    accGameAnimals->setActionGroup(typeGameGroup);

    accGameInstrument = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/instrument.png"), tr("Show musical instruments for each letter"), this);
    accGameInstrument->setStatusTip(tr("Show musical instruments for each letter"));
    accGameInstrument->setCheckable(true);
    accGameInstrument->setActionGroup(typeGameGroup);

    accGameToys = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/toys.png"), tr("Show toys for each letter"), this);
    accGameToys->setStatusTip(tr("Show toys for each letter"));
    accGameToys->setCheckable(true);
    accGameToys->setActionGroup(typeGameGroup);

    accSound = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/sound_on.png"), tr("Sound on/off"), this);
    accSound->setStatusTip(tr("Sound on/off"));

    accTyping = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_on.png"), tr("Typing on/off"), this);
    accTyping->setStatusTip(tr("Typing on/off"));

    accLang = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/languages.png"), tr("Select language"), this);
    accLang->setStatusTip(tr("Select language"));

    accHelp = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/help.png"), tr("Help"), this);
    accHelp->setStatusTip(tr("Help"));

    accInfo = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/info.png"), tr("About qABCs"), this);
    accInfo->setStatusTip(tr("About qABCs"));

    accExit = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/exit.png"), tr("Exit"), this);
    accExit->setStatusTip(tr("Exit"));

    toolBar->addAction(accGameAbc);
    toolBar->addAction(accGameRand);
    toolBar->addAction(accGameFood);
    toolBar->addAction(accGameAnimals);
    toolBar->addAction(accGameInstrument);
    toolBar->addAction(accGameToys);
    toolBar->addSeparator();
    toolBar->addAction(accSound);
    toolBar->addAction(accTyping);
    toolBar->addAction(accLang);
    toolBar->addAction(accHelp);
    toolBar->addAction(accInfo);
    toolBar->addSeparator();
    toolBar->addAction(accExit);


    connect(accGameAbc,SIGNAL(changed()),this,SLOT(clickButtonGameAbc()));
    connect(accGameRand,SIGNAL(changed()),this,SLOT(clickButtonGameRand()));
    connect(accGameFood,SIGNAL(changed()),this,SLOT(clickButtonGameFood()));
    connect(accGameAnimals,SIGNAL(changed()),this,SLOT(clickButtonGameAnimals()));
    connect(accGameInstrument,SIGNAL(changed()),this,SLOT(clickButtonGameInstrument()));
    connect(accGameToys,SIGNAL(changed()),this,SLOT(clickButtonGameToys()));
    connect(accSound,SIGNAL(triggered()),this,SLOT(clickButtonSound()));
    connect(accTyping,SIGNAL(triggered()),this,SLOT(clickButtonTyping()));
    connect(accLang,SIGNAL(triggered()),this,SLOT(clickButtonLang()));
    connect(accHelp,SIGNAL(triggered()),this,SLOT(clickButtonHelp()));
    connect(accInfo,SIGNAL(triggered()),this,SLOT(clickButtonInfo()));
    connect(accExit,SIGNAL(triggered()),this,SLOT(close()));

}

void MainWindow::initLanguageAbc(){
    // Clear array
    listLetters.clear();
    for (QString type:listTypes) listCollections[type]->clear();

    _speak_method = "";
    _espeak_params = "";

    // default status of typing
    blockButtonTyping=false;
    _disable_additional_keys=true;
    accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_on.png"));

    if (confSettings->value("global/typing","true").toString()=="false"){
        accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_off.png"));
        _disable_additional_keys=false;
    }



    // reinit path to resource
    for (QString type:listTypes) listCollections[type]->setAbcLanguage(currentLanguageAbc);

    loadAbcConfig(globalPathUserResources+"/"+currentLanguageAbc+"/"+currentFilenameAbc);
}

void MainWindow::refreshTranslate(){
    QDir dirConfig(QDir::homePath()+"/.qabcs/");
    if (dirConfig.exists()==false) dirConfig.mkpath(QDir::homePath()+"/.qabcs/");

    QSettings confSettings(dirConfig.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    confSettings.setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());

    QString locale = confSettings.value("abc/lang",QLocale::system().bcp47Name()).toString();

    qApp->removeTranslator(&translator);
    qApp->removeTranslator(&qtTranslator);

    // save original
    if (listWidgetsRetranslateUi.size()==0){
        QList<QWidget*> l2 = this->findChildren<QWidget *>();
        for (auto &w:l2){
            QMap<QString,QString> m;
            m["toolTip"]=w->toolTip();
            m["whatsThis"]=w->whatsThis();
            m["windowTitle"]=w->windowTitle();
            m["statusTip"]=w->statusTip();
            listWidgetsRetranslateUi[w]=m;
        }

        QList<QAction*> l = this->findChildren<QAction *>();
        for (auto &w:l){
            QMap<QString,QString> m;
            m["toolTip"]=w->toolTip();
            m["whatsThis"]=w->whatsThis();
            m["statusTip"]=w->statusTip();
            listActionsRetranslateUi[w]=m;
        }

    }

    // set translator for app
    translator.load(QString(GLOBAL_PATH_USERDATA)+QString("/langs/qabcs_") + locale);
    qApp->installTranslator(&translator);

    // set translator for default widget's text (for example: QMessageBox's buttons)
#ifdef __WIN32
    qtTranslator.load("qt_"+locale,QString(GLOBAL_PATH_USERDATA)+"/translations");
#else
    qtTranslator.load("qt_"+locale,QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    qApp->installTranslator(&qtTranslator);


    // retranslate UI
    QList<QWidget*> l2 = this->findChildren<QWidget *>();
    for (auto &w:l2){
        if (!w->toolTip().isEmpty()) w->setToolTip(tr(listWidgetsRetranslateUi[w]["toolTip"].toStdString().c_str()));
        if (!w->whatsThis().isEmpty()) w->setWhatsThis(tr(listWidgetsRetranslateUi[w]["whatsThis"].toStdString().c_str()));
        if (!w->windowTitle().isEmpty()) w->setWindowTitle(tr(listWidgetsRetranslateUi[w]["windowTitle"].toStdString().c_str()));
        if (!w->statusTip().isEmpty()) w->setStatusTip(tr(listWidgetsRetranslateUi[w]["statusTip"].toStdString().c_str()));
    }

    QList<QAction*> l = this->findChildren<QAction *>();
    for (auto &w:l){
        if (!w->toolTip().isEmpty()) w->setToolTip(tr(listActionsRetranslateUi[w]["toolTip"].toStdString().c_str()));
        if (!w->whatsThis().isEmpty()) w->setWhatsThis(tr(listActionsRetranslateUi[w]["whatsThis"].toStdString().c_str()));
        if (!w->statusTip().isEmpty()) w->setStatusTip(tr(listActionsRetranslateUi[w]["statusTip"].toStdString().c_str()));
    }
}


void MainWindow::loadAbcConfig(QString abcFilename){
    if (abcFilename.indexOf(QRegExp("(.*)(.json)$"))!=-1){
        if (QFile::exists(abcFilename)){
            loadAbcConfigJson(abcFilename);
            _lastFormatLoaded="json";
        }
    }

    if (abcFilename.indexOf(QRegExp("(.*)(.properties)$"))!=-1){
        if (QFile::exists(abcFilename)){
            loadAbcConfigProperties(abcFilename);
            _lastFormatLoaded="properties";
        }
    }
}

bool MainWindow::loadAbcConfigJson(QString filename){
    QByteArray val;
    QFile file;
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this,"qABCs",tr("Error while opening")+" "+filename+"\n"+file.errorString());
        return false;
    }
    val = file.readAll();
    file.close();

    QStringList folders = filename.split(QRegExp("[/\\\\]"));
    QString folder_lang = folders.at(folders.size()-2);

    QJsonDocument document = QJsonDocument::fromJson(val);
    if (document.isEmpty()){
        QMessageBox::critical(this,"qABCs",filename+" "+tr("is not valid"));
        return false;
    }
    QJsonObject root = document.object();
    QJsonObject root_general = root.value("general").toObject();

    QString inheritsFrom = root_general.value("inheritsFrom").toString();
    if (!inheritsFrom.isEmpty()){
        loadAbcConfig(globalPathUserResources+"/"+inheritsFrom);
    }

    if (root_general.value("speak_method").isString()){
        _speak_method = root_general.value("speak_method").toString();
    }
    if (root_general.value("espeak_params").isString()){
        _espeak_params = root_general.value("espeak_params").toString();
    }
    if (root_general.value("typing").isString()){
        if ( root_general.value("typing").toString()=="false"){
            blockButtonTyping=true;
            _disable_additional_keys= false;
            accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_off.png"));
        }else{
            _disable_additional_keys= true;
            accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_on.png"));
            blockButtonTyping= false;
        }
    }

    for (QString type:listTypes){
        listCollections[type]->setGlobalParam(root_general);
        listCollections[type]->setLastFileName(filename);
    }

    QJsonArray arrLetters = root.value("letters").toArray();
    for (int i=0;i<arrLetters.size();i++){
        QString letter = arrLetters.at(i).toObject().keys().at(0);
        QJsonObject objLetter = arrLetters.at(i).toObject().value(letter).toObject();


        updateletterToList(folder_lang,{letter.toUpper(),
                             objLetter.value("sound_letter").toString(),
                             objLetter.value("speak_method").toString(),
                             objLetter.value("espeak_params").toString(),
                             objLetter.value("espeak_words").toString()
                           });

        for (QString type:listTypes){
            if (objLetter.value(type).isObject()){
                listCollections[type]->setLetter(letter.toUpper(),folder_lang,objLetter.value(type).toObject());
            }
        }

    }

    return true;
}


bool MainWindow::loadAbcConfigProperties(QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this,"qABCs",tr("Error while opening")+" "+filename+"\n"+file.errorString());
        return false;
    }

    QStringList folders = filename.split(QRegExp("[/\\\\]"));
    QString folder_lang = folders.at(folders.size()-2);

    _speak_method="properties";

    while (!file.atEnd()) {
        QString line = file.readLine();
        line.replace(QRegExp("(\\s*)$"),"");

        // ignoring lines
        if (line.indexOf(QRegExp("^([#|;])(.*)"))!=-1) continue;


        QStringList pair = line.split(":");

        // format
        // type:letter=str_rus=espeak_words=metka=noises

        //skip special words
        if (pair.at(0)=="language" or pair.at(0)=="author") continue;

        if (pair.at(0)=="espeak_params"){
            if (pair.size()==2){
                _espeak_params=pair.at(1);
                for (QString type:listTypes) listCollections[type]->setGlobalParam(_speak_method,_espeak_params);
            }
            continue;
        }
        if (pair.at(0)=="speak_method"){
            if (pair.size()==2){
                _speak_method=pair.at(1);
                if (_speak_method=="file" or _speak_method=="") _speak_method="properties";
                if (_speak_method=="espeak") _speak_method="properties_espeak";
                for (QString type:listTypes) listCollections[type]->setGlobalParam(_speak_method,_espeak_params);
            }
            continue;
        }

        if (pair.at(0)=="typing"){
            if (pair.size()==2){
                if (pair.at(1)=="false"){
                    blockButtonTyping=true;
                    _disable_additional_keys= false;
                    accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_off.png"));
                }else{
                    _disable_additional_keys= true;
                    accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_on.png"));
                    blockButtonTyping= false;
                }
            }
        }


        if (pair.at(0)=="inheritsFrom"){
            loadAbcConfig(globalPathUserResources+"/"+pair.at(1));
            continue;
        }

        for (QString type:listTypes) listCollections[type]->setLastFileName(filename);

        if (pair.size()==2){
            QStringList params = pair.at(1).split("=");
            if (params.size()<4) continue;
            QString type = pair.at(0);

            QString letter = params.at(0);
            QString str = params.at(1).toUpper();
            QString espeak_words = params.at(2);
            QString metka = params.at(3);
            QString espeak_params = "";
            QString noises = "";

            QString letter_sound = letter.toLower();

            if (params.size()>=5) noises=params.at(4);

            updateletterToList(folder_lang,{letter.toUpper(),letter_sound,"","",letter});

            listCollections[type]->setLetter(letter.toUpper(),folder_lang,str,metka,str.toLower(),"",espeak_params,espeak_words,noises);
        }else{
            qDebug() << tr("Error str:")+" "+line;
        }
    }
    file.close();

    return true;
}


QString MainWindow::typeGameToString(TYPE_GAME type){
    if (type==TYPE_ABC) return "misc";
    if (type==TYPE_RAND) return "rand";
    if (type==TYPE_FOOD) return "food";
    if (type==TYPE_ANIMALS) return "animals";
    if (type==TYPE_INSTRUMENT) return "instrument";
    if (type==TYPE_TOYS) return "toys";

    return "misc";
}

void MainWindow::setAbcLang(QString lang,QString filename){
    currentLanguageAbc=lang;
    currentFilenameAbc=filename;
    globalPathUserResources=QString(GLOBAL_PATH_USERDATA)+"/abcs";
    currentIndexLetter=0;

    initLanguageAbc();

    QString type_game = confSettings->value("global/gametype","misc").toString();
    if (type_game=="rand"){
        accGameRand->setChecked(true);
        clickButtonGameRand();
    } else if (type_game=="food" and listCollections["food"]->countLetters()>0){
        accGameFood->setChecked(true);
        clickButtonGameFood();
    } else if (type_game=="animals" and listCollections["animals"]->countLetters()>0){
        accGameAnimals->setChecked(true);
        clickButtonGameAnimals();
    } else if (type_game=="instrument" and listCollections["instrument"]->countLetters()>0){
        accGameInstrument->setChecked(true);
        clickButtonGameInstrument();
    } else if (type_game=="toys" and listCollections["toys"]->countLetters()>0){
        accGameToys->setChecked(true);
        clickButtonGameToys();
    }else{
        accGameAbc->setChecked(true);
        clickButtonGameAbc();
    }


    accGameFood->setEnabled(true);
    accGameAnimals->setEnabled(true);
    accGameInstrument->setEnabled(true);
    accGameToys->setEnabled(true);

    //
    if (listCollections["food"]->countLetters()==0){
        accGameFood->setEnabled(false);
    }
    if (listCollections["animals"]->countLetters()==0){
        accGameAnimals->setEnabled(false);
    }
    if (listCollections["instrument"]->countLetters()==0){
        accGameInstrument->setEnabled(false);
    }
    if (listCollections["toys"]->countLetters()==0){
        accGameToys->setEnabled(false);
    }

}

void MainWindow::refreshViewer(){
    if (currentIndexLetter<0) return;

    QString currentLetter = listLetters.at(currentIndexLetter).letter;
    QString text = listCollections[typeGameToString(typeGame)]->getName(currentLetter);

    text.replace("_"," ");
    text.replace(currentLetter,"<font color=\"red\">"+currentLetter+"</font>");

    setLetterAndText(currentLetter.toUpper()+currentLetter.toLower(),text);
    setPixmapViewer(listCollections[typeGameToString(typeGame)]->getPixmap(currentLetter));
}

void MainWindow::setLetterAndText(QString letter,QString text){
    QFont fontSizeLetter;
    fontSizeLetter.setPointSize(45);

    if (letter.size()>15) fontSizeLetter.setPointSize(40);

    lblAbcLetter->setFont(fontSizeLetter);
    lblAbcLetter->setText(letter);

    lblAbcText->setText(text);
}

void MainWindow::setPixmapViewer(QPixmap pixmap){
    lblAbcPicture->setPixmap(pixmap);

    int startY = 70;

    int maxWidth = this->width()-20;
    int maxHeight = 290;

    lblAbcPicture->setFixedSize(pixmap.width(),pixmap.height());


    if (lblAbcPicture->height()>maxHeight){
        double deltaH = (double)lblAbcPicture->height()/(double)maxHeight;
        lblAbcPicture->setFixedWidth(lblAbcPicture->width()/deltaH);
        lblAbcPicture->setFixedHeight(maxHeight);
    }

    if (lblAbcPicture->width()>maxWidth){
        double deltaW = (double)lblAbcPicture->width()/(double)maxWidth;
        lblAbcPicture->setFixedWidth(maxWidth);
        lblAbcPicture->setFixedHeight(lblAbcPicture->height()/deltaW);
    }



    // move to center
    lblAbcPicture->move((this->width()-lblAbcPicture->width())/2,startY+(maxHeight-lblAbcPicture->height())/2);
}

void MainWindow::updateletterToList(QString folder_lang,LETTER_INFO letter){
    int indexLetter=-1;
    for (int inx=0;inx<listLetters.size();inx++) {
        if (listLetters.at(inx).letter.toUpper()==letter.letter.toUpper()){
            indexLetter=inx;
            break;
        }
    }


    // find sound file for letter
    if (!letter.sound_letter.isEmpty()){
        QString folderAlpha = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+folder_lang+"/sounds/alpha";
        QString letterSoundLetterFilename =  SoundEngine::findSoundfile(folderAlpha,letter.sound_letter.toLower());

        if (!letterSoundLetterFilename.isEmpty() and QFile::exists(letterSoundLetterFilename)){
            letter.sound_letter=letterSoundLetterFilename;
        }else{
            if (indexLetter!=-1) letter.sound_letter=listLetters[indexLetter].sound_letter;
        }
    }else{
        if (indexLetter!=-1) letter.sound_letter=listLetters[indexLetter].sound_letter;
    }


    if (indexLetter==-1){
        listLetters.push_back(letter);
    }else{
        listLetters[indexLetter]=letter;
    }
}

void MainWindow::playSoundLetter(QString letter,bool async){
    if (!soundStatus) return;

    for (LETTER_INFO l:listLetters){
        if (l.letter==letter){
            QString speak_method = (l.speak_method.isEmpty()) ? _speak_method : l.speak_method;
            QString espeak_params = (l.espeak_params.isEmpty()) ? _espeak_params : l.espeak_params;

            if (_lastFormatLoaded=="properties"){
                if (speak_method!="properties_espeak"){
                    QString letterSoundLetterFilename =  SoundEngine::findSoundFile(QString(GLOBAL_PATH_USERDATA)+"/abcs/"+currentLanguageAbc+"/"+currentFilenameAbc,letter,"letter");
                    if (!letterSoundLetterFilename.isEmpty()){
                        SoundEngine::playSoundFromFile(letterSoundLetterFilename,async);
                        return;
                    }
                }
                if (!l.espeak_words.isEmpty()){
                    SoundEngine::playSoundFromSpeechSynthesizer(global_path_to_espeak+" "+espeak_params+" \""+l.espeak_words+"\"",async);
                }

            }else{
                if (speak_method=="espeak"){
                    if (!l.espeak_words.isEmpty()){
                        SoundEngine::playSoundFromSpeechSynthesizer(global_path_to_espeak+" "+espeak_params+" \""+l.espeak_words+"\"",async);
                    }
                }else{
                    QString filename = l.sound_letter;
                    if (!filename.isEmpty() and QFile::exists(filename)){
                        SoundEngine::playSoundFromFile(filename,async);
                    }else{
                        SoundEngine::playSoundFromSpeechSynthesizer(global_path_to_espeak+" "+espeak_params+" \""+letter+"\"",async);
                    }
                }
            }


            return;
        }
    }
}

int MainWindow::gameRandomGenerateNextIndex(){
    if (listLettersGameRand.size()==0) return -1;
    if (gameRandomCurrentIndex>=listLettersGameRand.size()) return -1;

    int indexLetter=-1;

    // find index letter
    QString letter = listLettersGameRand.at(gameRandomCurrentIndex).letter;
    for (int i=0;i<listLetters.size();i++){
        if (listLetters.at(i).letter==letter){
            indexLetter=i;
            break;
        }
    }
    if (indexLetter==-1) return -1;

    listCollections["rand"]->clearLetter(letter);
    listCollections["rand"]->setLetter(letter,currentLanguageAbc,listLettersGameRand.at(gameRandomCurrentIndex));

    return indexLetter;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key=event->key();

    soundEffect.stop();

    if (key==Qt::Key_Tab){
        switch (typeGame) {
        case TYPE_ABC:
            accGameRand->setChecked(true);
            break;
        case TYPE_RAND:
            accGameFood->setChecked(true);
            break;
        case TYPE_FOOD:
            accGameAnimals->setChecked(true);
            break;
        case TYPE_ANIMALS:
            accGameInstrument->setChecked(true);
            break;
        case TYPE_INSTRUMENT:
            accGameToys->setChecked(true);
            break;
        default:
            accGameAbc->setChecked(true);
            break;
        }

        return;
    }

    if (listLetters.size()==0) return;

    if (currentIndexLetter>=0 and key==Qt::Key_Space){
        if ((typeGame==TYPE_ABC or typeGame==TYPE_RAND) and gameAbcFinish==true) return;

        if (soundStatus){
            listCollections[typeGameToString(typeGame)]->playSoundPicture(listLetters.at(currentIndexLetter).letter);
        }
        return;
    }

    if (typeGame==TYPE_ABC){
        // start over the game
        if (gameAbcFinish and (key==Qt::Key_Enter or key==Qt::Key_Return)){
            clickButtonGameAbc();
            return;
        }

        // return back
        if (currentIndexLetter>0 and key==Qt::Key_Backspace){
            gameAbcFinish=false;
            currentIndexLetter--;
        }

        // previous picture by Key_Left
        if (currentIndexLetter>0 and (!_disable_additional_keys and key==Qt::Key_Left)){
            if (!gameAbcFinish) playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
            gameAbcFinish=false;
            currentIndexLetter--;
        }
        else if (currentIndexLetter==0 and (!_disable_additional_keys and key==Qt::Key_Left)){
            if (!gameAbcFinish) playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
        }

        // next picture
        if (currentIndexLetter<listLetters.size()){
            if ((_disable_additional_keys and listLetters.at(currentIndexLetter).letter==QString(QChar(key))) or (!_disable_additional_keys and key==Qt::Key_Right)){
                playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
                currentIndexLetter++;
            }
        }

        // finish
        if (currentIndexLetter>=listLetters.size()){
            setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/ribbon.png"));
            lblAbcLetter->setText(tr("CONGRATS!"));
            lblAbcText->setText(tr("Press \"ENTER\" to Play Again"));

            if (soundStatus){
                QString filename =  SoundEngine::findSoundfile(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/sounds","cheering");
                if (!filename.isEmpty()){
                    QFileInfo soundFile(filename);
                    soundEffect.setMedia(QUrl::fromLocalFile(soundFile.absoluteFilePath()));
                    soundEffect.play();
                }
            }

            gameAbcFinish=true;
            return;
        }
    } else if (typeGame==TYPE_RAND){
        // start over the game
        if (gameAbcFinish and (key==Qt::Key_Enter or key==Qt::Key_Return)){
            clickButtonGameRand();
            return;
        }
        // return back
        if (gameRandomCurrentIndex>0 and key==Qt::Key_Backspace){
            gameAbcFinish=false;
            gameRandomCurrentIndex--;
            currentIndexLetter = gameRandomGenerateNextIndex();
        }
        // previous picture by Key_Left
        if (gameRandomCurrentIndex>0 and (!_disable_additional_keys and key==Qt::Key_Left)){
            if (!gameAbcFinish) playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
            gameAbcFinish=false;
            gameRandomCurrentIndex--;
            currentIndexLetter = gameRandomGenerateNextIndex();
        }
        else if (gameRandomCurrentIndex==0 and (!_disable_additional_keys and key==Qt::Key_Left)){
            if (!gameAbcFinish) playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
        }
        // next picture
        if (gameRandomCurrentIndex<listLettersGameRand.size()){
            if ((_disable_additional_keys and listLetters.at(currentIndexLetter).letter==QString(QChar(key))) or (!_disable_additional_keys and key==Qt::Key_Right)){
                playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
                gameRandomCurrentIndex++;
                currentIndexLetter = gameRandomGenerateNextIndex();
            }
        }
        // finish
        if (gameRandomCurrentIndex>=listLettersGameRand.size()){
            setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/ribbon.png"));
            lblAbcLetter->setText(tr("CONGRATS!"));
            lblAbcText->setText(tr("Press \"ENTER\" to Play Again"));

            if (soundStatus){
                QString filename =  SoundEngine::findSoundfile(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/sounds","cheering");
                if (!filename.isEmpty()){
                    QFileInfo soundFile(filename);
                    soundEffect.setMedia(QUrl::fromLocalFile(soundFile.absoluteFilePath()));
                    soundEffect.play();
                }
            }

            gameAbcFinish=true;
            return;
        }

    } else {
        if (!_disable_additional_keys and key==Qt::Key_Left){
            if (currentIndexLetter>0){
                currentIndexLetter--;
            }else{
                currentIndexLetter=listLetters.size()-1;
            }
            playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
        } else if (!_disable_additional_keys and key==Qt::Key_Right){
            if (currentIndexLetter<listLetters.size()-1 and currentIndexLetter!=-1){
                currentIndexLetter++;
            }else{
                currentIndexLetter=0;
            }
            playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
        }else{
            if (_disable_additional_keys){
                for (int i=0;i<listLetters.size();i++){
                    if (listLetters.at(i).letter==QString(QChar(key))){
                        currentIndexLetter=i;

                        playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
                        break;
                    }
                }
            }
        }


    }

    refreshViewer();
}

void MainWindow::clickButtonGameAbc(){
    if (!accGameAbc->isChecked()) return;
    this->setWindowTitle(tr("Find the letter on the keyboard (alphabetically)"));

    typeGame=TYPE_ABC;
    currentIndexLetter=0;

    if (listLetters.size()>0){
        refreshViewer();
    }else{
        setPixmapViewer(QPixmap());
        lblAbcText->setText(tr("ABC is not loaded"));
        lblAbcLetter->setText("");
    }

    gameAbcFinish=false;

    confSettings->setValue("global/gametype",typeGameToString(typeGame));
}

void MainWindow::clickButtonGameRand(){
    if (!accGameRand->isChecked()) return;
    this->setWindowTitle(tr("Find the letter on the keyboard (randomly)"));

    typeGame=TYPE_RAND;
    currentIndexLetter=0;
    gameRandomCurrentIndex=0;

    QVector<LETTER_CONFIG> tmplistLetterConfig;
    QMap<QString,QString> useLetters;

    listCollections["rand"]->clear();
    listLettersGameRand.clear();


    listCollections["rand"]->setGlobalParam(_speak_method,_espeak_params);

    // generate list
    for (QString type:listTypes) {
        if (type=="rand") continue;

        for (LETTER_INFO l:listLetters){
            LETTER_CONFIG letterConfig = listCollections[type]->getLetterConfig(l.letter);

            if (useLetters.contains(l.letter+"-"+letterConfig.name)!=0) continue;
            tmplistLetterConfig.push_back(letterConfig);
            useLetters[l.letter+"-"+letterConfig.name]="1";
        }
    }

    // sort
    while (tmplistLetterConfig.size()>0){
        int nIndex = qrand()%tmplistLetterConfig.size();
        listLettersGameRand.push_back(tmplistLetterConfig.at(nIndex));
        tmplistLetterConfig.remove(nIndex);
    }


    currentIndexLetter = gameRandomGenerateNextIndex();

    if (listLetters.size()>0){
        refreshViewer();
    }else{
        setPixmapViewer(QPixmap());
        lblAbcText->setText(tr("ABC is not loaded"));
        lblAbcLetter->setText("");
    }

    gameAbcFinish=false;
    confSettings->setValue("global/gametype",typeGameToString(typeGame));
}

void MainWindow::clickButtonGameAnimals(){
    if (!accGameAnimals->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See an Animal"));

    typeGame=TYPE_ANIMALS;
    currentIndexLetter=-1;
    gameAbcFinish=true;
    confSettings->setValue("global/gametype",typeGameToString(typeGame));

    if (listLetters.size()==0){
        setPixmapViewer(QPixmap());
        lblAbcText->setText(tr("ABC is not loaded"));
        lblAbcLetter->setText("");
        return;
    }

    setLetterAndText(tr("Animals"),"");
    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/turtle.png"));
}

void MainWindow::clickButtonGameFood(){
    if (!accGameFood->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See a Food"));

    typeGame=TYPE_FOOD;
    currentIndexLetter=-1;
    gameAbcFinish=true;
    confSettings->setValue("global/gametype",typeGameToString(typeGame));

    if (listLetters.size()==0){
        setPixmapViewer(QPixmap());
        lblAbcText->setText(tr("ABC is not loaded"));
        lblAbcLetter->setText("");
        return;
    }


    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/hot_dog.png"));
    setLetterAndText(tr("Food"),"");
}

void MainWindow::clickButtonGameInstrument(){
    if (!accGameInstrument->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See an Instrument"));

    typeGame=TYPE_INSTRUMENT;
    currentIndexLetter=-1;
    gameAbcFinish=true;

    confSettings->setValue("global/gametype",typeGameToString(typeGame));
    if (listLetters.size()==0){
        setPixmapViewer(QPixmap());
        lblAbcText->setText(tr("ABC is not loaded"));
        lblAbcLetter->setText("");
        return;
    }

    setLetterAndText(tr("Music"),"");
    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/guitar.png"));
}

void MainWindow::clickButtonGameToys(){
    if (!accGameToys->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See a Toy"));

    typeGame=TYPE_TOYS;
    currentIndexLetter=-1;
    gameAbcFinish=true;
    confSettings->setValue("global/gametype",typeGameToString(typeGame));

    if (listLetters.size()==0){
        setPixmapViewer(QPixmap());
        lblAbcText->setText(tr("ABC is not loaded"));
        lblAbcLetter->setText("");
        return;
    }

    setLetterAndText(tr("Toys"),"");
    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/wagon.png"));
}

void MainWindow::clickButtonSound(){
    if (soundStatus){
        accSound->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/sound_off.png"));
        soundStatus=false;
        confSettings->setValue("global/sound","false");
    }else{
        accSound->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/sound_on.png"));
        soundStatus=true;
        confSettings->setValue("global/sound","true");
    }
}

void MainWindow::clickButtonTyping(){
    if (blockButtonTyping) return;

    if (_disable_additional_keys){
        accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_off.png"));
        _disable_additional_keys=false;
        confSettings->setValue("global/typing","false");
    }else{
        accTyping->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/typing_on.png"));
        _disable_additional_keys=true;
        confSettings->setValue("global/typing","true");
    }
}

void MainWindow::clickButtonLang(){
    FormSelectLanguage form(this);
    if (form.exec()){
        refreshTranslate();

        setAbcLang(confSettings->value("abc/language","en").toString(),confSettings->value("abc/filename","abc.json").toString());
    }
}

void MainWindow::clickButtonHelp(){
    FormHelp form(this);
    form.exec();
}

void MainWindow::clickButtonInfo(){
    FormAbout form(this);
    form.exec();
}


bool MainWindow::isExistSox(){
    QProcess proc;
    global_path_to_play="play";

#if defined(_WIN32)
    return true;
    //global_path_to_play = "\""+QCoreApplication::applicationDirPath() + "/3rdparty/sox/play.exe\"";
#endif

    proc.start(global_path_to_play+" --version");
    proc.waitForFinished();
    QString text = proc.readAll();

    if (!text.isEmpty()){
        qDebug() << tr("play was found:")+" "+global_path_to_play;
        return true;
    }

    return false;
}

bool MainWindow::isExistEspeak(){
    QProcess proc;
    global_path_to_espeak="espeak";

    QStringList pathForSearch;
    pathForSearch.push_back("espeak");

#if defined(_WIN32)
    pathForSearch.push_back("\""+QDir::rootPath()+"Program Files (x86)/eSpeak/command_line/espeak.exe\"");
    pathForSearch.push_back("\""+QDir::rootPath()+"Program Files/eSpeak/command_line/espeak.exe\"");
    pathForSearch.push_back("\""+QCoreApplication::applicationDirPath() + "/3rdparty/eSpeak/espeak.exe\"");
#endif

    for (QString pathEspeak:pathForSearch){
        proc.start(pathEspeak+" --version");
        proc.waitForFinished();
        QString text = proc.readAll();
        if (!text.isEmpty()){
            global_path_to_espeak=pathEspeak;
            qDebug() << tr("espeak was found:")+" "+global_path_to_espeak;
            return true;
        }
    }

    return false;
}
