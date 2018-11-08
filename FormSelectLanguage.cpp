#include "FormSelectLanguage.h"
#include "ui_FormSelectLanguage.h"

#include <QDir>

#include "config_qabcs.h"
#include "LoaderAbcFormats.h"

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
            ABC_CONFIG abcConfig = LoaderAbcFormats::LoadFilename(QString(GLOBAL_PATH_USERDATA)+"/abcs/"+lang+"/"+lang_filename);

            lang_info.author = abcConfig.author;
            lang_info.language = abcConfig.language;
            lang_info.visible = (!abcConfig.visible.isNull()) ? abcConfig.visible : "true";
            lang_info.folder = lang;
            lang_info.filename = lang_filename;

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

void FormSelectLanguage::saveLanguageAbc(){
    if (ui->comboBox->currentIndex()==-1) return;

    ABC_INFO abc_info = listAbcFiles.at(ui->comboBox->currentIndex());

    confSettings->setValue("abc/language",abc_info.folder);
    confSettings->setValue("abc/filename",abc_info.filename);
    accept();
}
