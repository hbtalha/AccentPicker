// config/appconfig.h
#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QSettings>

template<class T>
concept ConfigKeyConcept = requires{
    typename T::Type;
    {T::name()}-> std::convertible_to<QString>;
    {T::defaultValue()} -> std::convertible_to<typename T::Type>;
};

class AppConfig : public QObject
{
    Q_OBJECT
public:
    static AppConfig& instance()
    {
        static AppConfig s_instance;
        return s_instance;
    }

    template<ConfigKeyConcept Key>
    typename Key::Type get()
    {
        QVariant v = settings.value(Key::name(), QVariant::fromValue(Key::defaultValue()));
        return v.value<typename Key::Type>();
    }

    template<ConfigKeyConcept Key>
    void set(const typename Key::Type& value)
    {
        settings.setValue(Key::name(), QVariant::fromValue(value));
    }

private:
    explicit AppConfig(QObject* parent = nullptr)
        : QObject(parent)
    {}

    QSettings settings;
    Q_DISABLE_COPY(AppConfig)
};

extern AppConfig* appConfig;

#endif
