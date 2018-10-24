#ifndef FORMHELP_H
#define FORMHELP_H

#include <QDialog>

namespace Ui {
class FormHelp;
}

class FormHelp : public QDialog {
    Q_OBJECT

    public:
        explicit FormHelp(QWidget *parent = nullptr);
        ~FormHelp();

    private:
        Ui::FormHelp *ui;
    };

#endif // FORMHELP_H
