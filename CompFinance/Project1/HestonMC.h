#include "msrng.h"
#include "HestonModel.h"

double MontoCarloHeston(MSRNG& tempEngine, double spot, double strike, double mat, double r, double theta, double kappa, double sigma, double v0, double rho, unsigned int nPaths, unsigned int nSteps);
double MontoCarloHeston(MSRNG& tempEngine,HestonModel& HestonModel, double spot, double strike, double mat, double r, unsigned int nPaths, unsigned int nSteps);


