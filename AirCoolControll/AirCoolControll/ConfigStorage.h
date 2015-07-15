#ifndef __CONFIGSTORAGE__
#define __CONFIGSTORAGE__

#include "ConfigMap.h"

class ConfigStorage : public std::list<ConfigMapShared>
{
public:
    ConfigStorage();
    ~ConfigStorage();

private:
    bool ConfigStorage::readXMLConfig(const QString& path);
};

#endif //__CONFIGSTORAGE__