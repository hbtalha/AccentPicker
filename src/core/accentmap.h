#ifndef ACCENTMAP_H
#define ACCENTMAP_H

#include <QMap>
#include <QStringList>
#include <QChar>

enum class Language {
    ALL,
    BG,      // Bulgarian
    CA,      // Catalan
    CRH,     // Crimean Tatar
    CUR,     // Currency
    CY,      // Welsh
    CZ,      // Czech
    DK,      // Danish
    GA,      // Gaeilge (Irish)
    GD,      // Gàidhlig (Scottish Gaelic)
    DE,      // German
    EL,      // Greek
    EST,     // Estonian
    EPO,     // Esperanto
    FI,      // Finnish
    FR,      // French
    HR,      // Croatian
    HE,      // Hebrew
    HU,      // Hungarian
    IS,      // Iceland
    IPA,     // International Phonetic Alphabet
    IT,      // Italian
    KU,      // Kurdish
    LT,      // Lithuanian
    MK,      // Macedonian
    MT,      // Maltese
    MI,      // Maori
    NL,      // Dutch
    NO,      // Norwegian
    PI,      // Pinyin
    PIE,     // Proto-Indo-European
    PL,      // Polish
    PT,      // Portuguese
    RO,      // Romanian
    ROM,     // Middle Eastern Romanization
    SK,      // Slovak
    SL,      // Slovenian
    SP,      // Spain
    SR,      // Serbian
    SR_CYRL, // Serbian Cyrillic
    SV,      // Swedish
    TK,      // Turkish
    VI,      // Vietnamese
};

struct LanguageInfo {
    Language language;
    QString code;
    QString name;
};

class AccentMap
{
public:
    static QStringList getAccents(QChar baseChar, const QStringList &langCodes);
    static QList<LanguageInfo> getAllLanguages();

private:
    static QStringList getAccentsForLanguage(QChar baseChar, Language lang);
    static QStringList getAccentsForLanguage(QChar baseChar, const QString &langCode);

    // Individual language accent maps
    static QStringList getAccentsBG(QChar c);
    static QStringList getAccentsCA(QChar c);
    static QStringList getAccentsCRH(QChar c);
    static QStringList getAccentsCUR(QChar c);
    static QStringList getAccentsCY(QChar c);
    static QStringList getAccentsCZ(QChar c);
    static QStringList getAccentsDK(QChar c);
    static QStringList getAccentsGA(QChar c);
    static QStringList getAccentsGD(QChar c);
    static QStringList getAccentsDE(QChar c);
    static QStringList getAccentsEL(QChar c);
    static QStringList getAccentsEST(QChar c);
    static QStringList getAccentsEPO(QChar c);
    static QStringList getAccentsFI(QChar c);
    static QStringList getAccentsFR(QChar c);
    static QStringList getAccentsHR(QChar c);
    static QStringList getAccentsHE(QChar c);
    static QStringList getAccentsHU(QChar c);
    static QStringList getAccentsIS(QChar c);
    static QStringList getAccentsIPA(QChar c);
    static QStringList getAccentsIT(QChar c);
    static QStringList getAccentsKU(QChar c);
    static QStringList getAccentsLT(QChar c);
    static QStringList getAccentsMK(QChar c);
    static QStringList getAccentsMT(QChar c);
    static QStringList getAccentsMI(QChar c);
    static QStringList getAccentsNL(QChar c);
    static QStringList getAccentsNO(QChar c);
    static QStringList getAccentsPI(QChar c);
    static QStringList getAccentsPIE(QChar c);
    static QStringList getAccentsPL(QChar c);
    static QStringList getAccentsPT(QChar c);
    static QStringList getAccentsRO(QChar c);
    static QStringList getAccentsROM(QChar c);
    static QStringList getAccentsSK(QChar c);
    static QStringList getAccentsSL(QChar c);
    static QStringList getAccentsSP(QChar c);
    static QStringList getAccentsSR(QChar c);
    static QStringList getAccentsSRCyrillic(QChar c);
    static QStringList getAccentsSV(QChar c);
    static QStringList getAccentsTK(QChar c);
    static QStringList getAccentsVI(QChar c);

    // Cache for ALL languages
    static QMap<QChar, QStringList> allLanguagesCache;
};

#endif // ACCENTMAP_H
