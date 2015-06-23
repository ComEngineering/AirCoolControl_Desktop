#include "DeviceInfo.h"

DeviceInfo::DeviceInfo(const QString& uart, int id, int speed, const QString& ven, const QString& p, const QString& ver) :
    m_uart(uart),
    m_id(id),
    m_speed(speed),
    m_vendor(ven),
    m_product(p),
    m_version(ver),
    m_empty(false)
{
}

DeviceInfo::DeviceInfo(const QString& uart, int id) : 
    m_empty(true),
    m_uart(uart),
    m_id(id)
{
}

DeviceInfo::~DeviceInfo()
{

}

bool DeviceInfo::empty() const
{
    return m_empty;
}

QString DeviceInfo::getDescription() const
{
    return getUART() + ':' + QString::number(getID()) + '(' + getVendor() + '.' + getProduct() + '.' + getVersion() + ')';
}