#include "Configurator.h"
#include <qapplication.h>

const QString Configurator::s_version("0.0.1.3");
QSharedPointer <QSettings>   Configurator::s_settings;

Configurator::Configurator()
{
    QApplication::setOrganizationName("TEKO");
    QApplication::setApplicationName("AirCoolController");

    QString settingsFile = QApplication::applicationDirPath();
    settingsFile += "/settings.ini";
    s_settings = QSharedPointer <QSettings> (new QSettings(settingsFile, QSettings::IniFormat));
   // QApplication::restoreGeometry(s_settings->value("Main/Geometry").toByteArray());
}

Configurator::~Configurator()
{
    //s_settings->setValue("Main/Geometry", saveGeometry());
    s_settings->sync();
}

const QString& Configurator::getVersion()
{
    return s_version;
}

const QVariant Configurator::getSettings(const QString& key)
{
    if (s_settings == NULL)
        return QVariant();

    return s_settings->value(key);
}

void Configurator::setSettings(const QString& key, const QVariant& value)
{
    if (s_settings != NULL)
    {
        s_settings->setValue(key, value);
    }
}

QString Configurator::getConfigFilesPath()
{
    return QApplication::applicationDirPath() + "/configs/";
}

QString Configurator::getTestFilesPath()
{
    return QApplication::applicationDirPath() + "/tests/";
}