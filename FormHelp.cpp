#include "FormHelp.h"
#include "ui_FormHelp.h"

#include "config_qabcs.h"

FormHelp::FormHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormHelp)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Help"));
    this->setWindowIcon(QIcon(QString(GLOBAL_PATH_USERDATA)+"/images/icons/abc.png"));


    ui->textEdit->setText(tr("How to use:\n\n")+
      tr("Selecting a mode:\n\
        The six icons on the left side of the toolbar represent the\n\
        different modes. Use the \"Tab\" key to toggle between modes,\n\
        or click on an icon with the mouse.\n")+
    "\n"+
      tr("Alphabet Mode:\n\
        The first icon on the left is the Alphabet Mode. Press the\n\
        letter on the keyboard that is displayed on the screen to\n\
        cycle through alphabet. Press the \"Backspace\" key to move\n\
        back one letter. Finish by finding all the letters.\n")+
    "\n"+
      tr("Random Mode:\n\
        The second icon on the left is the Random Mode. Press the\n\
        letter on the keyboard that is displayed on the screen.\n\
        Press the \"Backspace\" key to move back one letter.\n\
        Finish by finding all the letters.\n")+
    "\n"+
      tr("Other Modes:\n\
        In all other modes, simply press a key on the keyboard to see\n\
        a letter with a related image and description.\n")+
    "\n"+
      tr("Sounds:\n\
        Press the spacebar to hear the name of the pictured object.\n\
        You can turn on/off sounds by pressing on the \"Sound on/off\"\n\
        icon.\n")+
    "\n"+
      tr("Select ABC language:\n\
        You can change ABC language by pressing on the \"Select\n\
        language\" icon. Default language is American English.")
    );


    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));

}

FormHelp::~FormHelp()
{
    delete ui;
}
