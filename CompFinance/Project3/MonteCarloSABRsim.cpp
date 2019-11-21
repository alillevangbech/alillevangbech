#include <math.h>
#include <stdlib.h>
#include "MonteCarloSABRsim.h"
#include "gaussians.h"


double MonteCarloSABRsim(double spot, double strike, double mat, double alpha, double rho, double beta, double sigma0, double nPaths, double nSteps)
{

	srand(54321);

	double dt = mat / nSteps;
	double vol;
	double S;
	double result = 0;
	double payoff;
	double Ns;
	double Nvol;
	double N2;

	for (int n = 0; n < nPaths; n++)
	{
		vol = sigma0;
		S = spot;

		for (int i = 0; i < nSteps; i++)
		{	

Nvol = invNormalCdf((double)(rand() + 1) / (RAND_MAX + 2));
N2 = invNormalCdf((double)(rand() + 1) / (RAND_MAX + 2));
Ns = rho*Nvol + sqrt(1.0 - rho*rho)*N2;

S = S + vol*pow(S, beta)*sqrt(dt)*Ns;
vol = vol*exp(-0.5*alpha*alpha*dt + alpha*sqrt(dt)*Nvol);
			
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


