#include <math.h>
#include "blackScholes.h"

int factorial(int n) {
	int fact=1;
	if (n >= 1) {
		for (int i = 1; i <= n; i++) {
			fact *= i;
		}
	}
	else {
		fact = 1;
	}
	return fact;
}

double Merton(double spot, double vol, double mat, double strike, double lambda, double m, double v, double nJumps) {
	double comp = lambda*m;
	double S;
	double sigma;
	double Finalresult=0.0;
	for (int n = 0; n <= nJumps; n++) {
		
S = spot*exp(n*(m + v / 2) - comp*mat);
sigma = sqrt(vol*vol + n*v / mat);
Finalresult += blackScholes(S, sigma, mat, strike)*exp(-lambda*mat) / factorial(n) * pow(lambda*mat, n);
	}
	return Finalresult;
}

