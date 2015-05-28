#ifdef _WIN32
#pragma warning(disable:4996)
#define  _D_SCL_SECURE_NO_WARNINGS
#endif

#include "VersionStorage.h"
#include <algorithm> 
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <qtextstream.h>


VersionStorage::VersionStorage(QString versionString)
{
    std::remove_if(versionString.begin(), versionString.end(), versionFilter);
    std::vector<std::string> strs;
    std::string versionSTD(versionString.toStdString());
    boost::split(strs,versionSTD, boost::is_any_of(".,"));
    if (strs.size() != 3)
    {
        m_isLegal = false;
        return;
    }
    m_major = stoi(strs[0]);
    m_minor = stoi(strs[1]);
    m_revision = stoi(strs[2]);

    m_isLegal = true;
}


VersionStorage::~VersionStorage()
{
}

bool VersionStorage::versionFilter(QChar c)
{
    return !(c == '.' || (c >= '0' && c <= '9'));
}

bool VersionStorage::operator<=(const VersionStorage& other) const 
{
    if (m_major > other.m_major)
        return false;
    else if (m_major < other.m_major)
        return true;

    if (m_minor > other.m_minor)
        return false;
    else if (m_minor < other.m_minor)
        return true;

    if (m_revision > other.m_revision)
        return false;
   return true;
}

bool VersionStorage::operator==(const VersionStorage& other) const
{
    return m_major == other.m_major && m_minor == other.m_minor && m_revision == other.m_revision;
}

VersionStorage::operator QString() const
{
    QString rc;
    if (m_isLegal)
    {
        QTextStream ss(&rc);
        ss << 'V' << m_major << '.' << m_minor << '.' << m_revision;
    }
    return rc;
}