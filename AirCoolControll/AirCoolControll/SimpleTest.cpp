#include "SimpleTest.h"
#include "Configurator.h"
#include <assert.h>

SimpleTest::SimpleTest(const std::string& name, const std::string& descripton) :
m_description(descripton),
m_name(name)
{
}


SimpleTest::~SimpleTest()
{
}

void SimpleTest::addStage(const Stage& a_stage)
{
    m_list.emplace_back(a_stage);
}

void SimpleTest::deleteStage(int n)
{
    if (n < 0 || n >= m_list.size())
        return;

    StageList::const_iterator it = m_list.begin();
    while (n)
    {
        it++;
        n--;
    }
    m_list.erase(it);
}

int  SimpleTest::stagesAmount()const
{
    return m_list.size();
}

void SimpleTest::saveToFile(const std::string& path)const
{

}

void SimpleTest::save(void) const
{
    QString path = Configurator::getTestFilesPath() + QString::fromStdString(m_filePath) + ".xml";
    saveToFile(path.toStdString());
}

int SimpleTest::getOverallDuraton(void) const
{
    int rc = 0;
    for (const Stage& i : m_list)
        rc += i.m_duration;
    return rc;
}

SimpleTest::Stage* SimpleTest::stageWithIndex(uint n)
{
    assert(n < m_list.size());
    return &m_list[n];
}