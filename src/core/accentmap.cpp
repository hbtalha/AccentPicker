#include "core/accentmap.h"
#include "config/appconfig.h"
#include "config/configkeys.h"
#include <QSet>

QMap<QChar, QStringList> AccentMap::allLanguagesCache;


QStringList AccentMap::getAccents(QChar baseChar, const QStringList &langCodes)
{
    if (appConfig->get<ConfigKey::SelectedAllCharacterSets>()) {
        if (allLanguagesCache.contains(baseChar)) {
            return allLanguagesCache[baseChar];
        }

        QSet<QString> allAccents;

        for (int i = static_cast<int>(Language::ALL);
                i <= static_cast<int>(Language::VI); ++i) {
            Language lang = static_cast<Language>(i);

            const QStringList accents = getAccentsForLanguage(baseChar, lang);

            allAccents.unite(QSet<QString>(accents.begin(), accents.end()));
        }

        QStringList result = allAccents.values();
        allLanguagesCache[baseChar] = result;
        return result;
    }

    QStringList combinedAccents;
    QSet<QString> seen;
    for (auto const& code : langCodes) {
        QStringList langAccents = getAccentsForLanguage(baseChar, code);

        for (const QString &accent : std::as_const(langAccents) ) {
            // O(1) duplicate check
            if (!seen.contains(accent)) {
                combinedAccents.push_back(accent);
                seen.insert(accent);// preserves insertion order
            }
        }
    }

    return combinedAccents;
}

QList<LanguageInfo> AccentMap::getAllLanguages()
{
    return {
        {Language::ALL, "ALL", "All Languages"},
        {Language::BG, "BG", "Bulgarian"},
        {Language::CA, "CA", "Catalan"},
        {Language::CRH, "CRH", "Crimean Tatar"},
        {Language::CUR, "CUR", "Currency"},
        {Language::CY, "CY", "Welsh"},
        {Language::CZ, "CZ", "Czech"},
        {Language::DK, "DK", "Danish"},
        {Language::EPO, "EPO", "Esperanto"},
        {Language::EST, "EST", "Estonian"},
        {Language::FI, "FI", "Finnish"},
        {Language::FR, "FR", "French"},
        {Language::GA, "GA", "Gaeilge (Irish)"},
        {Language::GD, "GD", "Gàidhlig (Scottish)"},
        {Language::DE, "DE", "German"},
        {Language::EL, "EL", "Greek"},
        {Language::HE, "HE", "Hebrew"},
        {Language::HR, "HR", "Croatian"},
        {Language::HU, "HU", "Hungarian"},
        {Language::IS, "IS", "Icelandic"},
        {Language::IPA, "IPA", "IPA"},
        {Language::IT, "IT", "Italian"},
        {Language::KU, "KU", "Kurdish"},
        {Language::LT, "LT", "Lithuanian"},
        {Language::MI, "MI", "Maori"},
        {Language::MK, "MK", "Macedonian"},
        {Language::MT, "MT", "Maltese"},
        {Language::NL, "NL", "Dutch"},
        {Language::NO, "NO", "Norwegian"},
        {Language::PI, "PI", "Pinyin"},
        {Language::PIE, "PIE", "Proto-Indo-European"},
        {Language::PL, "PL", "Polish"},
        {Language::PT, "PT", "Portuguese"},
        {Language::RO, "RO", "Romanian"},
        {Language::ROM, "ROM", "Romanization (ME)"},
        {Language::SK, "SK", "Slovak"},
        {Language::SL, "SL", "Slovenian"},
        {Language::SP, "SP", "Spanish"},
        {Language::SR, "SR", "Serbian"},
        {Language::SR_CYRL, "SR_CYRL", "Serbian (Cyrillic)"},
        {Language::SV, "SV", "Swedish"},
        {Language::TK, "TK", "Turkish"},
        {Language::VI, "VI", "Vietnamese"}
    };
}

