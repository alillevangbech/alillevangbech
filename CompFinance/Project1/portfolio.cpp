#include "portfolio.h"
portfolio::portfolio(size_t dim_, std::vector<ClosedformOption*>& OptionValueVector_, std::vector<double> weights_)
	: dim(dim_), OptionValueVector(OptionValueVector_), weights(weights_)
{
}

portfolio::~portfolio()
{
	delete theClosedformOptionPtr;
}

double portfolio::getPrice()
{
	double PortfolioValue = 0;
	for (unsigned int i = 0; i < dim; i++) {
		theClosedformOptionPtr = OptionValueVector[i]->clone();
		PortfolioValue += (theClosedformOptionPtr->getPrice())*weights[i];
	}
	return PortfolioValue;
}