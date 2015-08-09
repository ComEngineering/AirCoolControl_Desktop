#include "TestStorage.h"
#include "Logger.h"
#include "Configurator.h"
#include <qdiriterator.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

TestStorage::TestStorage()
{
    QString testsPath = Configurator::getTestFilesPath();
    QDirIterator iter(testsPath, QStringList() << "*.xml", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);

    while (iter.hasNext())
    {
        iter.next();
        QString xmlFilePath = iter.filePath();
        if (!readXML_Test(xmlFilePath))
            Logger::log("Invalid config file - " + xmlFilePath.toStdString(), Logger::Error);;
    }
}


TestStorage::~TestStorage()
{
}

bool TestStorage::readXML_Test(const QString& path)
{
    boost::property_tree::ptree tree;
    try
    {
        boost::property_tree::read_xml(path.toStdString(), tree, boost::property_tree::xml_parser::no_comments);

        std::string name = tree.get<std::string>("Test.<xmlattr>.name");
        std::string description = tree.get<std::string>("Test.Description");
        SimpleTestShared a_test = std::make_shared<SimpleTest>(name, description);

        boost::property_tree::ptree stages = tree.get_child("Test.stages", boost::property_tree::ptree());
        for (const std::pair<std::string, boost::property_tree::ptree> &a_stage_tree : stages)
        {
            SimpleTest::Stage a_stage;
            a_stage.m_duration = a_stage_tree.second.get<int>("duration");
            
            a_test->addStage(a_stage);
        }

        push_back(a_test);
    }
    catch (boost::property_tree::xml_parser_error& err)
    {
        Logger::log(err.message(), Logger::Error);
        return false;
    }
    catch (boost::property_tree::ptree_bad_data& err)
    {
        Logger::log(err.what(), Logger::Error);
        return false;
    }
    catch (boost::property_tree::ptree_bad_path& err)
    {
        Logger::log(err.what(), Logger::Error);
        return false;
    }

    return true;
}

std::vector<std::string> TestStorage::getNames(void) const
{
    std::vector<std::string> rc;
    for (const auto a_test : *this)
    {
        rc.emplace_back(a_test->getName());
    }
    return rc;
}

SimpleTestShared TestStorage::getTest(int n) const
{
    std::list<SimpleTestShared>::const_iterator it;
    for (it = begin(); it != end(); it++)
        if (n-- == 0)
        {
            return (*it);
        }
    return SimpleTestShared();
}