QStringList AccentMap::getAccentsForLanguage(QChar baseChar, const QString &langCode)
{
    static const QHash<QString, std::function<QStringList(QChar)>> langMap = {
        {
            "BG", [](QChar c)
            {
                return getAccentsBG(c);
            }
        },
        {
            "CA", [](QChar c)
            {
                return getAccentsCA(c);
            }
        },
        {
            "CRH", [](QChar c)
            {
                return getAccentsCRH(c);
            }
        },
        {
            "CUR", [](QChar c)
            {
                return getAccentsCUR(c);
            }
        },
        {
            "CY", [](QChar c)
            {
                return getAccentsCY(c);
            }
        },
        {
            "CZ", [](QChar c)
            {
                return getAccentsCZ(c);
            }
        },
        {
            "DK", [](QChar c)
            {
                return getAccentsDK(c);
            }
        },
        {
            "GA", [](QChar c)
            {
                return getAccentsGA(c);
            }
        },
        {
            "GD", [](QChar c)
            {
                return getAccentsGD(c);
            }
        },
        {
            "DE", [](QChar c)
            {
                return getAccentsDE(c);
            }
        },
        {
            "EL", [](QChar c)
            {
                return getAccentsEL(c);
            }
        },
        {
            "EST", [](QChar c)
            {
                return getAccentsEST(c);
            }
        },
        {
            "EPO", [](QChar c)
            {
                return getAccentsEPO(c);
            }
        },
        {
            "FI", [](QChar c)
            {
                return getAccentsFI(c);
            }
        },
        {
            "FR", [](QChar c)
            {
                return getAccentsFR(c);
            }
        },
        {
            "HR", [](QChar c)
            {
                return getAccentsHR(c);
            }
        },
        {
            "HE", [](QChar c)
            {
                return getAccentsHE(c);
            }
        },
        {
            "HU", [](QChar c)
            {
                return getAccentsHU(c);
            }
        },
        {
            "IS", [](QChar c)
            {
                return getAccentsIS(c);
            }
        },
        {
            "IPA", [](QChar c)
            {
                return getAccentsIPA(c);
            }
        },
        {
            "IT", [](QChar c)
            {
                return getAccentsIT(c);
            }
        },
        {
            "KU", [](QChar c)
            {
                return getAccentsKU(c);
            }
        },
        {
            "LT", [](QChar c)
            {
                return getAccentsLT(c);
            }
        },
        {
            "MK", [](QChar c)
            {
                return getAccentsMK(c);
            }
        },
        {
            "MT", [](QChar c)
            {
                return getAccentsMT(c);
            }
        },
        {
            "MI", [](QChar c)
            {
                return getAccentsMI(c);
            }
        },
        {
            "NL", [](QChar c)
            {
                return getAccentsNL(c);
            }
        },
        {
            "NO", [](QChar c)
            {
                return getAccentsNO(c);
            }
        },
        {
            "PI", [](QChar c)
            {
                return getAccentsPI(c);
            }
        },
        {
            "PIE", [](QChar c)
            {
                return getAccentsPIE(c);
            }
        },
        {
            "PL", [](QChar c)
            {
                return getAccentsPL(c);
            }
        },
        {
            "PT", [](QChar c)
            {
                return getAccentsPT(c);
            }
        },
        {
            "RO", [](QChar c)
            {
                return getAccentsRO(c);
            }
        },
        {
            "ROM", [](QChar c)
            {
                return getAccentsROM(c);
            }
        },
        {
            "SK", [](QChar c)
            {
                return getAccentsSK(c);
            }
        },
        {
            "SL", [](QChar c)
            {
                return getAccentsSL(c);
            }
        },
        {
            "SP", [](QChar c)
            {
                return getAccentsSP(c);
            }
        },
        {
            "SR", [](QChar c)
            {
                return getAccentsSR(c);
            }
        },
        {
            "SR_CYRL", [](QChar c)
            {
                return getAccentsSRCyrillic(c);
            }
        },
        {
            "SV", [](QChar c)
            {
                return getAccentsSV(c);
            }
        },
        {
            "TK", [](QChar c)
            {
                return getAccentsTK(c);
            }
        },
        {
            "VI", [](QChar c)
            {
                return getAccentsVI(c);
            }
        }
    };

    auto it = langMap.find(langCode.toUpper());
    if (it != langMap.end()) {
        return it.value()(baseChar);
    }

    return QStringList();
}

