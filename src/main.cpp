#include <QSharedMemory>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>

#include "accentpicker.h"
#include "appconfig.h"
#include "keymonitor.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    static const char* sharedKey = "AccentPickerSingletonKey";

    QSharedMemory sharedMemory(sharedKey);

    if(!sharedMemory.create(1)) {
        QMessageBox::warning(nullptr, "Accent Picker",
                             "Another instance is already running.");
        return 1;
    }

    QCoreApplication::setOrganizationName("HBatalha");
    QCoreApplication::setApplicationName("Accent Picker");

    AccentPicker picker;
    KeyMonitor monitor;

    QObject::connect(&monitor, &KeyMonitor::keyEvent,
                     &picker, &AccentPicker::keyEvent);

    QObject::connect(&picker, &AccentPicker::visibleChanged,
                     &monitor, &KeyMonitor::accentPickerVisible);

    QObject::connect(&picker, &AccentPicker::accentSelected,
                     &monitor, &KeyMonitor::insertText);

    MainWindow window(&monitor);

    if (!monitor.start()) {
        QMessageBox::critical(nullptr, "Error",
                              "Failed to start keyboard monitoring.");
        return 1;
    }

    AppConfig config;
    if (!config.value(AppConfig::StartHidden, false).toBool())
        window.show();

    return app.exec();
}

