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
    this->setWindowTitle(tr("Select ABC language"));
    this->setWindowIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"));

    QDir dirConfig(QDir::homePath()+"/.qabcs/");
    if (dirConfig.exists()==false) dirConfig.mkpath(QDir::homePath()+"/.qabcs/");

    confSettings = new QSettings(dirConfig.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    confSettings->setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());



    QDir dir(QString(GLOBAL_PATH_USERDATA)+"/abcs/");
    dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.fileName()=="all") continue;
        QString lang = fileInfo.fileName();

        QDir dir2(QString(GLOBAL_PATH_USERDATA)+"/abcs/"+lang+"/");
        dir2.setFilter(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
        QFileInfoList list2 = dir2.entryInfoList();
        for (int i = 0; i < list2.size(); ++i) {
            QFileInfo fileInfo2 = list2.at(i);
            QString lang_filename = fileInfo2.fileName();
            if (lang_filename.indexOf(QRegExp("(.*)(.json|.properties)$"))==-1) continue;

            ABC_INFO lang_info;

            if (lang_filename.indexOf(QRegExp("(.*)(.json)$"))!=-1){
                lang_info = getLangFromJson(QString(GLOBAL_PATH_USERDATA)+"/abcs/"+lang+"/"+lang_filename);
            }
            if (lang_filename.indexOf(QRegExp("(.*)(.properties)$"))!=-1){
                lang_info = getLangFromProperties(QString(GLOBAL_PATH_USERDATA)+"/abcs/"+lang+"/"+lang_filename);
            }
            lang_info.folder=lang;
            lang_info.filename=lang_filename;

            if (lang_info.visible=="false") continue;

            listAbcFiles.push_back(lang_info);

            ui->comboBox->addItem(lang_info.language+"  ("+lang_info.author+")");
            if (confSettings->value("abc/language","en").toString()==lang and confSettings->value("abc/filename","abc.json").toString()==lang_filename){
                ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
            }
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
    result.visible = root_general.value("visible").toString();

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
                result.language = rx.cap(3).replace(QRegExp("(\\s*)$"),"");
            }
            if (rx.cap(1)=="author"){
                result.author = rx.cap(3).replace(QRegExp("(\\s*)$"),"");
            }
            if (rx.cap(1)=="visible"){
                result.visible = rx.cap(3).replace(QRegExp("(\\s*)$"),"");
            }
        }
    }
    file.close();

    return result;
}

void FormSelectLanguage::saveLanguageAbc(){
    if (ui->comboBox->currentIndex()==-1) return;

    ABC_INFO abc_info = listAbcFiles.at(ui->comboBox->currentIndex());

    confSettings->setValue("abc/language",abc_info.folder);
    confSettings->setValue("abc/filename",abc_info.filename);
    accept();
}
