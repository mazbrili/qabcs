#include "AlphabetTable.h"
#include "ui_AlphabetTable.h"

#include <QDir>
#include <QIcon>
#include <QtDebug>

#include "config_qabcs.h"
#include "SoundEngine.h"

AlphabetTable::AlphabetTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlphabetTable)
{
    ui->setupUi(this);    
    this->setWindowTitle(tr("Alphabet table"));
    this->setWindowIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"));

    isPlayLetter=false;

    QDir dirConfig(QDir::homePath()+"/.qabcs/");
    confSettings = new QSettings(dirConfig.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    confSettings->setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());

    QString pathAbc = QString(GLOBAL_PATH_USERDATA)+"/abcs/"+confSettings->value("abc/language","en").toString()+"/"+confSettings->value("abc/filename","abc1.json").toString();

    config_current = LoaderAbcFormats::LoadFilename(pathAbc,true);

    int x=10,y=10;
    int sizeFormWidth=500;

    QFont font;
    font.setBold(true);
    font.setPixelSize(30);

    for (auto &obj:config_current.letters){
        QLabel *cube = new QLabel(this);
        cube->setText(LoaderAbcFormats::upperString(obj.letter)+obj.letter.toLower());
        cube->setFont(font);
        cube->setAutoFillBackground(true);
        cube->setAlignment(Qt::AlignCenter);
        cube->setGeometry(x,y,100,80);
        cube->setFrameShape(QFrame::Box);
        cube->setProperty("letter",obj.letter);
        cube->installEventFilter(this);

        setPalette(cube,false);

        listLabelLetters.push_back(cube);

        x+=110;
        if (x>600){
            sizeFormWidth=x;
            x=10;
            y+=90;
        }
    }

    this->setFixedSize(sizeFormWidth,y+90);

    readLetterVariants();
}

AlphabetTable::~AlphabetTable(){
    delete confSettings;
    delete ui;
}


bool AlphabetTable::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::MouseButtonRelease and !isPlayLetter){
        auto it = std::find_if(listLabelLetters.begin(),listLabelLetters.end(),[&obj](QLabel *label){
            return obj==label;
        });
        if (it!=listLabelLetters.end()){
            QLabel *label = dynamic_cast<QLabel*>(*it);
            if (label==nullptr) return QDialog::eventFilter(obj, event);

            playLetter(label);
        }
    }

    return QDialog::eventFilter(obj, event);
}

void AlphabetTable::keyPressEvent(QKeyEvent *event){
    if (isPlayLetter) return;
    int key=event->key();

    auto it = std::find_if(listLabelLetters.begin(),listLabelLetters.end(),[&](QLabel *label){
        return isKeypressLetter(label->property("letter").toString(),QString(QChar(key)));
    });
    if (it!=listLabelLetters.end()){
        QLabel *label = dynamic_cast<QLabel*>(*it);
        if (label==nullptr) return;
        playLetter(label);
    }

}

void AlphabetTable::playLetter(QLabel *label){
    QString letter = label->property("letter").toString();

    auto it2 = std::find_if(config_current.letters.begin(),config_current.letters.end(),[&letter](ABC_CONFIG_ALPHA configAlpha){
        return configAlpha.letter==letter;
    });
    if (it2==config_current.letters.end()) return;

    ABC_CONFIG_ALPHA configAlpha = static_cast<ABC_CONFIG_ALPHA>(*it2);

    setPalette(label,true);

    QString currentLanguageAbc=confSettings->value("abc/language","en").toString();
    QString currentFilenameAbc=confSettings->value("abc/filename","abc1.json").toString();


    QString speak_method = (configAlpha.speak_method.isEmpty()) ? config_current.speak_method : configAlpha.speak_method;
    QString espeak_params = (configAlpha.espeak_params.isEmpty()) ? config_current.espeak_params : configAlpha.espeak_params;

    if (config_current.format=="properties"){
        if (speak_method!="properties_espeak"){
            QString letterSoundLetterFilename =  SoundEngine::findSoundFile(QString(GLOBAL_PATH_USERDATA)+"/abcs/"+currentLanguageAbc+"/"+currentFilenameAbc,letter,"letter");
            if (!letterSoundLetterFilename.isEmpty()){
                SoundEngine::playSoundFromFile(letterSoundLetterFilename,false);
                setPalette(label,false);
                return;
            }
        }
        if (!configAlpha.espeak_words.isEmpty()){
            SoundEngine::playSoundFromSpeechSynthesizer(global_path_to_espeak+" "+espeak_params+" \""+configAlpha.espeak_words+"\"",false);
        }

    }else{
        if (speak_method=="espeak"){
            if (!configAlpha.espeak_words.isEmpty()){
                SoundEngine::playSoundFromSpeechSynthesizer(global_path_to_espeak+" "+espeak_params+" \""+configAlpha.espeak_words+"\"",false);
            }
        }else{
            QString filename = configAlpha.sound_letter;
            if (!filename.isEmpty() and QFile::exists(filename)){
                SoundEngine::playSoundFromFile(filename,false);
            }else{
                SoundEngine::playSoundFromSpeechSynthesizer(global_path_to_espeak+" "+espeak_params+" \""+letter+"\"",false);
            }
        }
    }

    setPalette(label,false);
}

void AlphabetTable::setPalette(QLabel *label,bool fill){

    QPalette palette;
    QBrush brush(QColor(0, 0, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);

    if (fill){
        QBrush brush3(QColor(255, 255, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        isPlayLetter=true;
    }else{
        QBrush brush2(QColor(0, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        isPlayLetter=false;
    }

    label->setPalette(palette);
    QCoreApplication::instance()->processEvents();
}


void AlphabetTable::readLetterVariants(){
    listLetterVariants.clear();

    QString currentLanguageAbc=confSettings->value("abc/language","en").toString();

    QStringList filepaths {
        QString(GLOBAL_PATH_USERDATA)+"/langs/variants_"+currentLanguageAbc+".txt",
        QString(GLOBAL_PATH_USERDATA)+"/langs/variants.txt"
    };

    for (auto filename:filepaths){
        QFile file(filename);
        if (!file.exists()) continue;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            while (!file.atEnd()) {
                QString line = file.readLine();
                QStringList pairs = line.split("=");
                if (pairs.size()!=2) continue;

                QString l = pairs.at(1);
                l.replace(QRegExp("\\t|\\s|\\n|\\r"),"");
                listLetterVariants[pairs.at(0)].push_back(l);
            }
            file.close();
        }
    }

}

bool AlphabetTable::isKeypressLetter(QString letter,QString key){
    if (letter.toLower().at(0)==key.toLower().at(0)) return true;

    if (std::find_if(listLetterVariants[letter.toLower()].begin(),listLetterVariants[letter.toLower()].end(),
        [&key](QString var){
            return (key.toLower()==var.toLower());
        }
    )!=listLetterVariants[letter.toLower()].end()){
        return true;
    }

    return  false;
}
