#include "FormSelectLanguage.h"
#include "ui_FormSelectLanguage.h"

#include <QDir>
#include <QDebug>
#include <QRegExp>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

#include "config_qabcs.h"
#include "LoaderAbcFormats.h"

FormSelectLanguage::FormSelectLanguage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSelectLanguage)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(tr("Select language"));
    this->setWindowIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"));

    QDir dirConfig(QDir::homePath()+"/.qabcs/");
    if (dirConfig.exists()==false) dirConfig.mkpath(QDir::homePath()+"/.qabcs/");

    confSettings = new QSettings(dirConfig.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    confSettings->setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());

    int count=1;

    QDir dir(QString(GLOBAL_PATH_USERDATA)+"/abcs/");
    dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name|QDir::Type);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.fileName()=="all") continue;
        QString lang = fileInfo.fileName();

        QDir dir2(QString(GLOBAL_PATH_USERDATA)+"/abcs/"+lang+"/");
        dir2.setFilter(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
        dir2.setSorting(QDir::Name|QDir::Type);
        QFileInfoList list2 = dir2.entryInfoList();
        for (int y = 0; y < list2.size(); ++y) {
            QFileInfo fileInfo2 = list2.at(y);
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

            ui->comboBox->addItem(QString::number(count)+". "+lang_info.language+"  ("+lang_info.author+")");
            if (confSettings->value("abc/language","en").toString()==lang and confSettings->value("abc/filename","abc.json").toString()==lang_filename){
                ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
            }

            count++;
        }
    }


    // read langs.json
    QJsonObject rootLangs;
    QFile file;
    file.setFileName(QString(GLOBAL_PATH_USERDATA)+"/langs/langs.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << tr("Error while opening")+" langs.json" << file.errorString();
    }else{
        QByteArray val = file.readAll();
        file.close();

        QJsonDocument document = QJsonDocument::fromJson(val);
        if (document.isEmpty()){
            qDebug() << "langs.json "+tr("is not valid");
        }else{
            rootLangs = document.object();
        }
    }

    // loading languages
    ui->comboBox_2->clear();
    QDir dirLang(QString(GLOBAL_PATH_USERDATA)+"/langs/");
    dirLang.setFilter(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
    QFileInfoList listLang = dirLang.entryInfoList();
    for (int i = 0; i < listLang.size(); ++i) {
        QFileInfo fileInfo = listLang.at(i);
        QRegExp rx("(qabcs_)(.*)(.qm)");
        if (rx.indexIn(fileInfo.fileName())!=-1){
            QString lang_code = rx.cap(2);

            QString lang_code_loc = lang_code;
            if (lang_code_loc.indexOf("_")==-1) lang_code_loc+="_"+lang_code_loc.toUpper();

            QString languageName = "";

            if (rootLangs.value(lang_code).toObject().value("nativeName").isString()){
                languageName = rootLangs.value(lang_code).toObject().value("nativeName").toString();
            }

            if (languageName.isEmpty()){
                QLocale loc(lang_code_loc);
                languageName = loc.nativeLanguageName();

                if (languageName.isEmpty()){
                    languageName=QLocale::languageToString(loc.language());
                }

            }

            if (!languageName.isEmpty()) languageName[0]=languageName[0].toUpper();

            if (languageName.isEmpty() or languageName=="C"){
                    languageName=lang_code;
            }

            ui->comboBox_2->addItem(languageName,lang_code);
            if (confSettings->value("abc/lang",QLocale::system().bcp47Name()).toString()==lang_code){
                ui->comboBox_2->setCurrentIndex(ui->comboBox_2->count()-1);
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

    confSettings->setValue("abc/lang",ui->comboBox_2->currentData());
    confSettings->setValue("abc/language",abc_info.folder);
    confSettings->setValue("abc/filename",abc_info.filename);
    accept();
}
