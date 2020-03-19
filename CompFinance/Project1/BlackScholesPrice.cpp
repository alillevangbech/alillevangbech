
#include "BlackScholesPrice.h"
#include <math.h>
#include "gaussian_header.h"
#include "BlackScholesModel.h"
#include "CallOption.h"

double BlackScholes(double spot, double vol, double mat, double strike)
{
	double std = sqrt(mat)*vol;
	double halfVar = 0.5*std*std;
	double d1 = (log(spot / strike) + halfVar) / std;
	double d2 = d1 - std;
	
	return spot*normalCdf(d1) - strike * normalCdf(d2);
	//Since we are working with r = 0, then the last exp(-r*mat) cancels out
	//to be more correct I should probably implement it
	//however since in the Q2P3 problem with BS model, we are not required to include interest rate
	//I choose to leave it out here as well

}

double BlackScholes(BlackScholesModel& BsModel, double mat, double strike)
{

	double spot = BsModel.getSpot();
	double vol = BsModel.getVol();

	double std = sqrt(mat)*vol;
	double halfVar = 0.5*std*std;
	double d1 = (log(spot / strike) + halfVar) / std;
	double d2 = d1 - std;

	return spot*normalCdf(d1) - strike * normalCdf(d2);
	//Since we are working with r = 0, then the last exp(-r*mat) cancels out

}



double BlackScholes(BlackScholesModel& BsModel, CallOption& TheCallOption)
{

	double spot = BsModel.getSpot();
	double vol = BsModel.getVol();
	double mat = TheCallOption.getExpiry();
	double strike = TheCallOption.getStrike();

	double std = sqrt(mat)*vol;
	double halfVar = 0.5*std*std;
	double d1 = (log(spot / strike) + halfVar) / std;
	double d2 = d1 - std;

	return spot*normalCdf(d1) - strike * normalCdf(d2);
	//Since we are working with r = 0, then the last exp(-r*mat) cancels out

}

