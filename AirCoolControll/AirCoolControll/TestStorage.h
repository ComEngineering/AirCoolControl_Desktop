#ifndef __TESTSTORAGE__
#define __TESTSTORAGE__

#include "SimpleTest.h"
#include <list>

class TestStorage : public std::list<SimpleTestShared>
{
public:
    TestStorage();
    ~TestStorage();

    std::vector<std::string> getNames() const;
    SimpleTestShared TestStorage::getTest(int n) const;

private:
    bool readXML_Test(const QString& path);
};

#endif // __TESTSTORAGE__