#include "RequestRouter.h"
#include "Logger.h"


RequestRouter::RequestRouter(const RouterMap& map) :
    m_map(map)
{
}

bool RequestRouter::routeRequest(std::istringstream& instream, const std::string& key) const
{
    boost::property_tree::ptree a_tree;

    try
    {
        boost::property_tree::read_xml(instream, a_tree);

        std::string command = a_tree.get<std::string>(key);

        if (command.empty())
            return false;

        if (m_map.find(command) == m_map.end())
        {
            Logger::log("Unsufisiend command", Logger::Error);
            return false;
        }

        const RouterProc proc = m_map.find(command)->second;
        proc(&a_tree.get_child("params"));
    }
    catch (const boost::property_tree::ptree_error &e)
    {
        Logger::log(e.what(), Logger::Error);
        return false;
    }

    return true;
}