// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef X11PLATFORMWINDOW_H
#define X11PLATFORMWINDOW_H

#include <QGuiApplication>
#include <X11/Xlib.h>

class AppConfig;
class QWidget;

class X11PlatformWindow
{
public:

    explicit X11PlatformWindow(Window winId);

    void raise() ;

    void pasteClipboard() ;

    bool isValid() const;

private:
    bool waitForFocus(int ms);

    void sendKeyPress(int modifier, int key);

    Window m_window;

    bool isPlatformX11;

    QNativeInterface::QX11Application *x11Application;

};

Window getCurrentWindow();

#endif // X11PLATFORMWINDOW_H
