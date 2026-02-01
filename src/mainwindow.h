// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "appconfig.h"

#include <QMainWindow>

class QCheckBox;
class KeyMonitor;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(KeyMonitor* monitor, QWidget* parent = nullptr);

protected slots:
    void onAutoStartToggled(bool value);
    void onMonitorToggled(bool active);
protected:
    void closeEvent(QCloseEvent* event) override;

private:
    KeyMonitor* monitor;
    QCheckBox* activateCheck;
    QCheckBox* startHiddenCheck;
    QCheckBox* autostartCheck;

    AppConfig config;

    void setupWindow();
    void setupTrayIcon();
    void toggleVisible();
};

#endif //MAINWINDOW_H
