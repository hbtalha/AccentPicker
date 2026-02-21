#include <QApplication>
#include <QScreen>
#include <QCursor>
#include <QClipboard>
#include <QMimeData>
#include <QUrl>

#include "keymonitor.h"
#include "config/configkeys.h"
#include "config/appconfig.h"
#include "platform/x11/x11platformwindow.h"
#include "core/accentmap.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/record.h>

#include <chrono>

KeyMonitorThread::KeyMonitorThread(QObject *parent)
    : QThread(parent), display(nullptr), dataDisplay(nullptr),
      context(0), running(false)
{
}

KeyMonitorThread::~KeyMonitorThread()
{
    stop();
    wait();
}

void KeyMonitorThread::eventCallback(void* closure, void *rawData)
{
    XRecordInterceptData *data =
        static_cast<XRecordInterceptData*>(rawData);

    if (data->category != XRecordFromServer) {
        XRecordFreeData(data);
        return;
    }

    KeyMonitorThread *self =
        reinterpret_cast<KeyMonitorThread*>(closure);

    const unsigned char *eventData =
        reinterpret_cast<const unsigned char*>(data->data);

    const int eventType = eventData[0];
    const int keycode   = eventData[1];

    if (eventType == KeyPress || eventType == KeyRelease) {

        const bool pressed = (eventType == KeyPress);

        // Determine modifier state (Shift / CapsLock) and pick the
        // appropriate keysym level so letter case is preserved.
        XkbStateRec xkbState;
        int level = 0;
        if (XkbGetState(self->display, XkbUseCoreKbd, &xkbState) == Success) {
            const bool shift = (xkbState.mods & ShiftMask);
            const bool caps = (xkbState.mods & LockMask);
            level = (shift ^ caps) ? 1 : 0;
        }

        KeySym keysym = XkbKeycodeToKeysym(self->display, keycode, 0, level);

        QString keysymStr = QString::fromLatin1(XKeysymToString(keysym));

        if (pressed) {
            emit self->keyPressed(keycode, keysymStr);
        } else {
            emit self->keyReleased(keycode);
        }
    }

    XRecordFreeData(data);
}

void KeyMonitorThread::run()
{
    display = XOpenDisplay(nullptr);
    dataDisplay = XOpenDisplay(nullptr);

    if (!display || !dataDisplay) {
        qWarning() << "Failed to open display";
        return;
    }

    XRecordClientSpec clients = XRecordAllClients;
    XRecordRange *range = XRecordAllocRange();

    if (!range) {
        qWarning() << "Failed to allocate XRecordRange";
        return;
    }

    range->device_events.first = X11Events::KeyPressEvent;
    range->device_events.last = X11Events::KeyReleaseEvent;

    context = XRecordCreateContext(dataDisplay, 0, &clients, 1, &range, 1);
    XFree(range);

    if (!context) {
        qWarning() << "Failed to create XRecord context";
        return;
    }

    running = true;
    XRecordEnableContextAsync(dataDisplay, context,
                              reinterpret_cast<XRecordInterceptProc>(eventCallback),
                              reinterpret_cast<XPointer>(this));

    while (running) {
        XRecordProcessReplies(dataDisplay);
        sleep(std::chrono::milliseconds(100));
    }
}

void KeyMonitorThread::stop()
{
    if (!running)
        return;

    running = false;

    if (context && display) {
        XRecordDisableContext(display, context);
        XRecordFreeContext(display, context);
        context = 0;
    }

    if (display) {
        XCloseDisplay(display);
        display = nullptr;
    }

    if (dataDisplay) {
        XCloseDisplay(dataDisplay);
        dataDisplay = nullptr;
    }
}


KeyMonitor::KeyMonitor(QObject *parent)
    : QObject(parent), currentKeycode(-1), keyIsHeld(false)
{

    monitorThread = new KeyMonitorThread(this);

    connect(monitorThread, &KeyMonitorThread::keyPressed,
            this, &KeyMonitor::handleKeyPress);
    connect(monitorThread, &KeyMonitorThread::keyReleased,
            this, &KeyMonitor::handleKeyRelease);
}