QStringList AccentMap::getAccentsForLanguage(QChar baseChar, Language lang)
{
    switch (lang) {
    case Language::BG:
        return getAccentsBG(baseChar);
    case Language::CA:
        return getAccentsCA(baseChar);
    case Language::CRH:
        return getAccentsCRH(baseChar);
    case Language::CUR:
        return getAccentsCUR(baseChar);
    case Language::CY:
        return getAccentsCY(baseChar);
    case Language::CZ:
        return getAccentsCZ(baseChar);
    case Language::DK:
        return getAccentsDK(baseChar);
    case Language::GA:
        return getAccentsGA(baseChar);
    case Language::GD:
        return getAccentsGD(baseChar);
    case Language::DE:
        return getAccentsDE(baseChar);
    case Language::EL:
        return getAccentsEL(baseChar);
    case Language::EST:
        return getAccentsEST(baseChar);
    case Language::EPO:
        return getAccentsEPO(baseChar);
    case Language::FI:
        return getAccentsFI(baseChar);
    case Language::FR:
        return getAccentsFR(baseChar);
    case Language::HR:
        return getAccentsHR(baseChar);
    case Language::HE:
        return getAccentsHE(baseChar);
    case Language::HU:
        return getAccentsHU(baseChar);
    case Language::IS:
        return getAccentsIS(baseChar);
    case Language::IPA:
        return getAccentsIPA(baseChar);
    case Language::IT:
        return getAccentsIT(baseChar);
    case Language::KU:
        return getAccentsKU(baseChar);
    case Language::LT:
        return getAccentsLT(baseChar);
    case Language::MK:
        return getAccentsMK(baseChar);
    case Language::MT:
        return getAccentsMT(baseChar);
    case Language::MI:
        return getAccentsMI(baseChar);
    case Language::NL:
        return getAccentsNL(baseChar);
    case Language::NO:
        return getAccentsNO(baseChar);
    case Language::PI:
        return getAccentsPI(baseChar);
    case Language::PIE:
        return getAccentsPIE(baseChar);
    case Language::PL:
        return getAccentsPL(baseChar);
    case Language::PT:
        return getAccentsPT(baseChar);
    case Language::RO:
        return getAccentsRO(baseChar);
    case Language::ROM:
        return getAccentsROM(baseChar);
    case Language::SK:
        return getAccentsSK(baseChar);
    case Language::SL:
        return getAccentsSL(baseChar);
    case Language::SP:
        return getAccentsSP(baseChar);
    case Language::SR:
        return getAccentsSR(baseChar);
    case Language::SR_CYRL:
        return getAccentsSRCyrillic(baseChar);
    case Language::SV:
        return getAccentsSV(baseChar);
    case Language::TK:
        return getAccentsTK(baseChar);
    case Language::VI:
        return getAccentsVI(baseChar);
    default:
        return QStringList();
    }
}

QStringList AccentMap::getAccentsFR(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"à", "â", "á", "ä", "ã", "æ"};
    if (lower == 'c') return {"ç"};
    if (lower == 'e') return {"é", "è", "ê", "ë", "€"};
    if (lower == 'i') return {"î", "ï", "í", "ì"};
    if (lower == 'o') return {"ô", "ö", "ó", "ò", "õ", "œ"};
    if (lower == 'u') return {"û", "ù", "ü", "ú"};
    if (lower == 'y') return {"ÿ", "ý"};
    if (c == ',') return {"«", "»", "‹", "›", "\"", "\"", "'", "'"};
    return QStringList();
}

// Spanish
QStringList AccentMap::getAccentsSP(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á"};
    if (lower == 'e') return {"é", "€"};
    if (lower == 'h') return {"ḥ"};
    if (lower == 'i') return {"í"};
    if (lower == 'l') return {"ḷ"};
    if (lower == 'n') return {"ñ"};
    if (lower == 'o') return {"ó"};
    if (lower == 'u') return {"ú", "ü"};
    if (c == ',') return {"¿", "?", "¡", "!", "«", "»", "\"", "\"", "'", "'"};
    return QStringList();
}

// German
QStringList AccentMap::getAccentsDE(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ä"};
    if (lower == 'e') return {"€"};
    if (lower == 'o') return {"ö"};
    if (lower == 's') return {"ß"};
    if (lower == 'u') return {"ü"};
    if (c == ',') return {"„", "\"", ".", "'", "»", "«", "›", "‹"};
    return QStringList();
}

// Portuguese
QStringList AccentMap::getAccentsPT(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á", "à", "â", "ã", "ª"};
    if (lower == 'c') return {"ç"};
    if (lower == 'e') return {"é", "ê", "€"};
    if (lower == 'i') return {"í"};
    if (lower == 'o') return {"ô", "ó", "õ", "º"};
    if (lower == 's') return {"$"};
    if (lower == 'u') return {"ú"};
    if (c == ',') return {"\"", "\"", "'", "'", "«", "»"};
    return QStringList();
}

