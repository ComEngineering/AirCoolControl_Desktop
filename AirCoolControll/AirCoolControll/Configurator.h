#ifndef __CONFIGURATOR__
#define __CONFIGURATOR__

#include <QString>
#include <qvariant.h>
#include <qsettings.h>
#include <qsharedpointer.h>

#define DEFINE_PROPERTY(name,default_value) \
    static int get ## name (){ return (s_settings == NULL) ? default_value : s_settings->value(#name, default_value).toInt(); } \
    static void set ## name(int value){ if(s_settings) s_settings->setValue(#name, value);}


class Configurator
{
public:
    Configurator();
    ~Configurator();

    static const QString& getVersion();
    static const QVariant getSettings(const QString& key);
    static void setSettings(const QString& key, const QVariant& value);
    static QString getConfigFilesPath();
    static QString getTestFilesPath();

    DEFINE_PROPERTY(RetryCount, 3);
    DEFINE_PROPERTY(ChankTimeout, 20);
    DEFINE_PROPERTY(MaximunTimeout, 50);
    DEFINE_PROPERTY(PullInterval, 1000);
    DEFINE_PROPERTY(HistoryLength, 24 * 60 * 60);

private:
    const static QString s_version;
    static QSharedPointer <QSettings> s_settings;
};

#endif //__CONFIGURATOR__

