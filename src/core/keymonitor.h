// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KEYMONITOR_H
#define KEYMONITOR_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <atomic>

struct _XDisplay;
typedef struct _XDisplay Display;
using XRecordContext = unsigned long;

// uninitialized
inline constexpr int UN_INIT = -1;

namespace X11Events
{
inline constexpr int KeyPressEvent = 2;
inline constexpr int KeyReleaseEvent = 3;
}

class KeyMonitorThread : public QThread
{
    Q_OBJECT

public:
    explicit KeyMonitorThread(QObject *parent = nullptr);
    ~KeyMonitorThread();

    void run() override;
    void stop();
    int spaceKeyCode() const;

signals:
    void keyPressed(int keycode, const QString &keysym);
    void keyReleased(int keycode);

private:
    static void eventCallback(void* closure, void *data);

    Display *display;
    Display *dataDisplay;
    XRecordContext context;
    bool running;
    std::atomic<int> m_spaceKeyCode{UN_INIT};

    void closeDisplay();
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
    void withClipboardBackup(const QString& injectedText, const std::function<void()>& operation);

    KeyMonitorThread *monitorThread;

    int currentKeycode;
    QString currentChar;
    bool keyIsHeld;
    bool isAccentPickerVisible = false;
    unsigned long lastWindow;
};

#endif // KEYMONITOR_H
