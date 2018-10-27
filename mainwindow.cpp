#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"));

    currentIndexLetter=0;   
    gameAbcFinish=false;
    _speak_method="";
    _espeak_params="";

    listTypes = QStringList() << "misc" << "food" << "animals" << "instrument" << "toys";

    qDebug() << "use path: " << QString(GLOBAL_PATH_USERDATA);

    // open ini user config
    QDir dirConfig(QDir::homePath()+"/.qabcs/");
    if (dirConfig.exists()==false) dirConfig.mkpath(QDir::homePath()+"/.qabcs/");
    confSettings = new QSettings(dirConfig.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    confSettings->setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());


    // init collections
    for (QString type:listTypes) listCollections[type] = new Collection(confSettings->value("abc/language","en").toString());

    initToolBar();
    setAbcLang(confSettings->value("abc/language","en").toString());


}

MainWindow::~MainWindow(){
    delete ui;
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


    QString jsonFilename = globalPathUserResources+"/"+currentLanguageAbc+"/abc.json";
    if (QFile::exists(jsonFilename)){
        if (loadAbcConfigJson(jsonFilename)) return;
    }

    QString propertiesFilename = globalPathUserResources+"/"+currentLanguageAbc+"/abc.properties";
    if (QFile::exists(propertiesFilename)){
        if (loadAbcConfigProperties(propertiesFilename)) return;
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
        line.replace(QRegExp("\\s"),"");

        QStringList pair = line.split(":");

        // format
        // type:letter:str_rus:metka:espeak_words:noises

        //skip special words
        if (pair.at(0)=="language" or pair.at(0)=="author") continue;

        if (pair.size()==2){
            QStringList params = pair.at(1).split("=");
            if (params.size()<4) continue;
            QString type = pair.at(0);

            QString letter = params.at(0);
            QString str = params.at(1).toUpper();
            QString metka = params.at(2);
            QString espeak_words = params.at(3);
            QString noises = "";

            if (params.size()==5) noises=params.at(4);

            QString speak_method = (espeak_words.isEmpty()) ? "file":"espeak";

            listLetters.push_back({letter.toUpper(),letter+".wav",speak_method,"",letter});
            listCollections[type]->setLetter(letter.toUpper(),str,metka,metka,speak_method,"",espeak_words,noises);
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

void MainWindow::setAbcLang(QString lang){
    currentLanguageAbc=lang;
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

    ui->label_3->setText(text);
    ui->label_2->setText(currentLetter);

    setPixmapViewer(listCollections[typeGameToString(typeGame)]->getPixmap(currentLetter));
}

void MainWindow::setPixmapViewer(QPixmap pixmap){
    ui->label_4->setPixmap(pixmap);

    int startY = 20;

    int maxWidth = this->width()-20;
    int maxHeight = 290;

    ui->label_4->setFixedSize(pixmap.width(),pixmap.height());


    if (ui->label_4->height()>maxHeight){
        double deltaH = (double)ui->label_4->height()/(double)maxHeight;
        ui->label_4->setFixedWidth(ui->label_4->width()/deltaH);
        ui->label_4->setFixedHeight(maxHeight);
    }

    if (ui->label_4->width()>maxWidth){
        double deltaW = (double)ui->label_4->width()/(double)maxWidth;
        ui->label_4->setFixedWidth(maxWidth);
        ui->label_4->setFixedHeight(ui->label_4->height()/deltaW);
    }



    // move to center
    ui->label_4->move((this->width()-ui->label_4->width())/2,startY+(maxHeight-ui->label_4->height())/2);
}

void MainWindow::playSoundLetter(QString letter,bool async){
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
                QString filename =  QString(GLOBAL_PATH_USERDATA)+"/abcs/"+currentLanguageAbc+"/sounds/alpha/"+l.sound_letter;
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

        listCollections[typeGameToString(typeGame)]->playSoundPicture(listLetters.at(currentIndexLetter).letter);
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
            ui->label_2->setText(tr("CONGRATS!"));
            ui->label_3->setText(tr("Press \"ENTER\" to Play Again"));

            soundEffect.setSource(QUrl::fromLocalFile(QString(GLOBAL_PATH_USERDATA)+"/abcs/all/sounds/cheering.wav"));
            soundEffect.play();

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
        ui->label_3->setText(tr("abc is not loaded"));
        ui->label_2->setText("");
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
    ui->label_3->setText("");
    ui->label_2->setText(tr("Animals"));
}

void MainWindow::clickButtonFood(){
    if (!accFood->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See a Food"));

    typeGame=TYPE_FOOD;
    currentIndexLetter=-1;
    gameAbcFinish=true;

    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/hot_dog.png"));
    ui->label_3->setText("");
    ui->label_2->setText(tr("Food"));
}

void MainWindow::clickButtonInstrument(){
    if (!accInstrument->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See an Instrument"));

    typeGame=TYPE_INSTRUMENT;
    currentIndexLetter=-1;
    gameAbcFinish=true;

    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/guitar.png"));
    ui->label_3->setText("");
    ui->label_2->setText(tr("Music"));

}

void MainWindow::clickButtonToys(){
    if (!accToys->isChecked()) return;
    this->setWindowTitle(tr("Press a Key to See a Toy"));

    typeGame=TYPE_TOYS;
    currentIndexLetter=-1;
    gameAbcFinish=true;

    setPixmapViewer(QPixmap(QString(GLOBAL_PATH_USERDATA)+"/images/backgrounds/wagon.png"));
    ui->label_3->setText("");
    ui->label_2->setText(tr("Toys"));
}

void MainWindow::clickButtonLang(){
    FormSelectLanguage form(this);
    if (form.exec()){
        setAbcLang(confSettings->value("abc/language","en").toString());
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
