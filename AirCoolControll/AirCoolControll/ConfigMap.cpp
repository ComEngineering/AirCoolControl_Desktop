#include "ConfigMap.h"
#include <boost/property_tree/xml_parser.hpp>
#include "DeviceInfo.h"
#include <algorithm>

using ert = ConfigMap::ErrorDetector::Error;
const std::unordered_map<std::string, ert::DetectionType> ert::s_error_map = { 
    { "EQ", ert::EQ }, { "GT", ert::GT }, { "LT", ert::LT }, { "GTE", ert::GTE }, { "LTE", ert::LTE }, { "AND", ert::AND }, { "XOR", ert::XOR } 
};

ConfigMap::ConfigMap(const std::string& configName, const std::string& vendor, const std::string& product, const std::string& versionMin, const std::string& versionMax) :
    m_configName(configName),
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
    m_map.push_back(std::pair<std::string, Parameter>(name,p));
    
    if (p.m_registerNumber < m_registersIntervals[n].first)
        m_registersIntervals[n].first = p.m_registerNumber;

    if (p.m_registerNumber > m_registersIntervals[n].second)
        m_registersIntervals[n].second = p.m_registerNumber;

}

int  ConfigMap::getRegisterNumber(const std::string& name) const
{
    if (!haveVariableWithName(name))
        return -1;

    Parameter p = findParameter(name)->second;
    return p.m_registerNumber;
}

bool ConfigMap::haveVariableWithName(const std::string& name) const
{
    ParameterMap::const_iterator f = findParameter(name);
    return f != m_map.end();
}

std::string ConfigMap::getParameterDescription(const std::string& name) const
{
    std::string rc;
    ParameterMap::const_iterator f = findParameter(name);
    if (f != m_map.end())
        rc = f->second.m_description;
    return rc;
}

QVariant ConfigMap::getValue(const std::string& name, const std::vector<quint16>& array) const
{
    ParameterMap::const_iterator f = findParameter(name);
    if (f == m_map.end())
        return QVariant(QString(QObject::tr("undefined")));

    Parameter p = f->second;
    int index = -1;
    for (auto interval : m_registersIntervals)
        if (interval.in(p.m_registerNumber))
        {
            index = p.m_registerNumber - interval.first;
            break;
        }

    assert(index != -1);
  
    int ret = array[index];

    QString errorDescription;
    if (!f->second.m_errorDetector.isValid(ret,errorDescription))
    {
        return QVariant(errorDescription);
    }

    if (p.m_isBool)
    {
        ret =  (ret & (1 << p.m_bitNumber)) ? 1 : 0;
    }

    if (!p.m_decodeMethod.empty())
        ret = decodeWithMethod(ret, p.m_decodeMethod);

    return QVariant(ret);
}

bool ConfigMap::isVariableBool(const std::string& name, int& bitNumber)
{
    assert(haveVariableWithName(name));

    Parameter p = findParameter(name)->second;
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

bool  ConfigMap::isSupport(const DeviceInfoShared info) const
{
    if (QString::compare(info->getVendor(), QString::fromStdString(m_vendor), Qt::CaseInsensitive) != 0 || QString::compare(info->getProduct(), QString::fromStdString(m_product), Qt::CaseInsensitive) != 0)
        return false;

    return info->checkVersion(m_versionMin,m_versionMax);
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

ConfigMap::ParameterMap::const_iterator ConfigMap::findParameter(const std::string& name) const
{
    ParameterMap::const_iterator f = std::find_if(m_map.begin(), m_map.end(), [name](const std::pair<std::string, Parameter>& e){return name == e.first; });
    assert(f != m_map.end());
    return f;
}

const ConfigMap::ParameterList& ConfigMap::getParametersList(ConfigMap::RegisterType e)
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

ConfigMap::RegisterType ConfigMap::getVariableType(const std::string& key) const
{
    const ParameterMap::const_iterator a_parameter = findParameter(key);
    
    return a_parameter->second.m_type;
}

const std::pair<std::string,ConfigMap::Parameter>& ConfigMap::operator[](int n) const
{
    assert(m_map.size() > n);

    return m_map.at(n);
}