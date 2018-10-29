#include "mainwindow.h"

#include "config_qabcs.h"
#include "FormAbout.h"
#include "FormHelp.h"
#include "FormSelectLanguage.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QRegExp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSound>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setFixedSize(552,541);
    this->setWindowIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"));

    currentIndexLetter=0;   
    gameAbcFinish=false;
    _speak_method="";
    _espeak_params="";
    soundStatus=true;       // sound on

    listTypes = QStringList() << "misc" << "food" << "animals" << "instrument" << "toys";

    qDebug() << "use path: " << QString(GLOBAL_PATH_USERDATA);

    // open ini user config
    QDir dirConfig(QDir::homePath()+"/.qabcs/");
    if (dirConfig.exists()==false) dirConfig.mkpath(QDir::homePath()+"/.qabcs/");
    confSettings = new QSettings(dirConfig.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    confSettings->setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());


    // init collections
    for (QString type:listTypes) listCollections[type] = new Collection(confSettings->value("abc/language","en").toString());

    initGUI();
    initToolBar();
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

    lblAbcLetter = new QLabel(this);
    lblAbcLetter->setFont(fontSizeLetter);
    lblAbcLetter->setAlignment(Qt::AlignCenter);
    lblAbcLetter->setGeometry(0,360,this->width(),101);
    //lblAbcLetter->setFrameShape(QFrame::Box);

    lblAbcText = new QLabel(this);
    lblAbcText->setFont(fontSizeText);
    lblAbcText->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    lblAbcText->setGeometry(0, this->height()-statusbar->height()-101, this->width(), 91);
    //lblAbcText->setFrameShape(QFrame::Box);
}


void MainWindow::initToolBar(){
    toolBar = new QToolBar("User");
    toolBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, toolBar);

    toolBar->setIconSize(QSize(40, 40));

    typeGameGroup = new QActionGroup(this);

    accAbc = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"), tr("Find the letter on the keyboard"), this);
    accAbc->setStatusTip(tr("Find the letter on the keyboard"));
    accAbc->setCheckable(true);
    accAbc->setActionGroup(typeGameGroup);

    accFood = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/food.png"), tr("Show foods that begin with each letter"), this);
    accFood->setStatusTip(tr("Show foods that begin with each letter"));
    accFood ->setCheckable(true);
    accFood->setActionGroup(typeGameGroup);

    accAnimals = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/animals.png"), tr("Show animals that begin with each letter"), this);
    accAnimals->setStatusTip(tr("Show animals that begin with each letter"));
    accAnimals->setCheckable(true);
    accAnimals->setActionGroup(typeGameGroup);

    accInstrument = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/instrument.png"), tr("Show musical instruments for each letter"), this);
    accInstrument->setStatusTip(tr("Show musical instruments for each letter"));
    accInstrument->setCheckable(true);
    accInstrument->setActionGroup(typeGameGroup);

    accToys = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/toys.png"), tr("Show toys for each letter"), this);
    accToys->setStatusTip(tr("Show toys for each letter"));
    accToys->setCheckable(true);
    accToys->setActionGroup(typeGameGroup);

    accSound = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/sound_on.png"), tr("Sound on/off"), this);
    accSound->setStatusTip(tr("Sound on/off"));

    accLang = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/languages.png"), tr("Select language"), this);
    accLang->setStatusTip(tr("Select language"));

    accHelp = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/help.png"), tr("Help"), this);
    accHelp->setStatusTip(tr("Help"));

    accInfo = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/info.png"), tr("About qabcs"), this);
    accInfo->setStatusTip(tr("About QABCs"));

    accExit = new QAction(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/exit.png"), tr("Exit"), this);
    accExit->setStatusTip(tr("Exit"));

    toolBar->addAction(accAbc);
    toolBar->addAction(accFood);
    toolBar->addAction(accAnimals);
    toolBar->addAction(accInstrument);
    toolBar->addAction(accToys);
    toolBar->addSeparator();
    toolBar->addAction(accSound);
    toolBar->addAction(accLang);
    toolBar->addAction(accHelp);
    toolBar->addAction(accInfo);
    toolBar->addSeparator();
    toolBar->addAction(accExit);


    connect(accAbc,SIGNAL(changed()),this,SLOT(clickButtonAbc()));
    connect(accFood,SIGNAL(changed()),this,SLOT(clickButtonFood()));
    connect(accAnimals,SIGNAL(changed()),this,SLOT(clickButtonAnimals()));
    connect(accInstrument,SIGNAL(changed()),this,SLOT(clickButtonInstrument()));
    connect(accToys,SIGNAL(changed()),this,SLOT(clickButtonToys()));
    connect(accSound,SIGNAL(triggered()),this,SLOT(clickButtonSound()));
    connect(accLang,SIGNAL(triggered()),this,SLOT(clickButtonLang()));
    connect(accHelp,SIGNAL(triggered()),this,SLOT(clickButtonHelp()));
    connect(accInfo,SIGNAL(triggered()),this,SLOT(clickButtonInfo()));
    connect(accExit,SIGNAL(triggered()),this,SLOT(close()));

}

