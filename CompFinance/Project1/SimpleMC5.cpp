// Unchanged

#include "SimpleMC5.h"
#include <cmath>        // exp, sqrt
#include "msrng.h"
#include <vector>

double SimpleMC(const VanillaOption& theOption,
						MSRNG& theEngine,
                         double Spot,
                         double Vol,
                         double r,
                         unsigned long NumberOfPaths)
{
    
    double Expiry = theOption.GetExpiry();
    
    double variance = Vol*Vol*Expiry;
    double rootVariance = sqrt(variance);
    double itoCorrection = -0.5*variance;
    
    double movedSpot = Spot*exp(r*Expiry +itoCorrection);
    double runningSum=0;
    
    for (unsigned long i=0; i < NumberOfPaths; i++)
    {
        double thisSpot  =movedSpot*exp(rootVariance*theEngine.genNormal()); //Use my genNormal method instead of OneGaussian
        double thisPayOff = theOption.OptionPayoff(thisSpot);
        runningSum += thisPayOff;
    }
    double mean = runningSum / NumberOfPaths;
    mean *= exp(-r*Expiry);
    return mean;
}


double SimpleMC(const VanillaOption& theOption,
		MSRNG& theEngine,
		double Spot,
		double Vol,
		double r,
		unsigned long NumberOfPaths,
		double relativeError,
		ClosedformOption& TheClosedformOption)
{

	double Expiry = theOption.GetExpiry();
	double variance = Vol*Vol*Expiry;
	double rootVariance = sqrt(variance);
	double itoCorrection = -0.5*variance;
	double movedSpot = Spot*exp(r*Expiry + itoCorrection);
	double runningSum = 0;
	double target = TheClosedformOption.getPrice();
	double mean = 0;
	unsigned long i = 0;
	while (fabs(1-mean/target) > relativeError)
	{
		double thisSpot = movedSpot*exp(rootVariance*theEngine.genNormal());
		double thisPayOff = theOption.OptionPayoff(thisSpot);
		runningSum += thisPayOff;
		mean = runningSum / i;
		mean *= exp(-r*Expiry);
		i++;
	}


	double error = fabs(1 - mean / target);
	return error;
}
