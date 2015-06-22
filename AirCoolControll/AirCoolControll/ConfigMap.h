#ifndef __CONFIGMAP__
#define __CONFIGMAP__

#include <string>
#include <unordered_map>
#include <qvector.h>
#include <Interval.h>
#include <qstring.h>
#include <memory>
#include "DeviceInfo.h"
#include "VersionStorage.h"
#include <assert.h>

class ConfigMap
{
public:
    ConfigMap(const std::string& vendor, const std::string& product, const std::string& versionMin, const std::string& versionMax);
    ~ConfigMap();

    enum RegisterType {
        REGISTER_PULL_FIRST = 0,
        INPUT_REGISTER = 0,
        OUTPUT_REGISTER,
        COIL,
        REGISTER_PULL_COUNT
    };
      
    static RegisterType &NEXT(ConfigMap::RegisterType &c) {
        assert(c < REGISTER_PULL_COUNT);
        c = static_cast<RegisterType>(c + 1);
        return c;
    }

    typedef struct 
    {
        int                     m_registerNumber;
        std::string             m_description;
        bool                    m_isBool;
        ConfigMap::RegisterType m_type;
        int                     m_bitNumber;
        std::string             m_decodeMethod;
        float                   m_minValue;
        float                   m_maxValue;
    } Parameter;

    typedef std::vector<std::pair<std::string, std::string>> ParameterList;
    typedef std::unordered_map<std::string, Parameter> ParameterMap;

    void addVariable(int n,const std::string& name, const Parameter& p);
    ConfigMap::RegisterType getVariableType(const std::string& name) const;
    int  getRegisterNumber(const std::string& name) const;
    bool haveVariableWithName(const std::string& name) const;
    unsigned int  getValue(const std::string& name, const std::vector<quint16>& array) const;
    bool isVariableBool(const std::string& name, int& bitNumber);
    Interval& getInterval(int n);
    bool  isSupport(const DeviceInfoShared info) const;
    ParameterList getParametersList(ConfigMap::RegisterType e);
    void  setUI_Config(const std::string& type, const std::string& configFile);

private:
    static qint16 decodeWithMethod(qint16 value, const std::string& method);
    
private:
    std::string     m_vendor;
    std::string     m_product;
    VersionStorage  m_versionMin;
    VersionStorage  m_versionMax;
    std::string     m_UI_type;
    std::string     m_UI_config;

    ParameterMap    m_map;
    Interval        m_registersIntervals[REGISTER_PULL_COUNT];

    ConfigMap::ParameterList m_parameters[REGISTER_PULL_COUNT];
};

typedef std::shared_ptr<ConfigMap> ConfigMapShared;
typedef std::list<ConfigMapShared> ConfigList;

#endif // __CONFIGMAP__