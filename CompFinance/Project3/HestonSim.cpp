#include "gaussians.h"
#include <math.h>
#include "HestonSim.h"



double MontoCarloHeston(double spot, double strike, double mat, double lambda, double eps, double MeanRv, double rho, double nPaths, double nSteps)
{

	srand(54321);

	double dt = mat / nSteps;
	double vol;
	double S;
	double result = 0;
	double payoff;
	double tempx;
	
	double tempvar;
	double Ns;
	double Nvol;
	double N2;


	for (int n = 0; n < nPaths; n++)
	{
		vol = 1;
		S = spot;

		for (int i = 0; i < nSteps; i++)
		{
			Nvol = invNormalCdf((double)(rand() + 1) / (RAND_MAX + 2));
			N2 = invNormalCdf((double)(rand() + 1) / (RAND_MAX + 2));
			Ns = rho*Nvol + sqrt(1.0 - rho*rho)*N2;

			S = S*exp(-0.5 * pow(lambda, 2)*vol*dt + lambda*sqrt(vol)*sqrt(dt)*Ns);

			tempx = 1.0 + (vol - 1.0)*exp(-MeanRv*dt);
			tempvar = log(vol*eps*eps / (MeanRv*pow(tempx, 2))*(exp(-MeanRv*dt) - exp(-2 * MeanRv*dt)) + eps*eps / (2 * MeanRv*pow(tempx, 2))*pow((1 - exp(-MeanRv*dt)), 2) + 1);	
			vol = tempx*exp(-0.5 * tempvar + sqrt(tempvar)*Nvol);


		}


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

	return result / nPaths;




}

