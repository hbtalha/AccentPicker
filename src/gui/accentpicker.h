// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ACCENTPICKER_H
#define ACCENTPICKER_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QList>
#include <QMouseEvent>

class AccentButton : public QLabel
{
    Q_OBJECT

public:
    explicit AccentButton(const QString &text, int index, QWidget *parent = nullptr);

    void setHighlighted(bool highlighted);
    int getIndex() const
    {
        return index;
    }
    QString getText() const
    {
        return text();
    }

private:
    int index;
    bool highlighted;
    void updateStyle();
};

class AccentPicker : public QWidget
{
    Q_OBJECT

public:
    explicit AccentPicker(QWidget *parent = nullptr);

public slots:
    void keyEvent(bool isPressed, const QString &baseChar);
    void showAccents(const QString &baseChar);
    void hide();

signals:
    void accentSelected(const QString &accent);
    void visibleChanged(bool isVisible);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    void handleAccentSelected(int index);
    void handleButtonHover(int index);

private:
    void clearButtons();
    void goToNextButton();
    void goToPreviousButton();

    QHBoxLayout *layout;
    QList<AccentButton*> buttons;
    int currentIndex;
};

#endif // ACCENTPICKER_H
