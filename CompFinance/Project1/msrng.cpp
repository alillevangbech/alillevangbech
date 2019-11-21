#include "msrng.h"
#include "gaussian_header.h"
#include "Gamma_header.h"
#include <vector>
#include <algorithm>


MSRNG::MSRNG(unsigned int seed_) : seed(seed_)
{
}

MSRNG::MSRNG() : seed(1)
{
}

MSRNG::MSRNG(unsigned int seed_, size_t dim_) : seed(seed_), dim(dim_)
{
}

MSRNG::MSRNG(std::minstd_rand engine_) : Engine(engine_)
{
}

MSRNG::MSRNG(const MSRNG& original) // This could probably have been done with some pointers
{									// but this works as well
	Engine = original.Engine;
	seed = original.seed;
	EngineState = original.EngineState;
	dim = original.dim;
}

MSRNG::~MSRNG()
{

}


double MSRNG::genUniform()
{
	return (Engine()) / ((double)Engine.max() + 1.0); // 1.0 to avoid 1's
}

double MSRNG::genUniform(double LowerBound, double UpperBound)
{
	double temp = (Engine()) / ((double)Engine.max());
	return ((UpperBound-LowerBound)*temp + LowerBound);

}

std::vector<double> MSRNG::genUniform(std::vector<double> InputVector)
{

	InputVector.clear(); //Clears the vector and sets it to size 0
						 // http://www.cplusplus.com/reference/vector/vector/clear/
						 // makes it perfect for using push_back afterwards.
	

		for (size_t i = 0; i < dim; i++)
		{
			InputVector.push_back(genUniform());   //Loops with dim - and push_back to input elemnts to the vector
		}

		return InputVector;

}


double MSRNG::genNormal()
{
	double u = genUniform();
	return invNormalCdf(u);
}

double MSRNG::genNormal(double mean, double sd)
{
	double stdnormal = invNormalCdf(genUniform());
	return (mean + stdnormal*sd);
	// X ~ N(0,1) --> Y = sigma*X + mu ~ N(mu,sigma^2)
}

std::vector<double> MSRNG::genNormal(std::vector<double> InputVector)
{

	InputVector.clear();
	//Clears the vector and sets it to size 0
	// http://www.cplusplus.com/reference/vector/vector/clear/
	// makes it perfect for using push_back afterwards.
	


		for (size_t i = 0; i < dim; i++)
		{
			InputVector.push_back(genNormal()); //Loops with dim - and push_back to input elemnts to the vector
		}

		return InputVector;


}


double MSRNG::genGamma(double alpha, double beta)
{

	double u = genUniform();
	return invgammp(u, alpha) / beta;
	
}


std::vector<double> MSRNG::genGamma(std::vector<double> InputVector, double alpha, double beta)
{
	InputVector.clear();
	

	for (size_t i = 0; i < dim; i++)
	{
		InputVector.push_back(genGamma(alpha, beta));
	}
	
	return InputVector;

}



void MSRNG::setSeed(unsigned int NewSeed)
{
	EngineState = 0;
	seed = NewSeed;
	Engine.seed(seed);
	
}


unsigned int MSRNG::operator() ()
{
	EngineState += 1;
	return Engine();
}

unsigned int MSRNG::GetState()
{
	unsigned int temp = EngineState;
	setSeed(seed);

	if (temp - 1 < 0)
	{
		return 0;
	}
	else
	{
		Engine.discard(temp - 1);
		return Engine();
	}
}

size_t MSRNG::getDim()
{
	return dim;
}