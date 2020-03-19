#include "BlackScholesCallOption.h"
#include "BlackScholesPrice.h"
#include <vector>

BSCallOption::BSCallOption(BlackScholesModel& BsModel_,CallOption& TheCallOption_) : BsModel(BsModel_), TheCallOption(TheCallOption_)
{
}

double BSCallOption::getPrice() const
{
	return BlackScholes(BsModel, TheCallOption);
}

ClosedformOption* BSCallOption::clone() const
{
	return new BSCallOption(*this);
}

