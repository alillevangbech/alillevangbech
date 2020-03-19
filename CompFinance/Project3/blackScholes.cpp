#include <math.h>
#include "gaussians.h"

double blackScholes(double spot, double vol, double mat, double strike)
{
	double std = sqrt(mat)*vol;
	double halfVar = 0.5 * std*std;
	double d1 = (log(spot / strike) + halfVar) / std;
	double d2 = d1 - std;
	return spot * normalCdf(d1) - strike*normalCdf(d2);
}



