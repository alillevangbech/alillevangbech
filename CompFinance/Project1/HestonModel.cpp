#include "HestonModel.h"


HestonModel::HestonModel(double kappa_, double theta_, double sigma_, double v0_, double rho_) : kappa(kappa_), theta(theta_), sigma(sigma_), v0(v0_), rho(rho_)
{

}
	

double HestonModel::getKappa()
{
	return kappa;
}

double HestonModel::getTheta()
{
	return theta;
}
double HestonModel::getSigma()
{
	return sigma;
}
double HestonModel::getV0()
{
	return v0;
}
double HestonModel::getRho()
{
	return rho;
}

