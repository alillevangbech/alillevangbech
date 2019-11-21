#include <math.h>
#include "gaussians.h"
#include "bsGreeksCall.h"

// Bachelier at time zero

double bachelier(double spot, double strike, double vol, double mat) {
	double std = sqrt(mat) * vol;
	double d = (spot - strike) / std;
	return (spot - strike)*normalCdf(d) + std*NormalDF(d, 0, 1);
}


double vasBachelier(double spot, double strike, double vol, double mat, double MR) {
	double std = sqrt(-(vol*vol / (2 * MR))*(exp(-2 * MR*mat) - 1));
	double d = (spot - strike) / std;
	return (spot - strike) * normalCdf(d) + std * NormalDF(d, 0, 1);
}