#include "PullerTaskBase.h"


PullerTaskBase::PullerTaskBase(int id) :
    m_id(id)
{
}

PullerTaskBase::~PullerTaskBase()
{
}

int  PullerTaskBase::getID() const
{
    return m_id;
}

