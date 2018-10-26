#ifndef FORMSELECTLANGUAGE_H
#define FORMSELECTLANGUAGE_H

#include <QDialog>
#include <QSettings>

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
            QString language;
            QString author;
        };

        ABC_INFO getLangFromJson(QString filename);
        ABC_INFO getLangFromProperties(QString filename);

        QSettings *confSettings;

    private slots:
        void saveLanguageAbc();
};

#endif // FORMSELECTLANGUAGE_H
