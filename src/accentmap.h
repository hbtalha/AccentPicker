// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ACCENTMAP_H
#define ACCENTMAP_H

#include <QMap>
#include <QStringList>

class AccentMap {
public:
    static QStringList getAccents(QChar baseChar);
    
private:
    static QMap<QChar, QStringList> createAccentMap();
    static const QMap<QChar, QStringList> accentMap;
};

#endif // ACCENTMAP_H