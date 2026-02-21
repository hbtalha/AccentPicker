#include <QSharedMemory>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>

#include "gui/accentpicker.h"
#include "config/appconfig.h"
#include "config/configkeys.h"
#include "core/keymonitor.h"
#include "gui/mainwindow.h"
#include "core/singleinstance.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    SingleInstance instance("AccentPickerSingletonKey");

    if(!instance.run()) {
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

    MainWindow mainWindow(&monitor);

    if (appConfig->get<ConfigKey::Active>()) {
        if (!monitor.start()) {
            QMessageBox::critical(nullptr, "Error",
                                  "Failed to start keyboard monitoring.");
            return 1;
        }
    }

    if (!appConfig->get<ConfigKey::StartHidden>()) {
        mainWindow.show();
    }

    return app.exec();
}

