#include "FormHelp.h"
#include "ui_FormHelp.h"

#include "config_qabcs.h"

FormHelp::FormHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormHelp)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Help"));
    this->setWindowIcon(QIcon(GLOBAL_PATH_USERDATA+"/images/abc.png"));


    ui->textEdit->setText(tr("How to use:\n\
      Selecting a mode:\n\
        The four icons on the left side of the toolbar represent the\n\
        different modes.  Use the \"Tab\" key to toggle between modes,\n\
        or click on an icon with the mouse.\n\
    \n\
      Alphabet Mode:\n\
        The first icon on the left is the Alphabet Mode.  Press the\n\
        letter on the keyboard that is displayed on the screen to\n\
        cycle through the English alphabet.  Press the \"Backspace\"\n\
        key to move back one letter.  Finish by finding all the\n\
        letters, A-Z.\n\
    \n\
      Other Modes:\n\
        In all other modes, simply press a key on the keyboard to see\n\
        a letter with a related image and description.\n\
    \n\
      Sounds:\n\
        Press the spacebar to hear the name of the pictured object."));


    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));

}

FormHelp::~FormHelp()
{
    delete ui;
}
