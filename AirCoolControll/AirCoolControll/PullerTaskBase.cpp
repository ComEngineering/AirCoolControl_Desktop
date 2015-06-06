#include "PullerTaskBase.h"


PullerTaskBase::PullerTaskBase(int id) :
    m_id(id),
    m_failCounter(0),
    m_lastSuccessfullAttemptTime(boost::gregorian::date(1971, boost::gregorian::May, 3))
{
}

PullerTaskBase::~PullerTaskBase()
{
}

int  PullerTaskBase::getID() const
{
    return m_id;
}

