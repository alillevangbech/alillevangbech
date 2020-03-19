#include <math.h>
#include <stdlib.h>
#include "gaussians.h"
#include "simpleMontoCarlo.h"



double bsMontoCarlo(double spot, double vol, double mat, double strike, double nPaths, double nSteps)
{
	srand(54321);

	double dt = mat / nSteps;
	double std = vol * sqrt(dt);
	double halfVar = std*std*0.5;
	double logS0 = log(spot);
	double result = 0;
	double logS;
	double spotAtT;
	double payoff;

	for (int n = 1; n < nPaths; n++)
	{
		logS = logS0;

		for (int i = 1; i < nSteps; i++)
		{
			logS = logS - halfVar + std * invNormalCdf((double)(rand() + 1) / (RAND_MAX + 2));
		}

		spotAtT = exp(logS);
		if (spotAtT > strike)
		{
			payoff = spotAtT - strike;
		}
		else
		{
			payoff = 0;
		}

		result = result + payoff;

	}

	return result / nPaths;

}

