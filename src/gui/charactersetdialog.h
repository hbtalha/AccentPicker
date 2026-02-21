#ifndef CHARACTERSETDIALOG_H
#define CHARACTERSETDIALOG_H

#include "core/accentmap.h"
#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QMap>
#include <QStringList>


class CharacterSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CharacterSetDialog(QWidget *parent = nullptr);
    ~CharacterSetDialog();



private slots:
    void updateAllCheckState();

protected:
    void accept() override;

private:
    void setupUI();
    QCheckBox* createCheckBox(const LanguageInfo &lang, bool initValue);

    QCheckBox *checkAllBox;
    QMap<QString, QCheckBox*> languageCheckBoxes;

    QStringList getSelectedSets() const;
    void setSelectedSets(const QStringList &sets);
    void setAllCheckBoxes(bool value);

    void loadSettings();
    void saveSettings();
};

#endif // CHARACTERSETDIALOG_H
