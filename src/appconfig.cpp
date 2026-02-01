// SPDX-License-Identifier: GPL-3.0-or-later

#include "appconfig.h"

#include <QSettings>

QString AppConfig::keyToString(Key key)
{
    switch (key) {
    case Key::Active:
        return "active";
    case Key::StartHidden:
        return "startHidden";
    case Key::AutoStart:
        return "autoStart";
    }
    Q_UNREACHABLE();
}


QVariant AppConfig::value(Key key, const QVariant &defaultValue) const
{
    return settings.value(keyToString(key), defaultValue);
}

QVariant AppConfig::value(Key key) const
{
    return settings.value(keyToString(key));
}

void AppConfig::setValue(Key key, const QVariant &value)
{
    settings.setValue(keyToString(key), value);
    emit valueChanged(key, value);
}
