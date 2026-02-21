// SPDX-License-Identifier: GPL-3.0-or-later

#include "sleeptimer.h"
#include "x11platformwindow.h"

#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <QTimer>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>


void waitMs(int msec)
{
    if (msec <= 0)
        return;

    QEventLoop loop;
    QTimer t;
    QObject::connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(msec);
    loop.exec();
}

class KeyPressTester final
{
public:
    explicit KeyPressTester(Display *display)
        : m_display(display)
    {
        XQueryKeymap(m_display, m_keyMap);
    }

    bool isPressed(KeySym key) const
    {
        const KeyCode keyCode = XKeysymToKeycode(m_display, key);
        return (m_keyMap[keyCode >> 3] >> (keyCode & 7)) & 1;
    }

private:
    Display *m_display;
    char m_keyMap[32] {};
};


void fakeKeyEvent(Display* display, unsigned int keyCode, Bool isPress, unsigned long delayMs = CurrentTime)
{
    XTestFakeKeyEvent(display, keyCode, isPress, delayMs);
    XSync(display, False);
}

void simulateModifierKeyPress(Display *display, const QList<int> &modCodes, Bool keyDown)
{
    for (int modCode : modCodes) {
        const auto keySym = static_cast<KeySym>(modCode);
        KeyCode keyCode = XKeysymToKeycode(display, keySym);
        fakeKeyEvent(display, keyCode, keyDown);
    }
}

bool isModifierPressed(Display *display)
{
    KeyPressTester tester(display);
    return tester.isPressed(XK_Shift_L)
           || tester.isPressed(XK_Shift_R)
           || tester.isPressed(XK_Control_L)
           || tester.isPressed(XK_Control_R)
           || tester.isPressed(XK_Meta_L)
           || tester.isPressed(XK_Meta_R)
           || tester.isPressed(XK_Alt_L)
           || tester.isPressed(XK_Alt_R)
           || tester.isPressed(XK_Super_L)
           || tester.isPressed(XK_Super_R)
           || tester.isPressed(XK_Hyper_L)
           || tester.isPressed(XK_Hyper_R);
}

bool waitForModifiersReleased(Display *display)
{
    const int maxWaitForModsReleaseMs = 2000;
    if (maxWaitForModsReleaseMs >= 0) {
        SleepTimer t(maxWaitForModsReleaseMs);
        while (t.sleep()) {
            if (!isModifierPressed(display))
                return true;
        }
    }

    return !isModifierPressed(display);
}

void simulateKeyPress(Display *display, const QList<int> &modCodes, unsigned int key)
{
    // Wait for user to release modifiers.
    if (!waitForModifiersReleased(display))
        return;

    simulateModifierKeyPress(display, modCodes, True);

    const KeyCode keyCode = XKeysymToKeycode(display, key);

    fakeKeyEvent(display, keyCode, True);
    // This is needed to paste into URL bar in Chrome.
    const unsigned long delayMs = 50;
    fakeKeyEvent(display, keyCode, False, delayMs);

    simulateModifierKeyPress(display, modCodes, False);

    XSync(display, False);
}

class X11WindowProperty final
{
public:
    X11WindowProperty(Display *display, Window w, Atom property, long longOffset,
                      long longLength, Atom reqType)
    {
        if ( XGetWindowProperty(display, w, property, longOffset, longLength, false,
                                reqType, &type, &format, &len, &remain, &data) != Success ) {
            data = nullptr;
        }
    }

    ~X11WindowProperty()
    {
        if (data != nullptr)
            XFree(data);
    }

    bool isValid() const
    {
        return data != nullptr;
    }

    X11WindowProperty(const X11WindowProperty &) = delete;
    X11WindowProperty &operator=(const X11WindowProperty &) = delete;

    Atom type{};
    int format{};
    unsigned long len{};
    unsigned long remain{};
    unsigned char *data;
};

Window getCurrentWindow()
{
    auto*x11Application = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();

    if (!x11Application)
        return 0L;

    auto display = x11Application->display();
    XSync(display, False);

    static Atom atomWindow = XInternAtom(display, "_NET_ACTIVE_WINDOW", true);

    X11WindowProperty property(display, DefaultRootWindow(display), atomWindow, 0l, 1l, XA_WINDOW);

    if ( property.isValid() && property.type == XA_WINDOW && property.format == 32 && property.len == 1)
        return *reinterpret_cast<Window *>(property.data);

    return 0L;
}

X11PlatformWindow::X11PlatformWindow(Window winId)
    : m_window(winId)
{
    x11Application = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
}


void X11PlatformWindow::raise()
{
    Q_ASSERT( isValid() );

    if (!x11Application)
        return;


    auto display = x11Application->display();

    if (!display)
        return;

    XEvent e{};
    memset(&e, 0, sizeof(e));
    e.type = ClientMessage;
    e.xclient.display = display;
    e.xclient.window = m_window;
    e.xclient.message_type = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    e.xclient.format = 32;
    e.xclient.data.l[0] = 2;
    e.xclient.data.l[1] = CurrentTime;
    e.xclient.data.l[2] = 0;
    e.xclient.data.l[3] = 0;
    e.xclient.data.l[4] = 0;

    XWindowAttributes wattr{};
    XGetWindowAttributes(display, m_window, &wattr);

    if (wattr.map_state == IsViewable) {
        XSendEvent(display, wattr.screen->root, False,
                   SubstructureNotifyMask | SubstructureRedirectMask,
                   &e);
        XSync(display, False);
        XRaiseWindow(display, m_window);
        XSetInputFocus(display, m_window, RevertToPointerRoot, CurrentTime);
        XSync(display, False);
    }
}

void X11PlatformWindow::pasteClipboard()
{
    sendKeyPress(XK_Shift_L, XK_Insert);
}

bool X11PlatformWindow::isValid() const
{
    return m_window != 0L;
}

bool X11PlatformWindow::waitForFocus(int ms)
{
    Q_ASSERT( isValid() );

    if (ms >= 0) {
        SleepTimer t(ms);
        while (t.sleep()) {
            const auto currentWindow = getCurrentWindow();
            if (currentWindow == m_window)
                return true;
        }
    }

    return m_window == getCurrentWindow();
}

void X11PlatformWindow::sendKeyPress(int modifier, int key)
{
    Q_ASSERT( isValid() );

    if ( !waitForFocus(20) ) {
        raise();
        if ( !waitForFocus(150) ) {
            return;
        }
    }

    waitMs(50);

    if (!x11Application)
        return;

    auto display = x11Application->display();

    if (!display)
        return;

    simulateKeyPress(display, QList<int>() << modifier, static_cast<uint>(key));
}
