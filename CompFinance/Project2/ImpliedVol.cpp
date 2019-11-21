#include <math.h>
#include "AnalyticSolutions.h"
#include "gaussians.h"



double iVol(double spot, double price, double mat, double strike)
{

	double ub = 2;
	double lb = 0.0001;
	double mb = 0;
	double bs = 0;

	while (ub - lb > 0.0001)
	{
		mb = 0.5*(ub + lb);
		bs = BlackScholes(spot, mb, mat, strike);
		if (bs > price)
		{
			ub = mb;
		}
		else
		{
			lb = mb;
		}
	}

	return 0.5*(ub + lb);

}

