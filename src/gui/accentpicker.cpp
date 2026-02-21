// SPDX-License-Identifier: GPL-3.0-or-later

#include "gui/accentpicker.h"
#include "core/accentmap.h"
#include "config/appconfig.h"
#include "config/configkeys.h"

#include <QKeyEvent>
#include <QApplication>
#include <QScreen>

AccentButton::AccentButton(const QString &text, int index, QWidget *parent)
    : QLabel(text, parent), index(index), highlighted(false)
{

    setAlignment(Qt::AlignCenter);
    setMinimumSize(40, 40);
    setCursor(Qt::PointingHandCursor);
    updateStyle();
}

void AccentButton::setHighlighted(bool highlighted)
{
    this->highlighted = highlighted;
    updateStyle();
}

void AccentButton::updateStyle()
{
    QString bgColor = highlighted ? "#4A90E2" : "#2C2C2C";
    QString textColor = highlighted ? "#FFFFFF" : "#CCCCCC";

    setStyleSheet(QString(
                      "QLabel {"
                      "    background-color: %1;"
                      "    color: %2;"
                      "    border-radius: 6px;"
                      "    font-size: 20px;"
                      "    font-weight: bold;"
                      "    padding: 8px;"
                      "}"
                  ).arg(bgColor, textColor));
}

AccentPicker::AccentPicker(QWidget *parent)
    : QWidget(parent), currentIndex(-1)
{

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    setStyleSheet(
        "QWidget {"
        "    background-color: rgba(44, 44, 44, 240);"
        "    border: 1px solid #555555;"
        "    border-radius: 8px;"
        "}"
    );
}

void AccentPicker::keyEvent(bool isPressed, const QString &baseChar)
{
    if(isPressed) {
        showAccents(baseChar);
    } else {
        handleAccentSelected(currentIndex);
    }
}

void AccentPicker::showAccents(const QString &baseChar)
{
    if(isVisible()) {
        hide();
    }

    if (baseChar.isEmpty()) return;

    const auto character = baseChar[0];
    const auto languages = appConfig->get<ConfigKey::SelectedCharacterSets>();

    QStringList accents = AccentMap::getAccents(character, languages);

    if (accents.isEmpty()) return;

    auto getEffectiveLetter = [character](const QString& letter) {
        return character.isUpper() ? letter.toUpper():letter;
    };

    clearButtons();

    // Create buttons for each accent
    for (int i = 0; i < accents.size(); ++i) {
        AccentButton *button = new AccentButton(getEffectiveLetter(accents[i]), i, this);
        layout->addWidget(button);
        buttons.append(button);
    }

    adjustSize();

    QRect screenGeometry = QApplication::primaryScreen()->geometry();

    int x = screenGeometry.x() + (screenGeometry.width() - this->width()) / 2;
    int y = screenGeometry.y() + 20;

    // Adjust if off-screen
    if (x < screenGeometry.left()) x = screenGeometry.left();
    if (x + width() > screenGeometry.right()) x = screenGeometry.right() - width();
    if (y < screenGeometry.top()) y =  screenGeometry.top() + 20;

    move(x, y);
    show();
    activateWindow();
    setFocus();

    emit visibleChanged(true);
    handleButtonHover(0);
}

void AccentPicker::hide()
{
    clearButtons();
    QWidget::hide();
}

void AccentPicker::clearButtons()
{
    for (AccentButton *button : std::as_const(buttons)) {
        layout->removeWidget(button);
        button->deleteLater();
    }
    buttons.clear();
    currentIndex = -1;
}

void AccentPicker::handleAccentSelected(int index)
{
    if (index >= 0 && index < buttons.size()) {
        emit accentSelected(buttons[index]->getText());
        hide();
    }
}

void AccentPicker::handleButtonHover(int index)
{
    if (currentIndex >= 0 && currentIndex < buttons.size()) {
        buttons[currentIndex]->setHighlighted(false);
    }

    currentIndex = index;
    if (currentIndex >= 0 && currentIndex < buttons.size()) {
        buttons[currentIndex]->setHighlighted(true);
    }
}

void AccentPicker::goToNextButton()
{
    if (buttons.isEmpty()) {
        return;
    }

    auto buttonsSize = buttons.size() - 1;

    if (currentIndex < buttonsSize) {
        handleButtonHover(currentIndex + 1);
    } else if(currentIndex >= buttonsSize) {
        handleButtonHover(0);
    }
}

void AccentPicker::goToPreviousButton()
{
    if (buttons.isEmpty()) {
        return;
    }

    if (currentIndex > 0) {
        handleButtonHover(currentIndex - 1);
    } else {
        handleButtonHover(buttons.size() - 1);
    }
}


void AccentPicker::keyPressEvent(QKeyEvent *event)
{
    if (buttons.isEmpty()) {
        QWidget::keyPressEvent(event);
        return;
    }

    auto key = event->key();

    if (key == Qt::Key_Right || key == Qt::Key_Space) {
        goToNextButton();
        return;
    } else if (key == Qt::Key_Left) {
        goToPreviousButton();
        return;
    }

    QWidget::keyPressEvent(event);
}

void AccentPicker::hideEvent(QHideEvent *)
{
    emit visibleChanged(false);
}
