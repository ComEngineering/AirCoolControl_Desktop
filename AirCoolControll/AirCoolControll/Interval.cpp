#include "Interval.h"


Interval::Interval() :
    std::pair<int,int>(0x8fff,0)
{
}

void Interval::add(int n)
{
    if (n > second)
        second = n;
    if (n < first)
        first = n;
}

bool Interval::in(int n) const
{
    return n >= first && n <= second;
}