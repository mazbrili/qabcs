#include "FormAbout.h"
#include "ui_FormAbout.h"

#include <QUrl>
#include <QDesktopServices>

#include "config_qabcs.h"
#include "icon.xpm"

FormAbout::FormAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormAbout)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(tr("About"));
    this->setWindowIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"));

    ui->label_2->setText(tr("Version:")+" "+QString(QABCS_VERSION));
    ui->label_3->setText(tr("Date build:")+" "+QString(QABCS_DATEBUILD));
    ui->label_4->setText("© 2018 DanSoft. "+tr("All rights reserved."));
    ui->label_5->setPixmap(QPixmap(icon_xpm));

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(visit_web_site()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(visit_bitbucket()));
}

FormAbout::~FormAbout()
{
    delete ui;
}

void FormAbout::visit_web_site(){
    QDesktopServices::openUrl(QUrl("http://dansoft.krasnokamensk.ru"));
}

void FormAbout::visit_bitbucket(){
    QDesktopServices::openUrl(QUrl("https://bitbucket.org/admsasha/qabcs"));
}
