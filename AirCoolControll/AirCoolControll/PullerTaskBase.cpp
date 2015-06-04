#include "PullerTaskBase.h"


PullerTaskBase::PullerTaskBase(int id) :
    m_id(id),
    m_failCounter(0)
{
}

PullerTaskBase::~PullerTaskBase()
{
}

int  PullerTaskBase::getID() const
{
    return m_id;
}

