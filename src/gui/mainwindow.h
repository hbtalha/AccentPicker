// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QCheckBox;
class KeyMonitor;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(KeyMonitor* monitor, QWidget* parent = nullptr);

protected slots:
    void onAutoStartToggled(bool value);
    void onMonitorToggled(bool active);
    void onConfigButtonClicked(bool);
protected:
    void closeEvent(QCloseEvent* event) override;

private:
    KeyMonitor* monitor;
    QPushButton* langSetConfigButton;

    void setupWindow();
    void setupTrayIcon();
    void toggleVisible();
};

#endif //MAINWINDOW_H
