#ifndef ALPHABETTABLE_H
#define ALPHABETTABLE_H

#include <QDialog>
#include <QVector>
#include <QLabel>
#include <QSettings>
#include <QKeyEvent>

#include "LoaderAbcFormats.h"

namespace Ui {
class AlphabetTable;
}

class AlphabetTable : public QDialog
{
    Q_OBJECT

    public:
        explicit AlphabetTable(QWidget *parent = nullptr);
        ~AlphabetTable() override;

    private:
        Ui::AlphabetTable *ui;
        QSettings *confSettings;

        ABC_CONFIG config_current;
        QVector<QLabel*> listLabelLetters;

        void setPalette(QLabel *label, bool fill);
        void playLetter(QLabel *label);

        bool isPlayLetter;

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

};

#endif // ALPHABETTABLE_H