KeyMonitor::~KeyMonitor()
{
    stop();
}

void KeyMonitor::setActive(bool enabled)
{
    if (enabled)
        start();
    else
        stop();
}

bool KeyMonitor::start()
{
    monitorThread->start();
    return true;
}

void KeyMonitor::stop()
{
    monitorThread->stop();
}

void KeyMonitor::handleKeyPress(int keycode, const QString &character)
{
    if(character.isEmpty()) {
        return;
    }

    Display *display =  XOpenDisplay(nullptr);
    if (!display) {
        return;
    }

    bool isSpaceKeycode = XKeysymToKeycode( display, XK_space) == keycode;

    if(keyIsHeld && !isAccentPickerVisible && isSpaceKeycode) {

        if(currentChar.isEmpty()) {
            return;
        }

        QStringList accents = AccentMap::getAccents(currentChar[0], appConfig->get<ConfigKey::SelectedCharacterSets>());

        if(accents.isEmpty()) {
            return;
        }

        // removes the space key
        simulateBackspace();

        checkKeyHold();
    } else {

        if (!keyIsHeld && character.length() == 1 ) {
            currentKeycode = keycode;
            currentChar = character[0];
            keyIsHeld = true;
        }
    }
}

void KeyMonitor::handleKeyRelease(int keycode)
{
    if (keycode == currentKeycode) {

        if(isAccentPickerVisible) {
            emit emit keyEvent(false, currentChar);
        }
        currentKeycode = -1;
        keyIsHeld = false;
        currentChar.clear();
    }
}

void KeyMonitor::checkKeyHold()
{
    if (currentKeycode != -1 && !currentChar.isEmpty()) {

        lastWindow = getCurrentWindow();

        emit keyEvent(true, currentChar);
    }
}

void KeyMonitor::accentPickerVisible(bool isVisible)
{
    isAccentPickerVisible = isVisible;
}

QPoint KeyMonitor::getCursorPosition()
{
    return QCursor::pos();
}

void KeyMonitor::simulateBackspace()
{
    Display *display =  XOpenDisplay(nullptr);
    if (!display) {
        return;
    }

    KeyCode backspace = XKeysymToKeycode(display, XK_BackSpace);

    XTestFakeKeyEvent(display, backspace, True, 0);
    XTestFakeKeyEvent(display, backspace, False, 0);
    XFlush(display);
    XCloseDisplay(display);
}

static QMimeData* cloneMimeData(const QMimeData* src)
{
    if (!src)
        return nullptr;

    QMimeData* dst = new QMimeData();

    // Common high-level helpers (nice to preserve semantics)

    if (src->hasImage())
        dst->setImageData(src->imageData());

    if (src->hasText())
        dst->setText(src->text());

    if (src->hasHtml())
        dst->setHtml(src->html());

    if (src->hasUrls())
        dst->setUrls(src->urls());

    if (src->hasColor())
        dst->setColorData(src->colorData());

    // Copy *all* remaining raw formats
    const QStringList formats = src->formats();
    for (const QString& format : formats) {
        if (!dst->hasFormat(format)) {
            dst->setData(format, src->data(format));
        }
    }

    return dst;
}


void KeyMonitor::insertText(const QString &text)
{
    auto cb = QGuiApplication::clipboard();

    const QMimeData *oldMime = cb->mimeData(QClipboard::Clipboard);
    QMimeData *backup = nullptr;

    if (oldMime) {
        backup = cloneMimeData(oldMime);
    }

    cb->setText(text, QClipboard::Clipboard);
    cb->setText(text, QClipboard::Selection);

    X11PlatformWindow window(lastWindow);

    window.raise();

    // remove the letter intended to be replaced with an accented variant
    simulateBackspace();

    window.pasteClipboard();

    QTimer::singleShot(50, [cb, backup]() {
        cb->setMimeData(backup);
    });
}

