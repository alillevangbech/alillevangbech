#include "GammaCallOption.h"
#include "Gamma_header.h"


GammaCallOption::GammaCallOption(gammaModel& GammaModel_, CallOption& TheCallOption_) : GammaModel(GammaModel_), TheCallOption(TheCallOption_)
{

}

double GammaCallOption::getPrice() const
{
	double alpha = GammaModel.getAlpha();
	double beta = GammaModel.getBeta();
	double strike = TheCallOption.getStrike();

	return (1 - gammaCDF(alpha, beta, strike));

}

ClosedformOption* GammaCallOption::clone() const
{
	return new GammaCallOption(*this);
}


