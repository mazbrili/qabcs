#include "FormAbout.h"
#include "ui_FormAbout.h"

#include <QUrl>
#include <QDesktopServices>

#include "config_qabcs.h"

FormAbout::FormAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormAbout)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(tr("About"));
    this->setWindowIcon(QIcon(GLOBAL_PATH_USERDATA+"/images/icons/abc.png"));

    ui->label_2->setText(tr("Version: ")+QString(QABCS_VERSION));
    ui->label_3->setText(tr("Date build: ")+QString(QABCS_DATEBUILD));
    ui->label_5->setPixmap(QPixmap(GLOBAL_PATH_USERDATA+"/images/icons/abc.png"));

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(visit_web_site()));
}

FormAbout::~FormAbout()
{
    delete ui;
}

void FormAbout::visit_web_site(){
    QDesktopServices::openUrl(QUrl("http://dansoft.krasnokamensk.ru"));
}
