#ifndef __SIMPLETEST__
#define __SIMPLETEST__

#include <qstring.h>
#include <memory>
#include <string>
#include <vector>
#include "ConfigMap.h"

class SimpleTest
{
public:
    SimpleTest(const std::string& name, const std::string& descripton, ConfigMapShared config);
    SimpleTest(){}
    ~SimpleTest();

    friend class TestEditDialog;

    struct Stage
    {
        int m_duration;
    };
    typedef std::vector<Stage> StageList;

    void addStage(const Stage& a_stage);
    void deleteStage(int n);
    int  stagesAmount()const;
    void saveToFile(const std::string& path)const; 
    void save(void) const;
    void setFilePath(const std::string& path) { m_filePath = path; }
    const std::string& getName(){ return m_name; }
    void setName(const std::string& name){ m_name = name; }

    int getOverallDuraton(void) const;
    Stage* stageWithIndex(uint n);
       
private:
    std::string     m_name;
    std::string     m_filePath;
    std::string     m_description;
    StageList       m_list;
    ConfigMapShared m_config;
};

typedef std::shared_ptr<SimpleTest> SimpleTestShared;

#endif //__SIMPLETEST__