QStringList AccentMap::getAccentsIT(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"à"};
    if (lower == 'e') return {"è", "é", "ə", "€"};
    if (lower == 'i') return {"ì", "í"};
    if (lower == 'o') return {"ò", "ó"};
    if (lower == 'u') return {"ù", "ú"};
    if (c == ',') return {"«", "»", "\"", "\"", "'", "'"};
    return QStringList();
}

QStringList AccentMap::getAccentsPL(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ą"};
    if (lower == 'c') return {"ć"};
    if (lower == 'e') return {"ę", "€"};
    if (lower == 'l') return {"ł"};
    if (lower == 'n') return {"ń"};
    if (lower == 'o') return {"ó"};
    if (lower == 's') return {"ś"};
    if (lower == 'z') return {"ż", "ź"};
    if (c == ',') return {"„", "\"", "'", "'", "»", "«"};
    return QStringList();
}

QStringList AccentMap::getAccentsTK(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"â"};
    if (lower == 'c') return {"ç"};
    if (lower == 'e') return {"ë", "€"};
    if (lower == 'g') return {"ğ"};
    if (lower == 'i') return {"ı", "İ", "î"};
    if (lower == 'o') return {"ö", "ô"};
    if (lower == 's') return {"ş"};
    if (lower == 't') return {"₺"};
    if (lower == 'u') return {"ü", "û"};
    if (c == ',') return {"\"", "\"", "'", "'", "«", "»", "‹", "›"};
    return QStringList();
}

