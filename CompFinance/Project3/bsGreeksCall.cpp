#include <math.h>
#include "gaussians.h"
#include "bsGreeksCall.h"


// Define d1,d2



double bsDelta(double spot, double strike, double mat, double vol)
{

	double std = sqrt(mat)*vol;
	double halfVar = 0.5 * std*std;
	double d1 = (log(spot / strike) + halfVar) / std;

	return normalCdf(d1);
}

double bsGamma(double spot, double strike, double mat, double vol)
{

	double std = sqrt(mat)*vol;
	double halfVar = 0.5 * std*std;
	double d1 = (log(spot / strike) + halfVar) / std;

	return NormalDF(d1,0,1)/(spot*std);
}

double bsRho(double spot, double strike, double mat, double vol)
{

	double std = sqrt(mat)*vol;
	double halfVar = 0.5 * std*std;
	double d1 = (log(spot / strike) + halfVar) / std;
	double d2 = d1 - std;

	return strike*mat*normalCdf(d2);
}
double bsTheta(double spot, double strike, double mat, double vol)
{

	double std = sqrt(mat)*vol;
	double halfVar = 0.5 * std*std;
	double d1 = (log(spot / strike) + halfVar) / std;

	return -(spot*NormalDF(d1, 0, 1)*vol) / (2 * sqrt(mat));

}

double bsVega(double spot, double strike, double mat, double vol)
{

	double std = sqrt(mat)*vol;
	double halfVar = 0.5 * std*std;
	double d1 = (log(spot / strike) + halfVar) / std;

	return spot*NormalDF(d1, 0, 1)*sqrt(mat);

}


// Normal distribution function

double NormalDF(double x, double mean, double var)
{
	const double pi = 3.1415926535897;

	return 1 / (sqrt(2 * pi*var))*exp(-((x - mean)*(x - mean)) / (2 * var));

}














