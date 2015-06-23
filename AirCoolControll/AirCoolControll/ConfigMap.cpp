#include "ConfigMap.h"
#include <boost/property_tree/xml_parser.hpp>
#include "DeviceInfo.h"

ConfigMap::ConfigMap(const std::string& vendor, const std::string& product, const std::string& versionMin, const std::string& versionMax) :
    m_vendor(vendor),
    m_product(product),
    m_versionMin(QString::fromStdString(versionMin)),
    m_versionMax(QString::fromStdString(versionMax)),
    m_UI_type("none")
{
    if ((!m_versionMin.isLegal()) || (!m_versionMax.isLegal()))
        throw boost::property_tree::xml_parser_error("Bad version string in config file",product,0);
}


ConfigMap::~ConfigMap()
{
}

void ConfigMap::addVariable(int n,const std::string& name, const Parameter& p)
{
    m_map[name] = p;
    
    if (p.m_registerNumber < m_registersIntervals[n].first)
        m_registersIntervals[n].first = p.m_registerNumber;

    if (p.m_registerNumber > m_registersIntervals[n].second)
        m_registersIntervals[n].second = p.m_registerNumber;

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

unsigned int  ConfigMap::getValue(const std::string& name, const std::vector<quint16>& array) const
{
    if (!haveVariableWithName(name))
        return -1;

    Parameter p = m_map.at(name);
    
    int index = p.m_registerNumber - m_registersIntervals[p.m_type].first;
  
    qint16 ret = array[index];

    if (p.m_isBool)
    {
        ret =  (ret & (1 << p.m_bitNumber)) ? 1 : 0;
    }

    if (!p.m_decodeMethod.empty())
        ret = decodeWithMethod(ret, p.m_decodeMethod);

    return ret;
}

bool ConfigMap::isVariableBool(const std::string& name, int& bitNumber)
{
    assert(haveVariableWithName(name));

    Parameter p = m_map.at(name);
    if (p.m_isBool)
    {
        bitNumber = p.m_bitNumber;
        return true;
    }
    
    return false;
}

Interval& ConfigMap::getInterval(int n) 
{
    return m_registersIntervals[n];
}

ConfigMap::RegisterType ConfigMap::getVariableType(const std::string& name) const
{
    return m_map.at(name).m_type;
}

bool  ConfigMap::isSupport(const DeviceInfoShared info) const
{
    if (QString::compare(info->getVendor(), QString::fromStdString(m_vendor), Qt::CaseInsensitive) != 0 || QString::compare(info->getProduct(), QString::fromStdString(m_product), Qt::CaseInsensitive) != 0)
        return false;

    return info->checkVersion(m_versionMin,m_versionMax);
}

ConfigMap::ParameterList ConfigMap::getParametersList(ConfigMap::RegisterType e)
{
    if (m_parameters[e].empty())
    {
        for (std::pair<std::string, Parameter> a_record : m_map)
        {
            if (a_record.second.m_type == e)
                m_parameters[e].push_back(std::pair<std::string, std::string>(a_record.first, a_record.second.m_description));
        }
    }
    return m_parameters[e];
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

void  ConfigMap::setUI_Config(const std::string& type, const std::string& configFile)
{
    m_UI_type = type;
    m_UI_config = configFile;
}