#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <qstring.h>
#include "VersionStorage.h"
#include <memory>

class DeviceExplorer;

class DeviceInfo 
{
public:
    DeviceInfo(const QString& uart, int id, int speed, const QString& ven, const QString& p, const QString& ver);
    DeviceInfo(const QString& uart, int id);
    ~DeviceInfo();

    bool empty() const;
    const QString& getUART() const { return m_uart; }
    const QString& getVendor() const { return m_vendor; }
    const QString& getProduct() const { return m_product; }
    QString        getVersion() const { return QString(m_version); } 
    int            getID() const { return m_id; }
    int            getSpeed() const { return m_speed; }

    QString        getDescription() const;
    
    bool checkVersion(const VersionStorage& min, const VersionStorage& max)const
    {
        return m_version <= max && m_version >= min;
    }
    std::shared_ptr<DeviceExplorer> getExplorer() { return m_explorer; }
    void setExplorer(std::shared_ptr<DeviceExplorer> explorer) { m_explorer = explorer; }

private:
    QString              m_uart;
    int                  m_id;
    int                  m_speed;
    QString              m_vendor;
    QString              m_product;
    VersionStorage       m_version;

    std::shared_ptr<DeviceExplorer> m_explorer;

    bool                 m_empty;
};

typedef std::shared_ptr<DeviceInfo> DeviceInfoShared;

#endif // DEVICEINFO_H
