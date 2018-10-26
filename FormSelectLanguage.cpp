#include "FormSelectLanguage.h"
#include "ui_FormSelectLanguage.h"

#include <QDir>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegExp>
#include <QDebug>

#include "config_qabcs.h"

FormSelectLanguage::FormSelectLanguage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSelectLanguage)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(tr("Select language abc"));
    this->setWindowIcon(QIcon(GLOBAL_PATH_USERDATA+"/images/icons/abc.png"));

    QDir dirConfig(QDir::homePath()+"/.qabcs/");
    if (dirConfig.exists()==false) dirConfig.mkpath(QDir::homePath()+"/.qabcs/");

    confSettings = new QSettings(dirConfig.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    confSettings->setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());



    QDir dir(GLOBAL_PATH_USERDATA+"/abcs/");
    dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        if (fileInfo.fileName()=="all") continue;

        ABC_INFO lang_info;

        if (QFile::exists(GLOBAL_PATH_USERDATA+"/abcs/"+fileInfo.fileName()+"/abc.json")){
            lang_info = getLangFromJson(GLOBAL_PATH_USERDATA+"/abcs/"+fileInfo.fileName()+"/abc.json");
        }
        if (QFile::exists(GLOBAL_PATH_USERDATA+"/abcs/"+fileInfo.fileName()+"/abc.properties")){
            lang_info = getLangFromProperties(GLOBAL_PATH_USERDATA+"/abcs/"+fileInfo.fileName()+"/abc.properties");
        }

        QString lang = fileInfo.fileName();
        ui->comboBox->addItem(lang_info.language+"  ("+lang_info.author+")",lang);
        if (confSettings->value("abc/language","en").toString()==lang){
            ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
        }
    }

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(saveLanguageAbc()));
}

FormSelectLanguage::~FormSelectLanguage(){
    delete ui;
}

FormSelectLanguage::ABC_INFO FormSelectLanguage::getLangFromJson(QString filename){
    ABC_INFO result;

    QByteArray val;
    QFile file;
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return ABC_INFO();
    }
    val = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val);
    if (document.isEmpty()){
        return ABC_INFO();
    }
    QJsonObject root = document.object();
    QJsonObject root_general = root.value("general").toObject();

    result.language = root_general.value("language").toString();
    result.author = root_general.value("author").toString();

    return result;
}

FormSelectLanguage::ABC_INFO FormSelectLanguage::getLangFromProperties(QString filename){
    ABC_INFO result;

    QRegExp rx("(.*)(:)(.*)");

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return ABC_INFO();
    }
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (rx.indexIn(line)!=-1){
            if (rx.cap(1)=="language"){
                result.language = rx.cap(3).replace(QRegExp("\\s"),"");
            }
            if (rx.cap(1)=="author"){
                result.author = rx.cap(3).replace(QRegExp("\\s"),"");
            }
        }
    }
    file.close();

    return result;
}

void FormSelectLanguage::saveLanguageAbc(){
    if (ui->comboBox->currentIndex()==-1) return;

    confSettings->setValue("abc/language",ui->comboBox->currentData());
    accept();
}
