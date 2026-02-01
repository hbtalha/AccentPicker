// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QSettings>

class AppConfig : public QObject
{
    Q_OBJECT
public:
    enum Key {
        Active,
        StartHidden,
        AutoStart
    };
    Q_ENUM(Key)

    explicit AppConfig(QObject* parent = nullptr): QObject(parent) {}

    QVariant value(Key key, const QVariant &defaultValue) const;
    QVariant value(Key key) const;
    void setValue(Key key, const QVariant &value);

signals:
    void valueChanged(Key key, const QVariant &value);

private:
    QSettings settings;

    static QString keyToString(Key key);
};

#endif // APPCONFIG_H
