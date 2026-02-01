// Include Qt headers first, before X11 headers
#include <QApplication>
#include <QScreen>
#include <QCursor>
#include <QDebug>
#include <QClipboard>
#include <QMimeData>
#include <QUrl>

#include "x11platformwindow.h"
#include <X11/keysym.h>
#include <X11/XKBlib.h>

#include "accentmap.h"
#include "keymonitor.h"

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

void KeyMonitorThread::eventCallback(XPointer closure, XRecordInterceptData *data)
{
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

        KeySym keysym =
            XkbKeycodeToKeysym(self->display, keycode, 0, 0);

        QString keysymStr =
            QString::fromLatin1(XKeysymToString(keysym));

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
    XRecordEnableContextAsync(dataDisplay, context, eventCallback,
                              reinterpret_cast<XPointer>(this));

    while (running) {
        XRecordProcessReplies(dataDisplay);
    }

    XRecordDisableContext(display, context);
    XRecordFreeContext(display, context);
    XCloseDisplay(display);
    XCloseDisplay(dataDisplay);
}

void KeyMonitorThread::stop()
{
    running = false;
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

bool KeyMonitor::start()
{
    monitorThread->start();
    return true;
}

void KeyMonitor::stop()
{
    monitorThread->stop();
}

void KeyMonitor::handleKeyPress(int keycode, const QString &keysym)
{
    QString character = getCharFromKeysym(keysym);

    if(keyIsHeld && !isAccentPickerVisible && keysym == "space") {

        // removes the space key
        simulateBackspace();

        checkKeyHold();
    } else if (!keyIsHeld && character.length() == 1 && AccentMap::getAccents(character[0]).count() > 0) {
        currentKeycode = keycode;
        currentChar = character;
        keyIsHeld = true;
    }
}

void KeyMonitor::handleKeyRelease(int keycode)
{
    if (keycode == currentKeycode) {
        currentKeycode = -1;
        currentChar.clear();
        keyIsHeld = false;

        if(isAccentPickerVisible)
            emit emit keyEvent(false, currentChar);
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

QString KeyMonitor::getCharFromKeysym(const QString &keysym)
{
    if (keysym.isEmpty()) {
        return QString();
    }

    // Handle single character keys
    if (keysym.length() == 1) {
        return keysym;
    }

    static QMap<QString, QString> specialKeys = {
        {"space", " "},
        {"minus", "-"},
        {"apostrophe", "'"},
        {"quotedbl", "\""},
        {"exclam", "!"},
        {"question", "?"},
        {"period", "."},
        {"comma", ","}
    };

    if (specialKeys.contains(keysym)) {
        return specialKeys[keysym];
    }

    return QString();
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

    X11PlatformWindow window(lastWindow);

    window.raise();

    // remove the letter intended to be replaced with an accented variant
    simulateBackspace();

    window.pasteClipboard();

    QTimer::singleShot(50, [cb, backup]() {
        cb->setMimeData(backup);
    });
}

