#include <math.h>
#include <stdlib.h>
#include "gaussians.h"
#include "Merton.h"



double Mertonsim(double spot, double vol, double mat, double strike, double lambda, double m, double v, double nJumps, double nPaths, double nSteps) {

	srand(54321);
	double Mertont_deriv;
	double Mertonkk_deriv;
	double sigma;
	double dt = mat / nSteps;
	double result = 0.0;
	double dK;
	double payoff;
	double halfVar;
	double S;
	double epsilon = 0.0001;
	for (int n = 1; n < nPaths; n++) {
		S = spot;
		sigma = vol;
		for (int i = 1; i < nSteps; i++) {
			halfVar = 0.5*sigma*sigma;
			S = S*exp(-halfVar*dt + sigma * sqrt(dt)* invNormalCdf((double)(rand() + 1) / (RAND_MAX + 2)));
			dK = S / nPaths;
			Mertont_deriv = (Merton(spot, vol, dt*i + epsilon, S, lambda, m, v, nJumps) - Merton(spot, vol, i*dt - epsilon, S, lambda, m, v, nJumps)) / (2.0*epsilon);
			Mertonkk_deriv = (Merton(spot, vol, dt*i, S - dK, lambda, m, v, nJumps) - 2 * Merton(spot, vol, dt*i, S, lambda, m, v, nJumps) + Merton(spot, vol, dt*i, S + dK, lambda, m, v, nJumps)) / (dK*dK);
			sigma = sqrt((2.0 * Mertont_deriv / (S*S*Mertonkk_deriv)));
		}
		if (S > strike) {
			payoff = S - strike;
		}
		else {
			payoff = 0;
		}

		result = result + payoff;

	}

	return result / (nPaths+1);

}