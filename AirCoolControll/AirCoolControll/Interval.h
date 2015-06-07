#ifndef __INTERVAL__
#define __INTERVAL__

#include <utility>

class Interval :
    public std::pair < int, int >
{
public:
    Interval();

    void add(int);
    bool in(int) const;
    bool empty(void) const;
};

#endif // __INTERVAL__