void MainWindow::initLanguageAbc(){

    // Clear array
    listLetters.clear();
    for (QString type:listTypes) listCollections[type]->clear();

    // reinit path to resource
    for (QString type:listTypes) listCollections[type]->setAbcLanguage(currentLanguageAbc);


    if (currentFilenameAbc.indexOf(QRegExp("(.*)(.json)$"))!=-1){
        QString jsonFilename = globalPathUserResources+"/"+currentLanguageAbc+"/"+currentFilenameAbc;
        if (QFile::exists(jsonFilename)){
            if (loadAbcConfigJson(jsonFilename)) return;
        }
    }

    if (currentFilenameAbc.indexOf(QRegExp("(.*)(.properties)$"))!=-1){
        QString propertiesFilename = globalPathUserResources+"/"+currentLanguageAbc+"/"+currentFilenameAbc;
        if (QFile::exists(propertiesFilename)){
            if (loadAbcConfigProperties(propertiesFilename)) return;
        }
    }
}

bool MainWindow::loadAbcConfigJson(QString filename){
    QByteArray val;
    QFile file;
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this,"QABCS",tr("Error open ")+filename+"\n"+file.errorString());
        return false;
    }
    val = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val);
    if (document.isEmpty()){
        QMessageBox::critical(this,"QABCS",filename+" "+tr("is not valid"));
        return false;
    }
    QJsonObject root = document.object();

    QJsonObject root_general = root.value("general").toObject();
    _speak_method = root_general.value("speak_method").toString();
    _espeak_params = root_general.value("espeak_params").toString();
    for (QString type:listTypes) listCollections[type]->setGlobalParam(root_general);

    QJsonArray arrLetters = root.value("letters").toArray();

    for (int i=0;i<arrLetters.size();i++){
        QString letter = arrLetters.at(i).toObject().keys().at(0);
        QJsonObject objLetter = arrLetters.at(i).toObject().value(letter).toObject();

        listLetters.push_back({letter.toUpper(),
                               objLetter.value("sound_letter").toString(),
                               objLetter.value("speak_method").toString(),
                               objLetter.value("espeak_params").toString(),
                               objLetter.value("espeak_words").toString()
                             });

        for (QString type:listTypes){
            listCollections[type]->setLetter(letter.toUpper(),objLetter.value(type).toObject());
        }

    }

    return true;
}


bool MainWindow::loadAbcConfigProperties(QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this,"QABCS",tr("Error open ")+filename+"\n"+file.errorString());
        return false;
    }

    _speak_method = "";
    _espeak_params = "";


    while (!file.atEnd()) {
        QString line = file.readLine();
        line.replace(QRegExp("(\\s*)$"),"");

        QStringList pair = line.split(":");

        // format
        // type:letter=str_rus=espeak_words=metka=noises

        //skip special words
        if (pair.at(0)=="language" or pair.at(0)=="author") continue;

        if (pair.at(0)=="espeak_params"){
            if (pair.size()==2){
                _espeak_params=pair.at(1);
                for (QString type:listTypes) listCollections[type]->setGlobalParam("",_espeak_params);
            }
            continue;
        }

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

            if (params.size()>=5) noises=params.at(4);

            QString speak_method = (espeak_words.isEmpty()) ? "file":"espeak";

            listLetters.push_back({letter.toUpper(),letter+".wav",speak_method,"",letter});
            listCollections[type]->setLetter(letter.toUpper(),str,metka,metka,speak_method,espeak_params,espeak_words,noises);
        }else{
            qDebug() << tr("error str: ")+line;
        }
    }
    file.close();

    return true;
}


QString MainWindow::typeGameToString(TYPE_GAME type){
    if (type==TYPE_ABC) return "misc";
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

    accAbc->setChecked(true);
    clickButtonAbc();

}

