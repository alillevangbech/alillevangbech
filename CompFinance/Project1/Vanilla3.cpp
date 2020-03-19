// Works of a Wrapper instead of a payoffbridge.
#include "Vanilla3.h"
//#include <iostream> // If wannt know when construction is called.

// Changed to input wrapper
VanillaOption::VanillaOption(const Wrapper<PayOff>& ThePayOff_, double Expiry_) : ThePayOff(ThePayOff_), Expiry(Expiry_)
{
}

double VanillaOption::GetExpiry() const
{
    return Expiry;
}

double VanillaOption::OptionPayoff(double Spot) const
{
    return (*ThePayOff)(Spot); // Slightly different syntax. It works as a pointer, i.e. is dereferenced and returns the dereferenced inner pointer. Compare the vanilla before the bridge.
}

