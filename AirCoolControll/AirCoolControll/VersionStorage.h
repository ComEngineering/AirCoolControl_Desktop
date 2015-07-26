#ifndef __VERSIONSTORAGE__
#define __VERSIONSTORAGE__

#include <qstring.h>

class VersionStorage
{
public:
    VersionStorage(QString versionString);
    VersionStorage() : m_isLegal(false), m_major(0), m_minor(0), m_revision(0) {}
    ~VersionStorage();

    bool isLegal(){ return m_isLegal; }
    bool operator<=(const VersionStorage& other) const;
    bool operator>=(const VersionStorage& other) const { return !operator<=(other); }
    bool operator==(const VersionStorage& other) const;
    explicit operator QString() const;

    friend class VersionEditWidget;

private:
    static bool versionFilter(QChar c);

private:
    int  m_major;
    int  m_minor;
    int  m_revision;
    bool m_isLegal;
};

#endif //__VERSIONSTORAGE__