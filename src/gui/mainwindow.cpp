// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QDir>
#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QPushButton>

#include "charactersetdialog.h"
#include "core/keymonitor.h"
#include "config/appconfig.h"
#include "config/configkeys.h"

MainWindow::MainWindow(KeyMonitor* monitor, QWidget* parent)
    : QMainWindow(parent), monitor(monitor)
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    auto activateCheck   = new QCheckBox("Activate "+ qApp->applicationDisplayName(), this);
    auto startHiddenCheck = new QCheckBox("Start hidden", this);
    auto autostartCheck   = new QCheckBox("Start with system", this);

    langSetConfigButton = new QPushButton("Config lang sets");

    layout->addWidget(activateCheck);
    layout->addWidget(startHiddenCheck);
    layout->addWidget(autostartCheck);
    layout->addWidget(langSetConfigButton);
    setCentralWidget(central);

    activateCheck->setChecked(appConfig->get<ConfigKey::Active>());
    startHiddenCheck->setChecked(appConfig->get<ConfigKey::StartHidden>());
    autostartCheck->setChecked(appConfig->get<ConfigKey::AutoStart>());

    connect(startHiddenCheck, &QCheckBox::toggled,
    [](bool v) {
        appConfig->set<ConfigKey::StartHidden>(v);
    });
    connect(activateCheck, &QCheckBox::toggled, this, &MainWindow::onMonitorToggled);
    connect(autostartCheck, &QCheckBox::toggled, this, &MainWindow::onAutoStartToggled);
    connect(langSetConfigButton, &QPushButton::clicked,this, &MainWindow::onConfigButtonClicked);

    setupTrayIcon();
    setupWindow();
}

void MainWindow::onAutoStartToggled(bool value)
{
    QString path = QDir::homePath() + "/.config/autostart/";
    QString appName = "accentpicker";
    QString name = appName +".desktop";
    QFile file(path+name);

    file.remove();

    if(value) {
        QDir dir(path);
        if(!dir.exists()) {
            dir.mkpath(path);
        }

        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "[Desktop Entry]" << Qt::endl;
            stream << "Name=" + appName<< Qt::endl;
            stream << "Icon=" + appName << Qt::endl;
            stream << "Type=Application" << Qt::endl;
            stream << "Exec=" << QCoreApplication::applicationFilePath() << Qt::endl;
        }

        appConfig->set<ConfigKey::AutoStart>(true);
    } else {
        appConfig->set<ConfigKey::AutoStart>(false);
    }
}

void MainWindow::onMonitorToggled(bool active)
{
    if(active) {
        monitor->start();
    } else {
        monitor->stop();
    }
    appConfig->set<ConfigKey::Active>(active);
}

void MainWindow::onConfigButtonClicked(bool)
{
    CharacterSetDialog dialog(this);
    dialog.exec();
}


void MainWindow::setupWindow()
{
    setWindowIcon(QIcon(":/icons/accentpicker.png"));
    setFixedSize(219, 120);
    move(QGuiApplication::primaryScreen()->availableGeometry().center() - rect().center());
}

void MainWindow::toggleVisible()
{
    if (!isMinimized() && isVisible()) {
        hide();
    } else {
        show();
    }
}

void MainWindow::setupTrayIcon()
{
    QSystemTrayIcon* tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/icons/accentpicker.png"));

    QMenu* trayMenu = new QMenu(this);
    QAction* showAction = trayMenu->addAction("Show");
    QAction* activateAction = trayMenu->addAction("Activate");
    activateAction->setCheckable(true);
    activateAction->setChecked(appConfig->get<ConfigKey::Active>());
    trayMenu->addSeparator();
    QAction* quitAction = trayMenu->addAction("Quit");

    tray->setContextMenu(trayMenu);
    tray->show();

    connect( tray, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason) {
        if(reason ==QSystemTrayIcon::Trigger ) {
            toggleVisible();
        }
    });

    connect(showAction, &QAction::triggered, this, &QWidget::show);
    connect(activateAction, &QAction::toggled, this, &MainWindow::onMonitorToggled);
    connect(quitAction, &QAction::triggered, []() {
        qApp->quit();
    });
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    hide();
}

