#include "CallOption.h"


CallOption::CallOption(double Expiry_, double Strike_) : Expiry(Expiry_), Strike(Strike_)
{
}

double CallOption::getExpiry()
{
	return Expiry;

}

double CallOption::getStrike()
{
	return Strike;
}