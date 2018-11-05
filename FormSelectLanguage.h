#ifndef FORMSELECTLANGUAGE_H
#define FORMSELECTLANGUAGE_H

#include <QDialog>
#include <QSettings>
#include <QVector>

namespace Ui {
    class FormSelectLanguage;
}

class FormSelectLanguage : public QDialog {
    Q_OBJECT

    public:
        explicit FormSelectLanguage(QWidget *parent = nullptr);
        ~FormSelectLanguage();

    private:
        Ui::FormSelectLanguage *ui;

        struct ABC_INFO {
            QString folder;
            QString filename;
            QString language;
            QString author;
            QString visible;

            ABC_INFO():visible("true"){}
        };

        QVector<ABC_INFO> listAbcFiles;

        ABC_INFO getLangFromJson(QString filename);
        ABC_INFO getLangFromProperties(QString filename);

        QSettings *confSettings;

    private slots:
        void saveLanguageAbc();
};

#endif // FORMSELECTLANGUAGE_H
