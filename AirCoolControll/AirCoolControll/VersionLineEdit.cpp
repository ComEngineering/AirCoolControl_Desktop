#include "VersionLineEdit.h"
#include <QIntValidator>

VersionLineEdit::VersionLineEdit(QWidget *parent)
    : QLineEdit(parent),
    m_intValidator(this)
{
    m_intValidator.setBottom(0);
    m_intValidator.setTop(9999);
    setMaxLength(4);
    setValidator(&m_intValidator);
    setAlignment(Qt::AlignVCenter | Qt::AlignRight);
}

VersionLineEdit::~VersionLineEdit()
{

}