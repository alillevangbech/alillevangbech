
#include "Vanilla3.h"
#include "msrng.h"
#include "ClosedformOption.h"

double SimpleMC(const VanillaOption& theOption,
						 MSRNG& theEngine,
                         double Spot,
                         double Vol,
                         double r,
                         unsigned long NumberOfPaths);

double SimpleMC(const VanillaOption& theOption,
	MSRNG& theEngine,
	double Spot,
	double Vol,
	double r,
	unsigned long NumberOfPaths,
	double relativeError,
	ClosedformOption& TheClosedformOption);

