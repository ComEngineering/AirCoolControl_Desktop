#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>

#include <qstring.h>
#include "VersionStorage.h"
#include <memory>

class DeviceExplorer;

class DeviceInfo : public QObject
{
    Q_OBJECT

public:
    DeviceInfo(const QString& uart, int id, const QString& ven, const QString& p, const QString& ver, QObject  *parent = NULL);
    explicit DeviceInfo(QObject  *parent = NULL);

    bool empty() const;

    QString              m_uart;
    int                  m_id;
    QString              m_vendor;
    QString              m_product;
    VersionStorage       m_version;

    std::shared_ptr<DeviceExplorer> m_explorer;
};

typedef std::shared_ptr<DeviceInfo> DeviceInfoShared;

#endif // DEVICEINFO_H
