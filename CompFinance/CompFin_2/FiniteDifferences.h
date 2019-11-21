#pragma once
#pragma warning(disable:4244)
#include <vector>
#include "LinearInterpolation.h"
#include "MatrixAlgebra.h"

inline double CNFiniteDifferencesCall(
	double StateMin, double StateMax,
	double StateSteps, double TimeSteps,
	double spot, double vol, double mat, double strike, double beta)
{

	StateSteps++;
	TimeSteps++;
	std::vector<double> StateVec(StateSteps), TimeVec(TimeSteps);
	size_t i, j;
	double StateIncrement, TimeIncrement;

	StateIncrement = (StateMax - StateMin) / (StateSteps - 1.0);
	//fill StateGrid
	for (i = 0; i < StateSteps; i++)
	{
		StateVec[i] = StateIncrement*i + StateMin;
	}

	//fill TimeGrid
	TimeIncrement = mat / (TimeSteps - 1.0);
	for (i = 0; i < TimeSteps; i++)
	{
		TimeVec[i] = TimeIncrement*i;
	}

	std::vector<double> CalcVec(StateSteps), PrevVec(StateSteps);
	std::vector<double> a(StateSteps), b(StateSteps), c(StateSteps), alpha(StateSteps), betaCoef(StateSteps), gamma(StateSteps);


	//fill PrevVec with terminal values
	//and fil
	double sigma;
	double theta = 0.5; //Crank-Nicholsen. 
	for (i = 0; i < StateSteps; i++) //the terminal timestep - so we start here.
	{
		sigma = pow(vol, 2)*pow(StateVec[i], 2 * beta) / pow(StateIncrement, 2.0);
		a[i] = (1 - theta)*0.5*(-sigma);
		b[i] = 1 / TimeIncrement + (1 - theta)*sigma;
		c[i] = (1 - theta)*0.5*(-sigma);
		alpha[i] = -theta*0.5*(-sigma);
		betaCoef[i] = (1 / TimeIncrement) - theta*sigma;
		gamma[i] = theta*0.5*sigma;
		//terminal option values
		PrevVec[i] = max(StateVec[i] - strike, 0.0);
	}

	//How do we proceed:
	//Now we have the coefficients. We need to add them together. 
	//the actual method - start from the end - CN method
	for (i = TimeSteps; i > 0; i--)
	{
		CalcVec[0] = betaCoef[0] * PrevVec[0] + gamma[0] * PrevVec[1];
		CalcVec[StateSteps - 1] = alpha[StateSteps - 1] * PrevVec[StateSteps - 2] + betaCoef[StateSteps - 1] * PrevVec[StateSteps - 1];
		for (j = 1; j < StateSteps - 1; j++) //d vector in munk
		{
			CalcVec[j] = alpha[j] * PrevVec[j - 1] + betaCoef[j] * PrevVec[j] + gamma[j] * PrevVec[j + 1];
		}
		PrevVec = tridag(a, b, c, CalcVec, PrevVec);
	}

	for (i = 0; i < StateSteps - 1; i++)
	{
		if (StateVec[i] <= spot && StateVec[i + 1] >= spot)
		{
			return LinearInterpolation(StateVec[i], PrevVec[i], StateVec[i + 1], PrevVec[i + 1], spot);
		}
	}
	return 0.0;
}
