#ifndef __CONFIGMAP__
#define __CONFIGMAP__

#include <string>
#include <unordered_map>
#include <qvector.h>
#include <Interval.h>
#include <qstring.h>
#include "VersionStorage.h"
#include <memory>
#include "DeviceInfo.h"

class ConfigMap
{
public:
    ConfigMap(const std::string& vendor, const std::string& product, const std::string& versionMin, const std::string& versionMax);
    ~ConfigMap();

    typedef struct 
    {
        int          m_registerNumber;
        std::string  m_description;
        bool         m_isBool;
        bool         m_isWriteble;
        int          m_bitNumber;
        std::string  m_decodeMethod;
        float        m_minValue;
        float        m_maxValue;
    } Parameter;

    typedef std::vector<std::pair<std::string, std::string>> ParameterList;
    typedef std::unordered_map<std::string, Parameter> ParameterMap;

    void addVariable(const std::string& name, const Parameter& p);
    bool isVariableOut(const std::string& name) const;
    int  getRegisterNumber(const std::string& name) const;
    bool haveVariableWithName(const std::string& name) const;
    unsigned int  getValue(const std::string& name, const QVector<quint16>& array) const;
    bool isVariableBool(const std::string& name, int& bitNumber);
    Interval& getInputInterval();
    Interval& getOutputInterval();
    bool  isSupport(const DeviceInfoShared info) const;
    ParameterList getInputParametersList(bool isForRead = true) const;
    ParameterList getOutputParametersList() const;

private:
    static qint16 decodeWithMethod(qint16 value, const std::string& method);

private:
    std::string     m_vendor;
    std::string     m_product;
    VersionStorage  m_versionMin;
    VersionStorage  m_versionMax;

    ParameterMap    m_map;
    Interval        m_inputRegistersInterval;
    Interval        m_outputRegistersInterval;
};

typedef std::shared_ptr<ConfigMap> ConfigMapShared;
typedef std::list<ConfigMapShared> ConfigList;

#endif // __CONFIGMAP__