QStringList AccentMap::getAccentsBG(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'i') return {"й"};
    return QStringList();
}
QStringList AccentMap::getAccentsCA(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"à", "á"};
    if (lower == 'c') return {"ç"};
    if (lower == 'e') return {"è", "é", "€"};
    if (lower == 'i') return {"ì", "í", "ï"};
    if (lower == 'n') return {"ñ"};
    if (lower == 'o') return {"ò", "ó"};
    if (lower == 'u') return {"ù", "ú", "ü"};
    if (lower == 'l') return {"·"};
    if (c == ',') return {"¿", "?", "¡", "!", "«", "»", "\"", "\"", "'", "'"};
    return QStringList();
}
QStringList AccentMap::getAccentsCRH(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"â"};
    if (lower == 'c') return {"ç"};
    if (lower == 'e') return {"€"};
    if (lower == 'g') return {"ğ"};
    if (lower == 'h') return {"₴"};
    if (lower == 'i') return {"ı", "İ"};
    if (lower == 'n') return {"ñ"};
    if (lower == 'o') return {"ö"};
    if (lower == 's') return {"ş"};
    if (lower == 't') return {"₺"};
    if (lower == 'u') return {"ü"};
    return QStringList();
}
QStringList AccentMap::getAccentsCUR(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'b') return {"฿", "в"};
    if (lower == 'c') return {"¢", "₡", "č"};
    if (lower == 'd') return {"₫"};
    if (lower == 'e') return {"€"};
    if (lower == 'f') return {"ƒ"};
    if (lower == 'h') return {"₴"};
    if (lower == 'k') return {"₭"};
    if (lower == 'l') return {"ł"};
    if (lower == 'n') return {"л"};
    if (lower == 'm') return {"₼"};
    if (lower == 'p') return {"£", "₽"};
    if (lower == 'r') return {"₹", "៛", "﷼"};
    if (lower == 's') return {"$", "₪"};
    if (lower == 't') return {"₮", "₺", "₸"};
    if (lower == 'w') return {"₩"};
    if (lower == 'y') return {"¥"};
    if (lower == 'z') return {"z"};
    return QStringList();
}
QStringList AccentMap::getAccentsCY(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"â", "ä", "à", "á"};
    if (lower == 'e') return {"ê", "ë", "è", "é"};
    if (lower == 'i') return {"î", "ï", "ì", "í"};
    if (lower == 'o') return {"ô", "ö", "ò", "ó"};
    if (lower == 'p') return {"£"};
    if (lower == 'u') return {"û", "ü", "ù", "ú"};
    if (lower == 'y') return {"ŷ", "ÿ", "ỳ", "ý"};
    if (lower == 'w') return {"ŵ", "ẅ", "ẁ", "ẃ"};
    if (c == ',') return {"'", "'", "\"", "\""};
    return QStringList();
}
QStringList AccentMap::getAccentsCZ(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á"};
    if (lower == 'c') return {"č"};
    if (lower == 'd') return {"ď"};
    if (lower == 'e') return {"ě", "é"};
    if (lower == 'i') return {"í"};
    if (lower == 'n') return {"ň"};
    if (lower == 'o') return {"ó"};
    if (lower == 'r') return {"ř"};
    if (lower == 's') return {"š"};
    if (lower == 't') return {"ť"};
    if (lower == 'u') return {"ů", "ú"};
    if (lower == 'y') return {"ý"};
    if (lower == 'z') return {"ž"};
    if (c == ',') return {"„", "\"", "‚", "'", "»", "«", "›", "‹"};
    return QStringList();
}
QStringList AccentMap::getAccentsDK(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"å", "æ"};
    if (lower == 'e') return {"€"};
    if (lower == 'o') return {"ø"};
    if (c == ',') return {"»", "«", "\"", "\"", "›", "‹", "'", "'"};
    return QStringList();
}
QStringList AccentMap::getAccentsGA(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á"};
    if (lower == 'e') return {"é", "€"};
    if (lower == 'i') return {"í"};
    if (lower == 'o') return {"ó"};
    if (lower == 'u') return {"ú"};
    if (c == ',') return {"\"", "\"", "'", "'"};
    return QStringList();
}
QStringList AccentMap::getAccentsGD(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"à"};
    if (lower == 'e') return {"è"};
    if (lower == 'i') return {"ì"};
    if (lower == 'o') return {"ò"};
    if (lower == 'p') return {"£"};
    if (lower == 'u') return {"ù"};
    if (c == ',') return {"\"", "\"", "'", "'"};
    return QStringList();
}
QStringList AccentMap::getAccentsEL(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"α", "ά"};
    if (lower == 'b') return {"β"};
    if (lower == 'c') return {"χ"};
    if (lower == 'd') return {"δ"};
    if (lower == 'e') return {"ε", "έ", "η", "ή"};
    if (lower == 'f') return {"φ"};
    if (lower == 'g') return {"γ"};
    if (lower == 'i') return {"ι", "ί"};
    if (lower == 'k') return {"κ"};
    if (lower == 'l') return {"λ"};
    if (lower == 'm') return {"μ"};
    if (lower == 'n') return {"ν"};
    if (lower == 'o') return {"ο", "ό", "ω", "ώ"};
    if (lower == 'p') return {"π", "φ", "ψ"};
    if (lower == 'r') return {"ρ"};
    if (lower == 's') return {"σ", "ς"};
    if (lower == 't') return {"τ", "θ", "ϑ"};
    if (lower == 'u') return {"υ", "ύ"};
    if (lower == 'x') return {"ξ"};
    if (lower == 'y') return {"υ"};
    if (lower == 'z') return {"ζ"};
    if (c == ',') return {"\"", "\"", "«", "»"};
    return QStringList();
}
QStringList AccentMap::getAccentsEST(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ä"};
    if (lower == 'e') return {"€"};
    if (lower == 'o') return {"ö", "õ"};
    if (lower == 'u') return {"ü"};
    if (lower == 'z') return {"ž"};
    if (lower == 's') return {"š"};
    if (c == ',') return {"„", "\"", "«", "»"};
    return QStringList();
}
QStringList AccentMap::getAccentsEPO(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'c') return {"ĉ"};
    if (lower == 'g') return {"ĝ"};
    if (lower == 'h') return {"ĥ"};
    if (lower == 'j') return {"ĵ"};
    if (lower == 's') return {"ŝ"};
    if (lower == 'u') return {"ŭ"};
    return QStringList();
}
QStringList AccentMap::getAccentsFI(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ä", "å"};
    if (lower == 'e') return {"€"};
    if (lower == 'o') return {"ö"};
    if (c == ',') return {"\"", "'", "»"};
    return QStringList();
}
QStringList AccentMap::getAccentsHR(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'c') return {"ć", "č"};
    if (lower == 'd') return {"đ"};
    if (lower == 'e') return {"€"};
    if (lower == 's') return {"š"};
    if (lower == 'z') return {"ž"};
    if (c == ',') return {"„", "\"", "»", "«"};
    return QStringList();
}
QStringList AccentMap::getAccentsHE(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"שׂ", "שׁ", "\u05b0"};
    if (lower == 'b') return {"׆"};
    if (lower == 'e') return {"\u05b8", "\u05b3", "\u05bb"};
    if (lower == 'g') return {"ױ"};
    if (lower == 'h') return {"ײ", "ײַ", "ׯ", "\u05b4"};
    if (lower == 'm') return {"\u05b5"};
    if (lower == 'p') return {"\u05b7", "\u05b2"};
    if (lower == 's') return {"\u05bc"};
    if (lower == 't') return {"ﭏ"};
    if (lower == 'u') return {"וֹ", "וּ", "װ", "\u05b9"};
    if (lower == 'x') return {"\u05b6", "\u05b1"};
    if (lower == 'y') return {"ױ"};
    if (c == ',') return {"\"", "'", "'", "״", "׳"};
    if (c == '.') return {"\u05ab", "\u05bd", "\u05bf"};
    if (c == '-') return {"־"};
    return QStringList();
}
QStringList AccentMap::getAccentsHU(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á"};
    if (lower == 'e') return {"é"};
    if (lower == 'i') return {"í"};
    if (lower == 'o') return {"ó", "ő", "ö"};
    if (lower == 'u') return {"ú", "ű", "ü"};
    if (c == ',') return {"„", "\"", "»", "«"};
    return QStringList();
}
QStringList AccentMap::getAccentsIS(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á", "æ"};
    if (lower == 'd') return {"ð"};
    if (lower == 'e') return {"é"};
    if (lower == 'o') return {"ó", "ö"};
    if (lower == 'u') return {"ú"};
    if (lower == 'y') return {"ý"};
    if (lower == 't') return {"þ"};
    if (c == ',') return {"„", "\"", "‚", "'"};
    return QStringList();
}

