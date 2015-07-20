#ifndef __VERSIONLINEEDIT__
#define __VERSIONLINEEDIT__

#include <QLineEdit>
#include <QIntValidator>

class VersionLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    VersionLineEdit(QWidget *parent = 0);
    ~VersionLineEdit();

private:
    QIntValidator m_intValidator;
};

#endif // __VERSIONLINEEDIT__
