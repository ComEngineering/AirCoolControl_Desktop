#include "DeviceInfo.h"

DeviceInfo::DeviceInfo(const QString& uart, int id, const QString& ven, const QString& p, const QString& ver, QObject  *parent) :
m_uart(uart),
m_id(id),
m_vendor(ven),
m_product(p),
m_version(ver)
{
}

DeviceInfo::DeviceInfo(QObject  *parent) : m_id(0)
{
}

bool DeviceInfo::empty() const
{
    return (0 == m_id);
}