#pragma once
#include <random>
#include <vector>
//need to ifndf enddef like other files
class MSRNG
{
public:
	MSRNG(unsigned int seed); //initiates a class with a certain seed
	MSRNG(); //defaluts to seed 1
	MSRNG(unsigned int seed, size_t dim); //takes a dimension as well
	MSRNG(std::minstd_rand engine); // Constructor which takes another engine as input
	MSRNG(const MSRNG& original); // Copy Constructor
	~MSRNG(); // Destructor

	void setSeed(unsigned int NewSeed); //method for setting a seed for the engine. This means resetting the engine.

	double genUniform(); //Generate uniform(0,1) realization
	double genUniform(double LowerBound, double UpperBound);// Generate uniform(a, b) realization
	std::vector<double> genUniform(std::vector<double> InputVector); // Generate uniform(0,1) realization in vector form

	double genNormal(); // Generate N(0,1) realization
	double genNormal(double mean, double sd); // Generate N(mean,sd) realization
	std::vector<double> genNormal(std::vector<double> InputVector); // Generate N(0,1) realization in vector

	double genGamma(double alpha, double beta); // Generate Gamma(Alpha,Beta) realization
	std::vector<double> genGamma(std::vector<double> InputVector, double alpha, double beta); // Generate Gamma(Alpha,Beta) realization in vector
	
	 
	unsigned int GetState(); // gets the engine state
	unsigned int operator()(); // generate random number

	size_t getDim(); //get the dimension


	
private:
	std::minstd_rand Engine; // my engine
	unsigned int seed; // my seed
	unsigned int EngineState=0; //engine state
	size_t dim; //my dim
	



};