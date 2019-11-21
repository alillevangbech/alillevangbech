#include <cmath>
#include "HestonMC.h"
#include "gaussian_header.h"
#include <vector>
#include "max.h"



double MontoCarloHeston(MSRNG& tempEngine, double spot, double strike, double mat,double r, double theta, double kappa, double sigma, double v0, double rho, unsigned int nPaths, unsigned int nSteps)
{
	double dt = mat / nSteps;
	double vol;
	double logS,S;
	double result = 0;
	double payoff;
	std::vector<double> N1;
	std::vector<double> N2;
	double Ns;
	double maxvol;


	for (size_t n = 0; n < nPaths; n++)
	{
		vol = v0;
		logS = log(spot);

		N1 = tempEngine.genNormal(N1);
		N2 = tempEngine.genNormal(N2);

		for (size_t i = 0; i < nSteps; i++)
		{
			Ns = rho*N1[i] + sqrt(1.0 - rho*rho)*N2[i];
			maxvol = max(vol, 0.0);
			logS = logS + (r -0.5*	maxvol)*dt + sqrt(maxvol*dt)*N1[i];
			vol = vol + kappa*(theta - maxvol)*dt + sigma*sqrt(maxvol*dt)*Ns;
	 }		

		S = exp(logS);

		if (S > strike)
		{
			payoff = S - strike;
		}
		else
		{
			payoff = 0;
		}

		result = result + payoff;

	}

	result *= exp(-r*mat);

	return (result / nPaths);




}

double MontoCarloHeston(MSRNG& tempEngine, HestonModel& HestonModel, double spot, double strike, double mat, double r, unsigned int nPaths, unsigned int nSteps)
{

	double kappa = HestonModel.getKappa();
	double theta = HestonModel.getTheta();
	double sigma = HestonModel.getSigma();
	double v0 = HestonModel.getV0();
	double rho = HestonModel.getRho();

	double dt = mat / nSteps;
	double vol;
	double logS, S;
	double result = 0;
	double payoff;
	std::vector<double> N1;
	std::vector<double> N2;
	double Ns;
	double maxvol;
	
	

	for (size_t n = 0; n < nPaths; n++)
	{
		vol = v0;
		logS = log(spot);

		N1 = tempEngine.genNormal(N1);
		N2 = tempEngine.genNormal(N2);

		for (size_t i = 0; i < nSteps; i++)
		{
			Ns = rho*N1[i] + sqrt(1.0 - rho*rho)*N2[i];
			maxvol = max(vol, 0.0);
			logS = logS + (r - 0.5*	maxvol)*dt + sqrt(maxvol*dt)*N1[i];
			vol = vol + kappa*(theta - maxvol)*dt + sigma*sqrt(maxvol*dt)*Ns;
		}

		S = exp(logS);

		if (S > strike)
		{
			payoff = S - strike;
		}
		else
		{
			payoff = 0;
		}

		result = result + payoff;

	}

	result *= exp(-r*mat);

	return (result / nPaths);
}