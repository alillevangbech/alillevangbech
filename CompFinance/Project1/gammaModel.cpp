#include "GammaModel.h"


gammaModel::gammaModel(double alpha_, double beta_) : alpha(alpha_), beta(beta_)
{
}

double gammaModel::getAlpha()
{
	return alpha;
}

double gammaModel::getBeta()
{
	return beta;
}
