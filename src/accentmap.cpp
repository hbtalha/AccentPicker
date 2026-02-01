// SPDX-License-Identifier: GPL-3.0-or-later

#include "accentmap.h"

QMap<QChar, QStringList> AccentMap::createAccentMap()
{
    QMap<QChar, QStringList> map;

    // Lowercase letters
    map['a'] = {"á", "à", "â", "ä", "ã", "å", "ā", "ą", "ă"};
    map['e'] = {"é", "è", "ê", "ë", "ē", "ė", "ę", "ě"};
    map['i'] = {"í", "ì", "î", "ï", "ī", "į", "ı"};
    map['o'] = {"ó", "ò", "ô", "ö", "õ", "ō", "ø", "ő"};
    map['u'] = {"ú", "ù", "û", "ü", "ū", "ų", "ű", "ů"};
    map['y'] = {"ý", "ÿ", "ȳ"};
    map['c'] = {"ć", "č", "ç", "ĉ", "ċ"};
    map['n'] = {"ñ", "ń", "ň", "ņ"};
    map['s'] = {"š", "ś", "ş", "ș", "ŝ"};
    map['z'] = {"ž", "ź", "ż"};
    map['l'] = {"ł", "ĺ", "ľ", "ļ"};
    map['d'] = {"đ", "ď"};
    map['g'] = {"ğ", "ģ", "ġ", "ĝ"};
    map['r'] = {"ř", "ŕ", "ŗ"};
    map['t'] = {"ť", "ţ", "ț"};

    // Uppercase letters
    map['A'] = {"Á", "À", "Â", "Ä", "Ã", "Å", "Ā", "Ą", "Ă"};
    map['E'] = {"É", "È", "Ê", "Ë", "Ē", "Ė", "Ę", "Ě"};
    map['I'] = {"Í", "Ì", "Î", "Ï", "Ī", "Į", "İ"};
    map['O'] = {"Ó", "Ò", "Ô", "Ö", "Õ", "Ō", "Ø", "Ő"};
    map['U'] = {"Ú", "Ù", "Û", "Ü", "Ū", "Ų", "Ű", "Ů"};
    map['Y'] = {"Ý", "Ÿ", "Ȳ"};
    map['C'] = {"Ć", "Č", "Ç", "Ĉ", "Ċ"};
    map['N'] = {"Ñ", "Ń", "Ň", "Ņ"};
    map['S'] = {"Š", "Ś", "Ş", "Ș", "Ŝ"};
    map['Z'] = {"Ž", "Ź", "Ż"};
    map['L'] = {"Ł", "Ĺ", "Ľ", "Ļ"};
    map['D'] = {"Đ", "Ď"};
    map['G'] = {"Ğ", "Ģ", "Ġ", "Ĝ"};
    map['R'] = {"Ř", "Ŕ", "Ŗ"};
    map['T'] = {"Ť", "Ţ", "Ț"};

    return map;
}

const QMap<QChar, QStringList> AccentMap::accentMap = AccentMap::createAccentMap();

QStringList AccentMap::getAccents(QChar baseChar)
{
    if (accentMap.contains(baseChar)) {
        return accentMap[baseChar];
    }
    return QStringList();
}
