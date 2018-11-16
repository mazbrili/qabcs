#include "FormSelectLanguage.h"
#include "ui_FormSelectLanguage.h"

#include <QDir>
#include <QRegExp>

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
            if (lang_code_loc.indexOf("_")==-1){
                lang_code_loc+="_"+lang_code_loc.toUpper();
            }

            QLocale loc(lang_code_loc);
            QString languageName = loc.nativeLanguageName();
            if (lang_code_loc=="en_EN") languageName="English";
            if (languageName.isEmpty()){
                if (lang_code_loc=="aa_AA") languageName="Afar";
                if (lang_code_loc=="ab_AB") languageName="Abkhazian";
                if (lang_code_loc=="ae_AE") languageName="Avestan";
                if (lang_code_loc=="an_AN") languageName="Aragonese";
                if (lang_code_loc=="av_AV") languageName="Avaric";
                if (lang_code_loc=="ay_AY") languageName="Aymara";
                if (lang_code_loc=="ba_BA") languageName="Bashkir";
                if (lang_code_loc=="bh_BH") languageName="Bihari";
                if (lang_code_loc=="bi_BI") languageName="Bislama";
                if (lang_code_loc=="ch_CH") languageName="Chamoru";
                if (lang_code_loc=="co_CO") languageName="Corsican";
                if (lang_code_loc=="cr_CR") languageName="Cree";
                if (lang_code_loc=="cv_CV") languageName="Chuvash";
                if (lang_code_loc=="dv_DV") languageName="Divehi; Maldivian";
                if (lang_code_loc=="eo_EO") languageName="Esperanto";
                if (lang_code_loc=="fj_FJ") languageName="Fijian; Fiji";
                if (lang_code_loc=="gn_GN") languageName="Guarani";
                if (lang_code_loc=="ho_HO") languageName="Hiri Motu";
                if (lang_code_loc=="ht_HT") languageName="Haitian; Haitian Creole";
                if (lang_code_loc=="hz_HZ") languageName="Herero";
                if (lang_code_loc=="ia_IA") languageName="Interlingua";
                if (lang_code_loc=="ie_IE") languageName="Interlingue; Occidental";
                if (lang_code_loc=="ik_IK") languageName="Inupiak; Inupiaq";
                if (lang_code_loc=="io_IO") languageName="Ido";
                if (lang_code_loc=="iu_IU") languageName="Inuktitut";
                if (lang_code_loc=="jv_JV") languageName="Javanese";
                if (lang_code_loc=="kg_KG") languageName="Kongo";
                if (lang_code_loc=="kj_KJ") languageName="Kuanyama; Kwanyama";
                if (lang_code_loc=="kr_KR") languageName="Kanuri";
                if (lang_code_loc=="ku_KU") languageName="Kurdish";
                if (lang_code_loc=="kv_KV") languageName="Komi";
                if (lang_code_loc=="la_LA") languageName="Latin";
                if (lang_code_loc=="li_LI") languageName="Limburgish; Limburger; Limburgan";
                if (lang_code_loc=="mh_MH") languageName="Marshallese";
                if (lang_code_loc=="mi_MI") languageName="Maori";
                if (lang_code_loc=="na_NA") languageName="Nauru";
                if (lang_code_loc=="ng_NG") languageName="Ndonga";
                if (lang_code_loc=="nr_NR") languageName="Ndebele, South";
                if (lang_code_loc=="nv_NV") languageName="Navajo; Navaho";
                if (lang_code_loc=="ny_NY") languageName="Chichewa; Nyanja";
                if (lang_code_loc=="oc_OC") languageName="Occitan; Provençal";
                if (lang_code_loc=="oj_OJ") languageName="Ojibwa";
                if (lang_code_loc=="pi_PI") languageName="Pali";
                if (lang_code_loc=="sa_SA") languageName="Sanskrit";
                if (lang_code_loc=="sc_SC") languageName="Sardinian";
                if (lang_code_loc=="sd_SD") languageName="Sindhi";
                if (lang_code_loc=="sm_SM") languageName="Samoan";
                if (lang_code_loc=="ss_SS") languageName="Swati; Siswati";
                if (lang_code_loc=="st_ST") languageName="Sesotho; Sotho, Southern";
                if (lang_code_loc=="su_SU") languageName="Sundanese";
                if (lang_code_loc=="tg_TG") languageName="Tajik";
                if (lang_code_loc=="tn_TN") languageName="Tswana; Setswana";
                if (lang_code_loc=="ts_TS") languageName="Tsonga";
                if (lang_code_loc=="tt_TT") languageName="Tatar";
                if (lang_code_loc=="tw_TW") languageName="Twi";
                if (lang_code_loc=="ty_TY") languageName="Tahitian";
                if (lang_code_loc=="ve_VE") languageName="Venda";
                if (lang_code_loc=="vo_VO") languageName="Volapük; Volapuk";
                if (lang_code_loc=="wa_WA") languageName="Walloon";
                if (lang_code_loc=="wo_WO") languageName="Wolof";
                if (lang_code_loc=="xh_XH") languageName="Xhosa";
                if (lang_code_loc=="yi_YI") languageName="Yiddish";
                if (lang_code_loc=="za_ZA") languageName="Zhuang";
            }

            languageName[0]=languageName[0].toUpper();

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
