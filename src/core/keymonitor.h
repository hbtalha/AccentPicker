// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KEYMONITOR_H
#define KEYMONITOR_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMap>

struct _XDisplay;
typedef struct _XDisplay Display;
using XRecordContext = unsigned long;

namespace X11Events
{
const int KeyPressEvent = 2;
const int KeyReleaseEvent = 3;
}

class KeyMonitorThread : public QThread
{
    Q_OBJECT

public:
    explicit KeyMonitorThread(QObject *parent = nullptr);
    ~KeyMonitorThread();

    void run() override;
    void stop();

signals:
    void keyPressed(int keycode, const QString &keysym);
    void keyReleased(int keycode);

private:
    static void eventCallback(void* closure, void *data);

    Display *display;
    Display *dataDisplay;
    XRecordContext context;
    bool running;
};

class KeyMonitor : public QObject
{
    Q_OBJECT

public:
    explicit KeyMonitor(QObject *parent = nullptr);
    ~KeyMonitor();

    bool start();
    void stop();
    void setActive(bool enabled);

public slots:
    void insertText(const QString &text);
    void accentPickerVisible(bool isVisible);

signals:
    void keyEvent(bool isPressed, const QString &character);

private slots:
    void handleKeyPress(int keycode, const QString &keysym);
    void handleKeyRelease(int keycode);
    void checkKeyHold();

private:
    QPoint getCursorPosition();
    void simulateBackspace();

    KeyMonitorThread *monitorThread;

    int currentKeycode;
    QString currentChar;
    bool keyIsHeld;
    bool isAccentPickerVisible = false;
    unsigned long lastWindow;

    static const int HOLD_THRESHOLD_MS = 500;
};

#endif // KEYMONITOR_H
