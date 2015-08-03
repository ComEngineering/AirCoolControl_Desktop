#ifdef _WIN32
#pragma warning(disable:4996)
#define  _D_SCL_SECURE_NO_WARNINGS
#endif

#include "ConfigStorage.h"
#include <qdiriterator.h>
#include "Configurator.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "Logger.h"
#include <climits>

ConfigStorage::ConfigStorage()
{
    QString configsPath = Configurator::getConfigFilesPath();
    QDirIterator iter(configsPath, QStringList() << "*.xml", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    int configsRed = 0;
    while (iter.hasNext())
    {
        iter.next();
        QString xmlFilePath = iter.filePath();
        if (readXMLConfig(xmlFilePath))
            configsRed++;
    }
}


ConfigStorage::~ConfigStorage()
{
}


bool ConfigStorage::readXMLConfig(const QString& path)
{
    boost::property_tree::ptree tree;
    try
    {
        boost::property_tree::read_xml(path.toStdString(), tree, boost::property_tree::xml_parser::no_comments);
        std::string configName = tree.get<std::string>("Config.<xmlattr>.name");
        std::string vendor = tree.get<std::string>("Config.Vendor");
        std::string product = tree.get<std::string>("Config.Product");
        std::string versionMin = tree.get<std::string>("Config.Version.min");
        std::string versionMax = tree.get<std::string>("Config.Version.max");
        std::shared_ptr<ConfigMap> a_map = std::make_shared<ConfigMap>(path.toStdString(), configName, vendor, product, versionMin, versionMax);

        std::string uiType = tree.get<std::string>("Config.UI.<xmlattr>.type", "none");
        if ("none" != uiType)
        {
            std::string ui_path = tree.get<std::string>("Config.UI");
            a_map->setUI_Config(uiType, ui_path);
        }

        boost::property_tree::ptree values[ConfigMap::REGISTER_PULL_COUNT] = {
            tree.get_child("Config.InputValues", boost::property_tree::ptree()),
            tree.get_child("Config.OutValues", boost::property_tree::ptree()),
            tree.get_child("Config.Coils", boost::property_tree::ptree())
        };

        for (ConfigMap::RegisterType i = ConfigMap::REGISTER_PULL_FIRST; i < ConfigMap::REGISTER_PULL_COUNT; i = ConfigMap::NEXT(i))
        {
            boost::property_tree::ptree& a_value = values[i];
            for (const std::pair<std::string, boost::property_tree::ptree> &p : a_value)
            {
                std::string name = p.first;
                ConfigMap::Parameter a_parameter;
                a_parameter.m_type = i;
                a_parameter.m_description = p.second.get_value<std::string>();
                a_parameter.m_registerNumber = p.second.get<int>("<xmlattr>.R");
                a_map->getInterval(i).add(a_parameter.m_registerNumber);
                int b = p.second.get<int>("<xmlattr>.B", -1);
                a_parameter.m_decodeMethod = p.second.get<std::string>("<xmlattr>.D", "");
                if (b != -1)
                {
                    a_parameter.m_isBool = true;
                    a_parameter.m_bitNumber = b;
                }
                else
                {
                    a_parameter.m_isBool = false;
                }

                a_parameter.m_minValue = p.second.get<int>("<xmlattr>.min", INT_MIN);
                a_parameter.m_maxValue = p.second.get<int>("<xmlattr>.max", INT_MAX);

                boost::property_tree::ptree errorDetectionSection = p.second.get_child("errors", boost::property_tree::ptree());
                for (const std::pair<std::string, boost::property_tree::ptree> &a_error : errorDetectionSection)
                {
                    std::string type = a_error.first;
                    std::string description = a_error.second.get<std::string>("<xmlattr>.d", "error");
                    int value = a_error.second.get<int>("<xmlattr>.v"); //mandatory
                    a_parameter.m_errorDetector.addError(type, description, value);
                }

                boost::property_tree::ptree enumSection = p.second.get_child("enum", boost::property_tree::ptree());
                for (const std::pair<std::string, boost::property_tree::ptree> &a_item : enumSection)
                {
                    std::string description = a_item.second.get<std::string>("<xmlattr>.d", "error");
                    int value = a_item.second.get<int>("<xmlattr>.v"); //mandatory
                    a_parameter.m_enumeration.push_back(std::pair<std::string,int>(description,value));
                }

                a_map->addVariable(name, a_parameter);
            }
        }

        push_back(a_map);
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