QStringList AccentMap::getAccentsIPA(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ā", "á", "ǎ", "à", "ɑ", "ɑ̄", "ɑ́", "ɑ̌", "ɑ̀"};
    if (lower == 'c') return {"ĉ"};
    if (lower == 'e') return {"ē", "é", "ě", "è", "ê", "ê̄", "ế", "ê̌", "ề"};
    if (lower == 'i') return {"ī", "í", "ǐ", "ì"};
    if (lower == 'm') return {"m̄", "ḿ", "m̌", "m̀"};
    if (lower == 'n') return {"n̄", "ń", "ň", "ǹ", "ŋ", "ŋ̄", "ŋ́", "ŋ̌", "ŋ̀"};
    if (lower == 'o') return {"ō", "ó", "ǒ", "ò"};
    if (lower == 's') return {"ŝ"};
    if (lower == 'u') return {"ū", "ú", "ǔ", "ù", "ü", "ǖ", "ǘ", "ǚ", "ǜ"};
    if (lower == 'v') return {"ü", "ǖ", "ǘ", "ǚ", "ǜ"};
    if (lower == 'y') return {"¥"};
    if (lower == 'z') return {"ẑ"};
    if (c == ',') return {"“", "”", "‘", "’", "「", "」", "『", "』"};
    return QStringList();
}

QStringList AccentMap::getAccentsKU(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'c') return {"ç"};
    if (lower == 'e') return {"ê", "€"};
    if (lower == 'i') return {"î"};
    if (lower == 'o') return {"ö", "ô"};
    if (lower == 'l') return {"ł"};
    if (lower == 'n') return {"ň"};
    if (lower == 'r') return {"ř"};
    if (lower == 's') return {"ş"};
    if (lower == 'u') return {"û", "ü"};
    if (c == ',') return {"«", "»", "“", "”"};
    return QStringList();
}

QStringList AccentMap::getAccentsLT(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ą", "á", "à", "â", "ä", "ā"};
    if (lower == 'c') return {"č", "ć"};
    if (lower == 'e') return {"ė", "ę", "é", "è", "ê"};
    if (lower == 'i') return {"į", "í", "ì"};
    if (lower == 'y') return {"ý", "ÿ"};
    if (lower == 'u') return {"ū", "ú", "ù", "û", "ü"};
    if (lower == 'o') return {"ó", "ò", "ô", "ö", "ø"};
    return QStringList();
}

QStringList AccentMap::getAccentsMK(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'e') return {"ѐ"};
    if (lower == 'i') return {"ѝ"};
    if (c == ',') return {"„", "“", "’", "‘"};
    return QStringList();
}

