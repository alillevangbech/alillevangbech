// Unchanged

#include "PayOff6.h"
#include <iostream>
#include "max.h"


PayOffCall::PayOffCall(double Strike) : Strike(Strike)
{
}

double PayOffCall::operator () (double Spot) const
{
    return max(Spot-Strike,0.0);
}

PayOff* PayOffCall::clone() const
{
//    std::cout << "PayOffCall Clone \n";

    return new PayOffCall(*this);
}

PayOffPut::PayOffPut(double Strike) : Strike(Strike)
{
    
}

double PayOffPut::operator () (double Spot) const
{
    return max(Strike-Spot,0.0);
}

PayOff* PayOffPut::clone() const
{
    return new PayOffPut(*this);
}

