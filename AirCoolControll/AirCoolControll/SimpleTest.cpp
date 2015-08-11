#include "SimpleTest.h"
#include "Configurator.h"
#include <assert.h>
#include <algorithm>

SimpleTest::SimpleTest(const std::string& name, const std::string& descripton, ConfigMapShared config) :
m_description(descripton),
m_name(name),
m_config(config)
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

int SimpleTest::getStageStartTime(int n) const
{
    int rc = 0;
    n = std::min(size_t(n), m_list.size());
    for (int i = 0; i < n; i++)
    {
        rc += m_list[i].m_duration;
    }
    return rc;
}

SimpleTest::Stage* SimpleTest::stageWithIndex(uint n)
{
    assert(n < m_list.size());
    return &m_list[n];
}

int SimpleTest::getStageWithTime(int time) const
{
    int currentTime = 0;
    for (int i = 0; i < m_list.size(); i++)
    {
        currentTime += m_list[i].m_duration;
        if (time < currentTime)
        {
            return i;
        }
    }
    return m_list.size() - 1;
}