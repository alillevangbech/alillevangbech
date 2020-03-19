#include <limits>
#include "gaussians.h"

inline double BlackScholes(double spot, double vol, double mat, double strike)
{
	double std = sqrt(mat) * vol;
	double halfVar = 0.5 * std * std;
	double d1 = (log(spot / strike) + halfVar) / std;
	double d2 = d1 - std;
	return spot * normalCdf(d1) - strike * normalCdf(d2);
}

inline double Bachelier(double spot, double vol, double mat, double strike) {
	double std = sqrt(mat) * vol;
	double d = (spot - strike) / std;
	return (spot - strike)*normalCdf(d) + std*normalDens(d);
}

inline double CEV(double spot, double vol, double mat, double strike, double beta)
{
	double a, b;
	if (beta == 0)
	{
		a = 9999999999;
		b = vol / a;
	}
	else
	{
		a = ((1.0 - beta) / (beta))*spot;
		b = vol*beta*pow(spot, beta - 1.0);
	}
	return BlackScholes(spot + a, b, mat, strike + a);
}