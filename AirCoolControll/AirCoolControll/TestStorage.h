#ifndef __TESTSTORAGE__
#define __TESTSTORAGE__

#include "SimpleTest.h"
#include <list>
#include "ConfigStorage.h"

class TestStorage : public std::list<SimpleTestShared>
{
public:
    TestStorage();
    ~TestStorage();

    std::vector<std::string> getNames() const;
    SimpleTestShared TestStorage::getTest(int n) const;
    void read(const ConfigStorage* configs);

private:
    bool readXML_Test(const QString& path);
    const ConfigStorage* m_configs;
};

#endif // __TESTSTORAGE__