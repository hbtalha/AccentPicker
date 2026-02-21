#include <QString>
#include <QStringLiteral>
#include <QList>

namespace ConfigKey
{
struct Active {

    using Type = bool;
    static QString name()
    {
        return QStringLiteral("active");
    }
    static Type defaultValue()
    {
        return true;
    }
};

struct StartHidden {
    using Type = bool;
    static QString name()
    {
        return QStringLiteral("startHidden");
    }
    static Type defaultValue()
    {
        return true;
    }
};

struct AutoStart {
    using Type = bool;
    static QString name()
    {
        return QStringLiteral("autoStart");
    }
    static Type defaultValue()
    {
        return true;
    }
};

struct SelectedAllCharacterSets {
    using Type = bool;
    static QString name()
    {
        return QStringLiteral("selectedAllCharacterSets");
    }
    static Type defaultValue()
    {
        return true;
    }
};

struct SelectedCharacterSets {
    using Type = QStringList;
    static QString name()
    {
        return QStringLiteral("selectedCharacterSets");
    }
    static  Type defaultValue()
    {
        return {};
    }
};
}
