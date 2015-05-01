#ifndef IPLINEEDIT_H
#define IPLINEEDIT_H

#include <QLineEdit>

class IpLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    IpLineEdit(QWidget *parent = 0);
    ~IpLineEdit();
};

#endif // IPLINEEDIT_H
