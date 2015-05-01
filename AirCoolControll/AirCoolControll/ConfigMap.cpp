#include "ConfigMap.h"
#include <boost/property_tree/xml_parser.hpp>



ConfigMap::ConfigMap(const std::string& vendor, const std::string& product, const std::string& versionMin, const std::string& versionMax) :
    m_vendor(vendor),
    m_product(product),
    m_versionMin(QString::fromStdString(versionMin)),
    m_versionMax(QString::fromStdString(versionMax))
{
    if ((!m_versionMin.isLegal()) || (!m_versionMax.isLegal()))
        throw boost::property_tree::xml_parser_error("Bad version string in config file",product,0);
}


ConfigMap::~ConfigMap()
{
}

void ConfigMap::addVariable(const std::string& name, const Parameter& p)
{
    m_map[name] = p;
    if (!p.m_isWriteble)
    {
        if (p.m_registerNumber < m_inputRegistersInterval.first)
            m_inputRegistersInterval.first = p.m_registerNumber;

        if (p.m_registerNumber > m_inputRegistersInterval.second)
            m_inputRegistersInterval.second = p.m_registerNumber;
    }
}

int  ConfigMap::getRegisterNumber(const std::string& name) const
{
    if (!haveVariableWithName(name))
        return -1;

    Parameter p = m_map.at(name);
    return p.m_registerNumber;
}

bool ConfigMap::haveVariableWithName(const std::string& name) const
{
    return m_map.find(name) != m_map.end();
}

unsigned int  ConfigMap::getValue(const std::string& name, const QVector<quint16>& array) const
{
    if (!haveVariableWithName(name))
        return -1;

    Parameter p = m_map.at(name);
    int index = p.m_registerNumber - ((p.m_isWriteble) ? m_outputRegistersInterval.first : m_inputRegistersInterval.first);
  
    qint16 ret = array[index];

    if (p.m_isBool)
    {
        ret =  (ret & (1 << p.m_bitNumber)) ? 1 : 0;
    }

    if (!p.m_decodeMethod.empty())
        ret = decodeWithMethod(ret, p.m_decodeMethod);

    return ret;
}

Interval& ConfigMap::getInputInterval() 
{
    return m_inputRegistersInterval;
}

Interval& ConfigMap::getOutputInterval()
{
    return m_outputRegistersInterval;
}

bool ConfigMap::isVariableOut(const std::string& name) const
{
    return m_outputRegistersInterval.in(getRegisterNumber(name));
}

bool  ConfigMap::isSupport(const DeviceInfo& info) const
{
    if (QString::compare(info.m_vendor, QString::fromStdString(m_vendor), Qt::CaseInsensitive) != 0 || QString::compare(info.m_product, QString::fromStdString(m_product), Qt::CaseInsensitive) != 0)
        return false;

    return info.m_version <= m_versionMax && info.m_version >= m_versionMin;
}

ConfigMap::ParameterList ConfigMap::getInputParametersList(bool isForRead) const
{
    std::vector<std::pair<std::string, std::string>> rc;
    for (std::pair<std::string, Parameter> a_record : m_map)
    {
        if (a_record.second.m_isWriteble != isForRead)
            rc.push_back(std::pair<std::string, std::string>(a_record.first, a_record.second.m_description));
    }

    return rc;
}

ConfigMap::ParameterList ConfigMap::getOutputParametersList() const
{
    return getInputParametersList(false);
}

qint16 ConfigMap::decodeWithMethod(qint16 value, const std::string& method)
{
    qint16 ret = value;
    if (method == "AT")
    {
        ret = (value & 0x007f);
        if (value & 0x0080)
            ret = -ret;
    }

    return ret;
}