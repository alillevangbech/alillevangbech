#pragma once

class HestonModel
{
public:
	HestonModel(double kappa, double theta, double sigma, double v0, double rho);

	double getKappa();
	double getTheta();
	double getSigma();
	double getV0();
	double getRho();

private:
	const double kappa;
	const double theta;
	const double sigma;
	const double v0;
	const double rho;
};