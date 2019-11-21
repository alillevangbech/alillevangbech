
// HandIn 1 by Alexander Lillevang Bech	
// Student id :  xrf930
// When developing the code I collaborated with Nikolaj Pedersen & Nicolaj Schou Andersen.

#include <iostream>
#include <random>
#include <cmath>
#include "msrng.h"
#include "SimpleMC5.h"
#include "PayOff6.h"
#include "Vanilla3.h"
#include "gaussian_header.h"
#include "BlackScholesPrice.h"
#include "BlackScholesModel.h"
#include "CallOption.h"
#include "BlackScholesCallOption.h"
#include <vector>
//#include "Gamma_header.h"
#include "HestonMC.h"
#include "Portfolio.h"
#include "GammaCallOption.h"
#include "HestonModel.h"



int main() {


// Question 1.
std::cout << "Question 1: \n";

    // Problem 1
    std::cout << "---  Q1.P1  --- \n";

	unsigned int seed = 1; // define a seed for the engine
	std::minstd_rand gen(seed); //inititate the newer minimum standard engine with seed 1

	for (int n = 0; n < 5; n++)
	{
		std::cout << "U(0,1) realization:\t" <<  (gen())/((double) gen.max() + 1.0) << "\n";  
						//Outputs 5 realizations of U(0,1)
						//max() takes the upper bound of possible random numbers of the engine
	}

	std::cout << "---  Q1.P2  --- \n";

	MSRNG engine(1); // initiates the minstd_rand engine with seed 1, as defined in my MSRNG class

	std::cout << "---  Q1.P3  --- \n";

	for (int n = 0; n < 5; n++)
	{
		std::cout << "U(0,1) realization with genUniform method:\t" << engine.genUniform() << "\n";
		//Outputs 5 realizations of U(0,1) using the genUniform method.
		//the object was initiated with seed 1 in the previous problem
	}
	
	std::cout << "---  Q1.P4  --- \n";

	engine.setSeed(1);
	std::cout << " generate random number with seed 1  :" << engine() << "\n";
								// I generated a random number by doing a operator() method, which takes the underlying 
								// engines operater() method for generating a random number.

	engine.setSeed(1);
	std::cout << " reset generator by using the setSeed method to 1 again  :" << engine() << "\n";
								// Generate numbers again.


	std::cout << "---  Q1.P5  --- \n";

		//answers in pdf file

	std::cout << "---  Q1.P6  --- \n";

	engine.setSeed(1);  // set seed to 1
	for (int n = 0; n < 5; n++)
	{
		std::cout << "Standard normal realization with genNormal:\t"  << engine.genNormal() << "\n";
		//Outputs 5 realizations of the standard normal distribution
		//It does this by first generating a U(0,1) realization by the genUniform method
		//Then it uses the invNormalCdf function implemented in the gaussian_header file
		//to return a standard normal realization.
	}


	std::cout << "---  Q1.P7  --- \n";

	engine.setSeed(1);
	for (int n = 0; n < 5; n++)
	{
		std::cout << "N(1,2) realizations with genNormal:\t" << engine.genNormal(1.0,2.0) << "\n";
		//Outputs 5 realizations of N(1,2)
		//It does this by first generating a U(0,1) realization by the genUniform method
		//Then it uses the invNormalCdf function implemented in the gaussian_header file
		//to return a standard normal realization.
		//then multiplies the standard deviation and plusses the mean
	}

	std::cout << "\n \n";

	engine.setSeed(1);
	for (int n = 0; n < 5; n++)
	{
		std::cout << "U(1,2) realizations with genNormal:\t" << engine.genUniform(1.0, 2.0) << "\n";
		//Outputs 5 realizations of the U(1,2)
		//For X ~ U(0,1) and Y = (b-a)*X+a then Y~U(a,b)
		//P(Y <= y) = P((b - a)*X + a <= y) = P(X <= (y - a) / (b - a)) = (y - a) / (b - a)
	}

	std::cout << "---  Q1.P8  --- \n";	

	engine.setSeed(1);
	// Call option and setting parameters
	double Spot = 100.0;
	double Expiry = 1.0;
	double Strike = 100.;
	double Vol = 0.1;
	double r = 0.04;
	unsigned long int NumberOfPaths = 10000;
	//Use relevant files from example 7
	PayOffCall CallPayoff(Strike);
	VanillaOption MyCallOption(CallPayoff, Expiry);
	//substitute my genNormal method with OneGaussian and pass my engine to the simpleMC.
	double CallOptionPrice = SimpleMC(MyCallOption, engine, Spot, Vol, r, NumberOfPaths);

	std::cout << "Call option price:\t" << CallOptionPrice << "\n";
	//outputs call option price

	std::cout << "---  Q1.P9  --- \n";

	engine.setSeed(1);
	for (int n = 0; n < 5; n++)
	{
		std::cout << "Random Numbers: \t" << engine() << "\n";
		//Outputs 5 random numbers
	}

	std::cout << "\n" <<  "GetState Method: \t" << engine.GetState() << "\n";
	
		//my GetState method:
		//add field variable named EngineState and keep track of every time I output a random number and say +1.
		//Then when i call my method i setSeed to restart the engine, and then discard() EngineState - 1
		//and then i can output my current state

	std::cout << "---  Q1.P10  --- \n";

	MSRNG defEngine;
	for (int n = 0; n < 5; n++)
	{
		std::cout << "U(0,1) realizations: \t" << defEngine.genUniform() << "\n";
		//Outputs 5 U(0,1) realizations with a MSRNG object constructed with no input
		//Should be setting the seed to 1 as default
	}
	

	std::cout << "---  Q1.P11  --- \n";

	std::minstd_rand InputEngine(10);
	MSRNG MSRNGConstructor(InputEngine); //use new constructor

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << "Printing uniforms: \t" << MSRNGConstructor.genUniform() << "\n";
		//generate uniforms with the new constructor and output
	}
	
	std::cout << "\n";
	MSRNGConstructor.setSeed(10);

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << "Printing uniforms after setting seed:\t" << MSRNGConstructor.genUniform() << "\n";
		//generate uniforms with the new constructor and output
	}


	std::cout << "---  Q1.P12  --- \n";

	//answers in pdf file

	std::cout << "---  Q1.P13  --- \n";

	MSRNG dimEngine(1, 5); //new constructor which takes seed and dimension as input.

	std::vector<double> MyNormals(dimEngine.getDim()); //initialized vector to hold normals
	MyNormals = dimEngine.genNormal(MyNormals); //generate normals

	for (auto& x : MyNormals) // use range based looping through vector because its so easy
	{
		std::cout << "Vector of normals: \t" << x << "\n";
	}

	std::cout << "\n";

	dimEngine.setSeed(1); //setseed

	std::vector<double> MyUniforms(dimEngine.getDim()); //same as before
	MyUniforms = dimEngine.genUniform(MyUniforms); 

	for (auto& x : MyUniforms) // use range based looping through vector because its so easy
	{
		std::cout << "Vector of uniforms: \t" << x << "\n";
	}

	std::cout << "---  Q1.P14  --- \n";

	dimEngine.setSeed(1);
	MSRNG CopyConstructor(dimEngine); //CopyConstructor

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << "Printing uniforms: \t" << CopyConstructor.genUniform() << "\n";
			//output 5 U(0,1) realizations
	}

	std::cout << "---  Q1.P15  --- \n";
	//answers in pdf

	std::cout << "---  Q1.P16  --- \n";

	engine.setSeed(1);
	//see implementation in "mrsng.h"
	std::cout << "Alpha_1 = 1.05, Beta_1 = 2 \n";
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "Output genGamma(alpha,beta): \t" << engine.genGamma(1.05, 2) << "\n";
		//output gamma realizations with alpha_1 and beta_1
	}
	
	engine.setSeed(1);
	std::cout << "\n Alpha_2 = 105, Beta_2 = 2 \n";
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "Output genGamma(alpha,beta): \t"  << engine.genGamma(105, 2) << "\n";
		//output gamma realizations with alpha_2 and beta_2
	}

	// Question 2.

	// Problem 1.
	std::cout << "---  Q2.P1  --- \n";

	// See gaussian_header.h file


	// Problem 2
	std::cout << "---  Q2.P2  --- \n";

	// These variables are already defined in Q1P8
	// so we just reuse them and set their values, so we are sure nothing happend to them:
	Spot = 100.0;
	Expiry = 1.0;
	Strike = 100.0;
	Vol = 0.1;
	r = 0.0;

	std::cout << "Price of canonical call option: " << BlackScholes(Spot, Vol, Expiry, Strike) << "\n";
	//output the value of call option using the function defined in "BlackScholesPrice.h"

	//Problem 3
	std::cout << "---  Q2.P3  --- \n";

	//intilization of my BlackScholesModel class
	//See BlackScholesModel.h for implementation
	BlackScholesModel BsModel(Spot,Vol);
	
	//Use getSpot method
	std::cout << "getSpot method: " << BsModel.getSpot() << "\n";
	//use getVol method
	std::cout << "getVol method: " << BsModel.getVol() << "\n";

	//Problem 4
	std::cout << "---  Q2.P4  --- \n";
	
	//outputs the value of call option, using the overloaded black-scholes formula
	//see "BlackScholesPrice.h" for implementation
	std::cout << "Price of canonical call option with BlackScholesModel overloading: " << BlackScholes(BsModel, Expiry, Strike) << "\n";

	//Problem 5
	std::cout << "---  Q2.P5  --- \n";

	//intilization of my CallOption class
	//See CallOption.h for implementation
	CallOption TheCallOption(Expiry,Strike);

	//outputs the value of call option, using the overloaded black-scholes formula
	//see "BlackScholesPrice.h" for implementation
	std::cout << "Price of canonical call option with BlackScholesModel & CallOption overloading: " << BlackScholes(BsModel, TheCallOption) << "\n";

	//Problem 6
	std::cout << "---  Q2.P6  --- \n";

	//outputs the value of call option, using the getPrice method from BSCallOption class
	//see "BlackScholesCallOption.h" for implementation
	BSCallOption BlackScholesCallOption(BsModel, TheCallOption);
	std::cout << "Price of canonical call option using BlackScholesCallOption getPrice method: " << BlackScholesCallOption.getPrice() << "\n";

	//Problem 7
	std::cout << "---  Q2.P7  --- \n";
	// see ClosedformOptionClass.h and BlackScholesCallOptionClass.h for implementation

	//Problem 8
	std::cout << "---  Q2.P8  --- \n";
	
	//define new engine from my class i made in Question 1.
	MSRNG MCEngine(1);
	PayOffCall COption(Strike);
	VanillaOption VOption(COption, Expiry);
	double relativeError = 0.01;
	NumberOfPaths = 500;
	double simplemcError;
	//I'll just use my previous BlackScholesCallOption, which is initialized earlier in Q2.P6.
	// in my overloaded version of my simpleMC function i return an relative error 
	simplemcError = SimpleMC(VOption, MCEngine, Spot, Vol, r, NumberOfPaths, relativeError, BlackScholesCallOption);

	std::cout << "Monto Carlo error:\t " << simplemcError << "\n";


	//Problem 9
	std::cout << "---  Q2.P9  --- \n";
	//see pdf file

	//Problem 10
	std::cout << "---  Q2.P10  --- \n";
	// checked against wolfram alpha - see Gamma_header.h for implementation

	//Problem 11
	std::cout << "---  Q2.P11  --- \n";
	// checked against wolfram alpha - see Gamma_header.h for implementation
	// but basically copy pasted, with the execption that i remade it so it does not contain a class
	// and just a lot of functions and constants

	//Problem 12
	std::cout << "---  Q2.P12  --- \n";
	
	// see GammaModel.h and GammaCallOption.h
	double alpha_1 = 105;
	double alpha_2 = 1.05;
	double beta_1 = 1.05;
	double beta_2 = 0.0105;
	CallOption TestCall(Expiry, Strike);
	gammaModel TestModel1(alpha_1, beta_1);
	gammaModel TestModel2(alpha_2, beta_2);
	GammaCallOption TestPrices1(TestModel1, TestCall);
	GammaCallOption TestPrices2(TestModel2, TestCall);

	std::cout << "Price for S_1: \t" << TestPrices1.getPrice() << "\n";
	std::cout << "Price for S_2: \t" << TestPrices2.getPrice() << "\n";

	std::cout << "---  Q2.P13  --- \n";
	
	// redefine again for good measure. 
	Spot = 100.0;
	Expiry = 1.0;
	Strike = 100.0;
	Vol = 0.1;
	r = 0.0;



	ClosedformOption* BSObjectPtr; //create pointer
	BSObjectPtr = new BSCallOption(BlackScholesModel(Spot, Vol),
		CallOption(Expiry, Strike)); //point to memory of a Black-scholes object 
	ClosedformOption* GammaObjectPtr;
	GammaObjectPtr = new GammaCallOption(gammaModel(alpha_1,beta_1),CallOption(Expiry,Strike)); // point to memory of a gamma object

	std::vector<ClosedformOption*> OptionValues; //create vector of pointers
	OptionValues.push_back(GammaObjectPtr);
	OptionValues.push_back(BSObjectPtr);
	size_t portdim = OptionValues.size();
	std::vector<double> weights = { 0.5,0.5 };
	portfolio MyPortfolio(portdim, OptionValues, weights); // Portfolio.h


	//output values
	for (size_t i = 0; i < portdim; i++) {
		std::cout << " Price of each object in portfolio " << OptionValues[i]->getPrice() << "\n";
	}
	std::cout << "GetPrice of Portfolio \n";
	std::cout << "  " << MyPortfolio.getPrice() << "\n";
	

	std::cout << "---  Q3.P1  --- \n";

	//answers in pdf file


	std::cout << "---  Q3.P2  --- \n";

	//answers in pdf file

	std::cout << "---  Q3.P3  --- \n";

	seed = 1;
	Spot = 100.0;
	r = 0.04;
	Strike = 100.0;
	Expiry = 1.0;
	double kappa = 1.78;
	double theta = 0.0961;
	double sigma = 0.863;
	double v0 = 0.0484;
	double rho = -0.845;
	unsigned int nPaths = 10000;
	unsigned int nSteps = 200;
	double CallPayOff = 0;

	MSRNG HestonEngine(seed, nSteps);
	// implementation of Heston Monte Carlo in "HestonMC.h"

	CallPayOff = MontoCarloHeston(HestonEngine, Spot, Strike, Expiry, r, theta, kappa, sigma, v0, rho, nPaths, nSteps);

	std::cout << "The payoff is: \t" << CallPayOff << "\n";

	std::cout << "---  Q3.P4  --- \n";

	// implementation of Heston object is in "HestonModel.h"

	HestonEngine.setSeed(1);//setseed
	HestonModel HModel(kappa, theta, sigma, v0, rho); //construct model
	CallPayOff = MontoCarloHeston(HestonEngine, HModel, Spot, Strike, Expiry, r, nPaths, nSteps); //use overloaded function

	std::cout << "the payoff with an overloaded monte carlo: \t" << CallPayOff << "\n";

	system("pause");
    return 0;
}
