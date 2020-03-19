// Works of a Wrapper instead of a payoffbridge.


#ifndef Vanilla_h
#define Vanilla_h

#include "PayOff6.h"
#include "Wrapper.h"

class VanillaOption
{
public:
    VanillaOption(const Wrapper<PayOff>& ThePayOff_, double Expiry_); // Constructor now takes Wrapper.

    double GetExpiry() const; 
    double OptionPayoff(double Spot) const;
  
private:
    double Expiry;
    Wrapper<PayOff> ThePayOff; // Holds a wrapped pointer instead.
};

#endif
