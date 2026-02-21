#include "core/accentmap.h"
#include "config/appconfig.h"
#include "charactersetdialog.h"
#include "config/configkeys.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QIcon>



CharacterSetDialog::CharacterSetDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    loadSettings();
    setWindowTitle("Choose character sets");
    resize(900, 600);
}

CharacterSetDialog::~CharacterSetDialog()
{
}

void CharacterSetDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *topLayout = new QHBoxLayout();

    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(QIcon(":/icons/accentpicker.png").pixmap(36, 36));
    topLayout->addWidget(iconLabel);

    QVBoxLayout *headerLayout = new QVBoxLayout();
    QLabel *titleLabel = new QLabel("Choose character sets");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    headerLayout->addWidget(titleLabel);

    QLabel *descLabel = new QLabel("Show only accented characters common to the selected sets");
    descLabel->setStyleSheet("color: #888888;");
    headerLayout->addWidget(descLabel);
    topLayout->addLayout(headerLayout);

    topLayout->addStretch();

    mainLayout->addLayout(topLayout);

    QGroupBox *languageGroup = new QGroupBox("Language sets");
    QFont groupFont = languageGroup->font();
    groupFont.setPointSize(10);
    groupFont.setBold(true);
    languageGroup->setFont(groupFont);

    QGridLayout *languageLayout = new QGridLayout(languageGroup);
    languageLayout->setSpacing(10);

    const auto languageSets = AccentMap::getAllLanguages();
    auto isAllSelected = appConfig->get<ConfigKey::SelectedAllCharacterSets>();

    int row = 0, col = 0;
    for (const auto &lang : languageSets) {
        QCheckBox *cb = createCheckBox(lang, isAllSelected);
        if(lang.language != Language::ALL) {
            languageCheckBoxes[lang.code] = cb;
        }
        languageLayout->addWidget(cb, row, col);

        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }

    mainLayout->addWidget(languageGroup);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &CharacterSetDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(buttonBox);

    mainLayout->addStretch();

    setStyleSheet(R"(
        QDialog {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        QGroupBox {
            border: 1px solid #444444;
            border-radius: 4px;
            margin-top: 10px;
            padding-top: 10px;
            color: #ffffff;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        QCheckBox {
            color: #ffffff;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
        }
        QLabel {
            color: #ffffff;
        }
    )");
}

QCheckBox* CharacterSetDialog::createCheckBox(const LanguageInfo &langInfo, bool initValue)
{
    QCheckBox *checkBox = new QCheckBox(langInfo.name);
    checkBox->setChecked(initValue);

    if(langInfo.language == Language::ALL) {

        checkAllBox = checkBox;
        checkAllBox->setTristate(true);

        connect(checkBox, &QCheckBox::checkStateChanged, [this](Qt::CheckState state) {
            if (state == Qt::Unchecked) {
                setAllCheckBoxes(false);
            } else {
                if(checkAllBox) {
                    QSignalBlocker blocker(checkAllBox);
                    checkAllBox->setChecked(true);
                }
                setAllCheckBoxes(true);
            }
        });
    } else {
        connect(checkBox, &QCheckBox::checkStateChanged,this, &CharacterSetDialog::updateAllCheckState);
    }

    return checkBox;
}

void CharacterSetDialog::updateAllCheckState()
{
    if (!checkAllBox)
        return;

    int checkedCount = 0;

    for(const auto*const cb : std::as_const(languageCheckBoxes) ) {
        if(cb->isChecked()) {
            ++checkedCount;
        }
    }

    QSignalBlocker blocker(checkAllBox);

    if(checkedCount == 0) {
        checkAllBox->setCheckState(Qt::Unchecked);
    } else if(checkedCount == languageCheckBoxes.size()) {
        checkAllBox->setCheckState(Qt::Checked);
    } else {
        checkAllBox->setCheckState(Qt::PartiallyChecked);
    }
}

QStringList CharacterSetDialog::getSelectedSets() const
{
    QStringList selected;

    for (auto it = languageCheckBoxes.constBegin(); it != languageCheckBoxes.constEnd(); ++it) {
        if (it.value()->isChecked()) {
            selected.append(it.key());
        }
    }

    return selected;
}

void CharacterSetDialog::setSelectedSets(const QStringList &sets)
{
    setAllCheckBoxes(false);

    for (const QString &set : sets) {
        if (languageCheckBoxes.contains(set)) {
            languageCheckBoxes[set]->setChecked(true);
        }
    }
}

void CharacterSetDialog::setAllCheckBoxes(bool value)
{
    for (QCheckBox  * const cb : std::as_const(languageCheckBoxes)) {
        QSignalBlocker blocker(cb);
        cb->setChecked(value);
    }
}

void CharacterSetDialog::loadSettings()
{
    auto isAllSelected = appConfig->get<ConfigKey::SelectedAllCharacterSets>();
    if(isAllSelected) {
        return;
    }

    QVariant savedSets = appConfig->get<ConfigKey::SelectedCharacterSets>();
    if (savedSets.isValid()) {
        setSelectedSets(savedSets.toStringList());
    } else {
        if(checkAllBox) {
            checkAllBox->setChecked(true);
        }
    }
}

void CharacterSetDialog::saveSettings()
{
    bool isAllSelected = false;
    if(checkAllBox) {
        isAllSelected = checkAllBox->checkState() == Qt::Checked;
    }

    appConfig->set<ConfigKey::SelectedAllCharacterSets>(isAllSelected);

    if(isAllSelected) {
        return;
    }

    const auto selected = getSelectedSets();
    appConfig->set<ConfigKey::SelectedCharacterSets>(selected);
}

void CharacterSetDialog::accept()
{
    saveSettings();
    QDialog::accept();
}