QStringList AccentMap::getAccentsMT(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"à"};
    if (lower == 'c') return {"ċ"};
    if (lower == 'e') return {"è", "€"};
    if (lower == 'g') return {"ġ"};
    if (lower == 'h') return {"ħ"};
    if (lower == 'i') return {"ì"};
    if (lower == 'o') return {"ò"};
    if (lower == 'u') return {"ù"};
    if (lower == 'z') return {"ż"};
    return QStringList();
}

QStringList AccentMap::getAccentsMI(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ā"};
    if (lower == 'e') return {"ē"};
    if (lower == 'i') return {"ī"};
    if (lower == 'o') return {"ō"};
    if (lower == 'u') return {"ū"};
    if (lower == 's') return {"$"};
    if (c == ',') return {"“", "”", "‘", "’"};
    return QStringList();
}

QStringList AccentMap::getAccentsNL(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á", "à", "ä"};
    if (lower == 'c') return {"ç"};
    if (lower == 'e') return {"é", "è", "ë", "ê", "€"};
    if (lower == 'i') return {"í", "ï", "î"};
    if (lower == 'n') return {"ñ"};
    if (lower == 'o') return {"ó", "ö", "ô"};
    if (lower == 'u') return {"ú", "ü", "û"};
    if (c == ',') return {"“", "„", "”", "‘", ",", "’"};
    return QStringList();
}

QStringList AccentMap::getAccentsNO(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"å", "æ"};
    if (lower == 'e') return {"é", "è"};
    if (lower == 'i') return {"í"};
    if (lower == 'o') return {"ø", "ó", "ö"};
    if (lower == 'u') return {"ú", "ü"};
    if (lower == 'y') return {"ý"};
    if (c == ',') return {"“", "”", "‘", "’"};
    return QStringList();
}

QStringList AccentMap::getAccentsPI(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ā", "á", "ǎ", "à", "ɑ", "ɑ̄", "ɑ́", "ɑ̌", "ɑ̀"};
    if (lower == 'c') return {"ĉ"};
    if (lower == 'e') return {"ē", "é", "ě", "è", "ê", "ê̄", "ế", "ê̌", "ề"};
    if (lower == 'i') return {"ī", "í", "ǐ", "ì"};
    if (lower == 'm') return {"m̄", "ḿ", "m̌", "m̀"};
    if (lower == 'n') return {"n̄", "ń", "ň", "ǹ", "ŋ", "ŋ̄", "ŋ́", "ŋ̌", "ŋ̀"};
    if (lower == 'o') return {"ō", "ó", "ǒ", "ò"};
    if (lower == 's') return {"ŝ"};
    if (lower == 'u') return {"ū", "ú", "ǔ", "ù", "ü", "ǖ", "ǘ", "ǚ", "ǜ"};
    if (lower == 'v') return {"ü", "ǖ", "ǘ", "ǚ", "ǜ"};
    if (lower == 'y') return {"¥"};
    if (lower == 'z') return {"ẑ"};
    if (c == ',') return {"“", "”", "‘", "’", "「", "」", "『", "』"};
    return QStringList();
}

QStringList AccentMap::getAccentsPIE(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ā"};
    if (lower == 'e') return {"ē"};
    if (lower == 'o') return {"ō"};
    if (lower == 'k') return {"ḱ"};
    if (lower == 'g') return {"ǵ"};
    if (lower == 'r') return {"r̥"};
    if (lower == 'l') return {"l̥"};
    if (lower == 'm') return {"m̥"};
    if (lower == 'n') return {"n̥"};
    return QStringList();
}

QStringList AccentMap::getAccentsRO(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"ă", "â"};
    if (lower == 'i') return {"î"};
    if (lower == 's') return {"ș"};
    if (lower == 't') return {"ț"};
    if (c == ',') return {"„", "”", "«", "»"};
    return QStringList();
}