void MainWindow::refreshViewer(){
    if (currentIndexLetter<0) return;

    QString currentLetter = listLetters.at(currentIndexLetter).letter;
    QString text = listCollections[typeGameToString(typeGame)]->getName(currentLetter);

    text.replace(currentLetter,"<font color=\"red\">"+currentLetter+"</font>");

    lblAbcText->setText(text);
    lblAbcLetter->setText(currentLetter);

    setPixmapViewer(listCollections[typeGameToString(typeGame)]->getPixmap(currentLetter));
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

void MainWindow::playSoundLetter(QString letter,bool async){
    if (!soundStatus) return;

    QStringList listExtensionFiles = QStringList() << "wav" << "ogg" << "mp3";

    for (LETTER_INFO l:listLetters){
        if (l.letter==letter){
            QString speak_method = (l.speak_method.isEmpty()) ? _speak_method : l.speak_method;
            QString espeak_params = (l.espeak_params.isEmpty()) ? _espeak_params : l.espeak_params;

            if (speak_method=="espeak"){
                if (!l.espeak_words.isEmpty()){
                    if (async){
                        QProcess::startDetached("espeak "+espeak_params+" \""+l.espeak_words+"\"");
                    }else{
                        QProcess::execute("espeak "+espeak_params+" \""+l.espeak_words+"\"");
                    }
                }

            }else{
                QString filename =  QString(GLOBAL_PATH_USERDATA)+"/abcs/"+currentLanguageAbc+"/sounds/alpha/"+l.sound_letter.toLower();
                if (!QFile::exists(filename)){
                    for (QString ext:listExtensionFiles){
                        filename = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+currentLanguageAbc+"/sounds/alpha/"+l.sound_letter.toLower()+"."+ext;
                        if (QFile::exists(filename)) break;
                    }
                }
                if (QFile::exists(filename)){
                    if (async){
                        QProcess::startDetached("play "+filename);
                    }else{
                        QProcess::execute("play "+filename);
                    }
                }
            }
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key=event->key();

    soundEffect.stop();

    if (key==Qt::Key_Tab){
        switch (typeGame) {
        case TYPE_ABC:
            accFood->setChecked(true);
            break;
        case TYPE_FOOD:
            accAnimals->setChecked(true);
            break;
        case TYPE_ANIMALS:
            accInstrument->setChecked(true);
            break;
        case TYPE_INSTRUMENT:
            accToys->setChecked(true);
            break;
        default:
            accAbc->setChecked(true);
            break;
        }

        return;
    }

    if (listLetters.size()==0) return;


    if (currentIndexLetter>=0 and key==Qt::Key_Space){
        if (typeGame==TYPE_ABC and gameAbcFinish==true) return;

        if (soundStatus){
            listCollections[typeGameToString(typeGame)]->playSoundPicture(listLetters.at(currentIndexLetter).letter);
        }
        return;
    }

    if (typeGame==TYPE_ABC){
        if (gameAbcFinish and (key==Qt::Key_Enter or key==Qt::Key_Return)){
            clickButtonAbc();
            return;
        }

        if (currentIndexLetter>=0 and key==Qt::Key_Backspace){
            gameAbcFinish=false;
            if (currentIndexLetter>0) currentIndexLetter--;
        }

        if (currentIndexLetter<listLetters.size()){
            if (listLetters.at(currentIndexLetter).letter==QString(QChar(key))){
                playSoundLetter(listLetters.at(currentIndexLetter).letter);
                currentIndexLetter++;
            }
        }
        if (currentIndexLetter>=listLetters.size()){
            setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/ribbon.png"));
            lblAbcLetter->setText(tr("CONGRATS!"));
            lblAbcText->setText(tr("Press \"ENTER\" to Play Again"));

            if (soundStatus){
                soundEffect.setSource(QUrl::fromLocalFile(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/sounds/cheering.wav"));
                soundEffect.play();
            }

            gameAbcFinish=true;
            return;
        }
    }else{
        for (int i=0;i<listLetters.size();i++){
            if (listLetters.at(i).letter==QString(QChar(key))){
                currentIndexLetter=i;

                playSoundLetter(listLetters.at(currentIndexLetter).letter,true);
                break;
            }
        }
    }

    refreshViewer();
}

void MainWindow::clickButtonAbc(){
    if (!accAbc->isChecked()) return;
    this->setWindowTitle(tr("Find the letter on the keyboard"));

    typeGame=TYPE_ABC;
    currentIndexLetter=0;

    if (listLetters.size()>0){
        refreshViewer();
    }else{
        setPixmapViewer(QPixmap());
        lblAbcText->setText(tr("abc is not loaded"));
        lblAbcLetter->setText("");
    }

    gameAbcFinish=false;
}

void MainWindow::clickButtonAnimals(){
    if (!accAnimals->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See an Animal"));

    typeGame=TYPE_ANIMALS;
    currentIndexLetter=-1;
    gameAbcFinish=true;

    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/turtle.png"));
    lblAbcText->setText("");
    lblAbcLetter->setText(tr("Animals"));
}

void MainWindow::clickButtonFood(){
    if (!accFood->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See a Food"));

    typeGame=TYPE_FOOD;
    currentIndexLetter=-1;
    gameAbcFinish=true;

    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/hot_dog.png"));
    lblAbcText->setText("");
    lblAbcLetter->setText(tr("Food"));
}

void MainWindow::clickButtonInstrument(){
    if (!accInstrument->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See an Instrument"));

    typeGame=TYPE_INSTRUMENT;
    currentIndexLetter=-1;
    gameAbcFinish=true;

    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/guitar.png"));
    lblAbcText->setText("");
    lblAbcLetter->setText(tr("Music"));

}

void MainWindow::clickButtonToys(){
    if (!accToys->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See a Toy"));

    typeGame=TYPE_TOYS;
    currentIndexLetter=-1;
    gameAbcFinish=true;

    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/wagon.png"));
    lblAbcText->setText("");
    lblAbcLetter->setText(tr("Toys"));
}

void MainWindow::clickButtonSound(){
    if (soundStatus){
        accSound->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/sound_off.png"));
        soundStatus=false;
    }else{
        accSound->setIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/sound_on.png"));
        soundStatus=true;
    }
}

void MainWindow::clickButtonLang(){
    FormSelectLanguage form(this);
    if (form.exec()){
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
