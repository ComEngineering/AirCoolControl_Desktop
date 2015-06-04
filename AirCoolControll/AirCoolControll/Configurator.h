#ifndef __CONFIGURATOR__
#define __CONFIGURATOR__

#include <QString>
#include <qvariant.h>
#include <qsettings.h>
#include <qsharedpointer.h>

class Configurator
{
public:
    Configurator();
    ~Configurator();

    static const QString& getVersion();
    static const QVariant getSettings(const QString& key);
    static void setSettings(const QString& key, const QVariant& value);
    static  QString getConfigFilesPath();

    static int getMaximumRetryCount(void);
    static void setMaximumRetryCount(int count);

private:
    const static QString s_version;
    static QSharedPointer <QSettings> s_settings;
};

#endif //__CONFIGURATOR__