QStringList AccentMap::getAccentsROM(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á", "â", "ă", "ā"};
    if (lower == 'b') return {"ḇ"};
    if (lower == 'c') return {"č", "ç"};
    if (lower == 'd') return {"ḑ", "ḍ", "ḏ", "ḏ̇"};
    if (lower == 'e') return {"ê", "ě", "ĕ", "ē", "é", "ə"};
    if (lower == 'g') return {"ġ", "ǧ", "ğ", "ḡ", "g̃", "g̱"};
    if (lower == 'h') return {"ḧ", "ḩ", "ḥ", "ḫ", "ẖ"};
    if (lower == 'i') return {"í", "ı", "î", "ī", "ı̇̄"};
    if (lower == 'j') return {"ǰ", "j̱"};
    if (lower == 'k') return {"ḳ", "ḵ"};
    if (lower == 'l') return {"ł"};
    if (lower == 'n') return {"ⁿ", "ñ"};
    if (lower == 'o') return {"ó", "ô", "ö", "ŏ", "ō", "ȫ"};
    if (lower == 'p') return {"p̄"};
    if (lower == 'r') return {"ṙ", "ṛ"};
    if (lower == 's') return {"ś", "š", "ş", "ṣ", "s̱", "ṣ̄"};
    if (lower == 't') return {"ẗ", "ţ", "ṭ", "ṯ"};
    if (lower == 'u') return {"ú", "û", "ü", "ū", "ǖ"};
    if (lower == 'v') return {"v̇", "ṿ", "ᵛ"};
    if (lower == 'y') return {"̀y"};
    if (lower == 'z') return {"ż", "ž", "z̄", "z̧", "ẓ", "z̤", "ẕ"};
    if (c == '.') return {"’", "ʾ", "ʿ", "′", "…"};
    return QStringList();
}

QStringList AccentMap::getAccentsSK(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"á", "ä"};
    if (lower == 'c') return {"č"};
    if (lower == 'd') return {"ď"};
    if (lower == 'e') return {"é", "€"};
    if (lower == 'i') return {"í"};
    if (lower == 'l') return {"ľ", "ĺ"};
    if (lower == 'n') return {"ň"};
    if (lower == 'o') return {"ó", "ô"};
    if (lower == 'r') return {"ŕ"};
    if (lower == 's') return {"š"};
    if (lower == 't') return {"ť"};
    if (lower == 'u') return {"ú"};
    if (lower == 'y') return {"ý"};
    if (lower == 'z') return {"ž"};
    if (c == ',') return {"„", "“", "‚", "‘", "»", "«", "›", "‹"};
    return QStringList();
}

QStringList AccentMap::getAccentsSL(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'c') return {"č", "ć"};
    if (lower == 's') return {"š"};
    if (lower == 'z') return {"ž"};
    if (c == ',') return {"„", "“", "‚", "‘", "»", "«", "›", "‹"};
    return QStringList();
}

QStringList AccentMap::getAccentsSR(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'c') return {"ć", "č"};
    if (lower == 'd') return {"đ"};
    if (lower == 's') return {"š"};
    if (lower == 'z') return {"ž"};
    if (c == ',') return {"„", "“", "‚", "’", "»", "«", "›", "‹"};
    return QStringList();
}

QStringList AccentMap::getAccentsSRCyrillic(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'd') return {"ђ", "џ"};
    if (lower == 'l') return {"љ"};
    if (lower == 'n') return {"њ"};
    if (lower == 'c') return {"ћ"};
    return QStringList();
}

QStringList AccentMap::getAccentsSV(QChar c)
{
    QChar lower = c.toLower();
    if (lower == 'a') return {"å", "ä"};
    if (lower == 'e') return {"é"};
    if (lower == 'o') return {"ö"};
    if (c == ',') return {"”", "’", "»", "«"};
    return QStringList();
}

QStringList AccentMap::getAccentsVI(QChar c)
{
    QChar lower = c.toLower();

    if (lower == 'a') return {"à", "ả", "ã", "á", "ạ", "ă", "ằ", "ẳ", "ẵ", "ắ", "ặ", "â", "ầ", "ẩ", "ẫ", "ấ", "ậ"};
    if (lower == 'd') return {"đ"};
    if (lower == 'e') return {"è", "ẻ", "ẽ", "é", "ẹ", "ê", "ề", "ể", "ễ", "ế", "ệ"};
    if (lower == 'i') return {"ì", "ỉ", "ĩ", "í", "ị"};
    if (lower == 'o') return {"ò", "ỏ", "õ", "ó", "ọ", "ô", "ồ", "ổ", "ỗ", "ố", "ộ", "ơ", "ờ", "ở", "ỡ", "ớ", "ợ"};
    if (lower == 'u') return {"ù", "ủ", "ũ", "ú", "ụ", "ư", "ừ", "ử", "ữ", "ứ", "ự"};
    if (lower == 'y') return {"ỳ", "ỷ", "ỹ", "ý", "ỵ"};

    return QStringList();
}
