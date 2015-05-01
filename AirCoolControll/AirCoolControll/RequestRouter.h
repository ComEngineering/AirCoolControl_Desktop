#ifndef __REQUESTROUTER__
#define __REQUESTROUTER__

#include <functional>
#include <boost/property_tree/xml_parser.hpp>
#include <unordered_map>
#include <string>

class RequestRouter
{
public:
    typedef std::function<void(boost::property_tree::ptree*)> RouterProc;
    typedef std::unordered_map<std::string, RouterProc> RouterMap;
    typedef std::pair<std::string, RouterProc> RouterPair;

    RequestRouter(const RouterMap& map);
    bool routeRequest(std::istringstream& instream,const std::string& key) const;

private:
    const RouterMap&  m_map;
};

#endif //__REQUESTROUTER__

