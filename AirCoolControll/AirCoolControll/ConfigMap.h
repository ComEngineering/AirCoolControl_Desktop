#ifndef __CONFIGMAP__
#define __CONFIGMAP__

#include <string>
#include <unordered_map>
#include <vector>
#include <Interval.h>
#include <qstring.h>
#include <qvariant.h>
#include <memory>
#include "DeviceInfo.h"
#include "VersionStorage.h"
#include <assert.h>

class ConfigMap
{
public:
    ConfigMap(const std::string& fileName, const std::string& configName, const std::string& vendor, const std::string& product, const std::string& versionMin, const std::string& versionMax);
    ConfigMap(void){}
    ~ConfigMap();

    friend class EditConfigWindow;

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

    class ErrorDetector
    {
    public:
        ErrorDetector(){}
        
        struct Error
        {
            enum DetectionType
            {
                EQ = 0,
                GT,
                LT,
                GTE,
                LTE,
                AND,
                XOR,
                UNSUPPORT
            };
            const static std::unordered_map<std::string, DetectionType> s_error_map;

            DetectionType m_type = UNSUPPORT;
            std::string   m_description;
            int           m_value;

            Error(std::string name, std::string desc, int v)
            {
                std::unordered_map<std::string, DetectionType>::const_iterator f = s_error_map.find(name);
                if (f == s_error_map.end())
                {
                    m_type = UNSUPPORT;
                }
                else
                {
                    m_type = f->second;
                    m_description = desc;
                    m_value = v;
                }
            }

            bool check(qint16 value) const 
            {
                bool rc = false;
                switch (m_type) 
                {
                case EQ:
                    rc = value == m_value;
                    break;
                case GT:
                    rc = value > m_value;
                    break;
                case LT:
                    rc = value < m_value;
                    break;
                case GTE:
                    rc = value >= m_value;
                    break;
                case LTE:
                    rc = value <= m_value;
                    break;
                case AND:
                    rc = value & m_value;
                    break;
                case XOR:
                    rc = value ^ m_value;
                    break;
                }

                return rc;
            }
        };

        void addError(std::string name, std::string desc, int v)
        {
            Error e(name, desc, v);
            if (e.m_type != Error::UNSUPPORT)
            {
                m_detection_list.push_back(e);
            }
        }

        bool isValid(qint16 value,QString& errorDescription) const
        {
            bool rc = true;
            for (const auto& i : m_detection_list)
            {
                if (i.check(value))
                {
                    rc = false;
                    errorDescription = QString::fromStdString(i.m_description);
                    break;
                }
            }
            return rc;
        }

        int size(void) const { return m_detection_list.size(); }
        const Error& operator[](int n) const { return m_detection_list[n]; }
    private:
        std::vector<Error> m_detection_list;
    };

    typedef struct _Parameter
    {
        int                        m_registerNumber;
        std::string                m_description;
        bool                       m_isBool;
        int                        m_bitNumber;
        std::string                m_decodeMethod;
        int                        m_minValue;
        int                        m_maxValue;
        RegisterType               m_type;
        ErrorDetector              m_errorDetector;
        std::vector<std::pair<std::string, int>> m_enumeration;

        _Parameter() : m_registerNumber(-1), m_minValue(SHRT_MIN), m_maxValue(SHRT_MAX){}
    } Parameter;

    typedef std::vector<std::pair<std::string, std::string>> ParameterList;
    typedef std::vector<std::pair<std::string, Parameter>> ParameterMap;

    const std::string&   getName(void) const { return m_configName; }
    void                 setName(const std::string& name) { m_configName = name; }
    void                 setFilePath(const std::string& path) { m_filePath = path; }
    void                 fetchInfo(std::string& vendor, std::string& product, VersionStorage& max, VersionStorage& min){ 
                             max = m_versionMax; 
                             min = m_versionMin; 
                             vendor = m_vendor; 
                             product = m_product; 
                         }
    void                 addVariable(const std::string& name, const Parameter& p);
    int                  getRegisterNumber(const std::string& name) const;
    bool                 haveVariableWithName(const std::string& name) const;
    QVariant             getValue(const std::string& name, const std::vector<quint16>& array) const;
    bool                 isVariableBool(const std::string& name, int& bitNumber);
    Interval&            getInterval(int n);
    bool                 isSupport(const DeviceInfoShared info) const;
    const ParameterList& getParametersList(ConfigMap::RegisterType e);
    RegisterType         getVariableType(const std::string& key) const;
    void                 setUI_Config(const std::string& type, const std::string& configFile);
    ParameterMap::const_iterator 
                         findParameter(const std::string& name) const;
    std::string          getParameterDescription(const std::string& name) const;
    const std::pair<std::string,Parameter>& 
                         operator[](int n) const;
    int                  size(void) const { return m_map.size(); }
    bool                 saveToFile(const std::string& path) const;
    void                 save(void) const;
    void                 deleteParameterWithName(const std::string& name);
    void                 setNewParameter(const std::string& name, const Parameter& newParameter);

private:
    static qint16 decodeWithMethod(qint16 value, const std::string& method);
    
private:
    std::string     m_filePath;
    std::string     m_configName;
    std::string     m_vendor;
    std::string     m_product;
    VersionStorage  m_versionMin;
    VersionStorage  m_versionMax;
    std::string     m_UI_type;
    std::string     m_UI_config;

    ParameterMap    m_map;
    Interval        m_registersIntervals[REGISTER_PULL_COUNT];

    ParameterList   m_parameters[REGISTER_PULL_COUNT];
};

typedef std::shared_ptr<ConfigMap> ConfigMapShared;

#endif // __CONFIGMAP__