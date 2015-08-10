#ifndef __CONFIGSTORAGE__
#define __CONFIGSTORAGE__

#include "ConfigMap.h"

class ConfigStorage : public std::list<ConfigMapShared>
{
public:
    ConfigStorage();
    ~ConfigStorage();

    std::vector<std::string> getNames(void) const;
    ConfigMapShared getConfig(int n) const;
    ConfigMapShared getConfig(const std::string& name) const;

private:
    bool ConfigStorage::readXMLConfig(const QString& path);
};

#endif //__CONFIGSTORAGE__