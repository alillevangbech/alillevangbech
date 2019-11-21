#include <math.h>
#include "gaussians.h"

double iVolSABR(double Forward0, double sigma0, double mat, double strike, double alpha, double beta, double rho)
{
	double spot;
	if (strike == Forward0)
	{
		spot = Forward0 + 0.001;
	}
	else
	{
		spot = Forward0;
	}
	if (rho == 1)
	{
		rho = rho - 0.0001;
	}
	if (beta == 1)
	{
		beta = beta - 0.0001;
	}

	double Fmid = sqrt(spot*strike);
	double CFmid = pow(spot, beta);
	double epsilon = mat*alpha*alpha;
	double zeta = alpha / (sigma0*(1 - beta))*(pow(spot, 1 - beta) - pow(strike, 1 - beta));
	double Dzeta = log((sqrt(1 - 2 * rho*zeta + pow(zeta, 2)) + zeta - rho) / (1 - rho));
	double gamma1 = beta / Fmid;
	double gamma2 = -(beta*(1 - beta)) / pow(Fmid, 2);
	double IVterm1 = (alpha*log(spot / strike)) / Dzeta;
	double IVterm2 = ((2 * gamma2 - pow(gamma1, 2) + (1 / pow(Fmid, 2))) / 24)*pow(sigma0*CFmid / alpha, 2);
	double IVterm3 = (rho*gamma1 / 4)*(sigma0*CFmid / alpha) + ((2 - 3 * pow(rho, 2)) / 24);

	return IVterm1*(1 + (IVterm2 + IVterm3)*epsilon);
}