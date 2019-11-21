#pragma once
#pragma warning(disable:4244)

#include <cmath>
#include <numeric>
#include "mrg32k3a.h"
#include <algorithm>
#include <iterator>
#include "threadPool.h"
#include "interpolate.h"
//#include "AAD.h"
#include "MonteCarloCEV.h"
#define BATCHSIZE 64
using namespace std;

//Euler scheme for spot eq. in Lord et al. [2010]. Truncation both in drift and diffusion.
//Used for every variation of monte-carlo below:

double MonteCarloCEVCall(double spot,
					 double lambda,
					 double strike,
					 double beta,
					 double mat,
					 double nPaths,
					 double nSteps)
{
	
	size_t MyDim = nSteps;
	double dt = mat / nSteps;
	double ThePayoff = 0.0;
	mrg32k3a rng;

	// Clone RNG
	auto cRng = rng.clone();
	// init the RNG 
	cRng->init(MyDim);
	// Allocate Gaussian vector
	vector<double> gaussianVec(MyDim);
	double test = 0;
	// Function to calculate the call payoff
	auto payoff = [strike](double S) {return S > strike ? S - strike : 0; };

	for (size_t i = 0; i < nPaths; i++)
	{
		// Next Gaussian vector, dimension MyDim
		cRng->nextG(gaussianVec);
		// accumulate with lambda syntax page 28 in antoine
		double path = accumulate(gaussianVec.begin(), gaussianVec.end(), spot, [&](const double& acc, const double& x)
		{
			double logSpot = log(acc);
			double temp = logSpot - 0.5*lambda*lambda*pow(acc, 2.0 * (beta - 1.0))*dt
				+ lambda * pow(acc, beta - 1.0)*sqrt(dt)*x;
			return exp(temp);
		});
		if (path >= 100.0) test++;
		// compute payoff
		ThePayoff += payoff(path);
	}

	return test++ / nPaths;
}

double pMonteCarloCEVCall(double spot, double lambda, double strike, double beta, double mat, double nPaths, double nSteps) {
	size_t MyDim = nSteps;
	double dt = mat / nSteps;
	vector<double> ThePayoff(nPaths + 1);
	mrg32k3a rng;

	// Allocate space for gaussian vectors and paths, one for each thread.
	ThreadPool *pool = ThreadPool::getInstance();
	const size_t nThread = pool->numThreads();
	vector<vector<double>> gaussVecs(nThread + 1);
	for (auto& vec : gaussVecs) vec.resize(MyDim);

	// Build one RNG per thread
	vector<unique_ptr<RNG>> rngs(nThread + 1);
	for (auto& random : rngs)
	{
		random = rng.clone();
		random->init(MyDim);
	}

	// Reserve memory for futures
	vector<TaskHandle> futures;
	futures.reserve(nPaths / BATCHSIZE + 1);

	// Function to calculate the call payoff
	auto payoff = [strike](double S) {return S > strike ? S - strike : 0; };

	// Start 
	// Same as MC_CEV() except we send tasks to the pool instead of executing them.
	size_t firstPath = 0;
	size_t pathsLeft = nPaths;
	while (pathsLeft > 0) {
		size_t pathsInTask = min<size_t>(pathsLeft, BATCHSIZE);

		// Spawn here
		futures.push_back(pool->spawnTask([&, firstPath, pathsInTask]()
		{

			// Index of the executing thread
			const size_t threadNum = pool->threadNum();

			// Pick the correct pre-allocated workspace for executing the thread
			vector<double>& gaussVec = gaussVecs[threadNum];

			// Pick the thread's RNG and skip it
			auto& random = rngs[threadNum];
			random->skipTo(firstPath);

			// Same procedure as serial simulation
			for (size_t i = 0; i < pathsInTask; i++) {
				// Next Gaussian vector, dimension MyDim
				random->nextG(gaussVec);
				// accumulate with lambda syntax 
				double path = accumulate(gaussVec.begin(), gaussVec.end(), spot, [&](const double& acc, const double& x)
				{
					double logSpot = log(acc);
					double temp = logSpot - 0.5*lambda*lambda*pow(acc, 2 * (beta - 1))*dt
						+ lambda * pow(acc, beta - 1)*sqrt(dt)*x;
					return exp(temp);
				});
				//calculate pay off 
				ThePayoff[firstPath + i] = payoff(path);

			}

			//  Tasks must return bool
			return true;

		}));


		pathsLeft -= pathsInTask; // countdown
		firstPath += pathsInTask; // add number of tasks run, so the rng knows where to start

	}
	//  Wait and help
	for (auto& future : futures) pool->activeWait(future);

	double result = accumulate(ThePayoff.begin(), ThePayoff.end(), 0.0,
		[&](const double& acc, const double x) {return acc + x; });

	return result / nPaths;

}


// With interpolation
double MonteCarloCEVinterpCall(double spot,
	vector<double> lambda,
	vector<double> timeline,
	double strike,
	double mat,
	double beta,
	double nPaths,
	double nSteps)
{

	size_t MyDim = nSteps;
	double dt = mat / nSteps;
	double ThePayoff = 0.0;
	mrg32k3a rng;

	// Page 222 in antione
	// Clone RNG
	auto cRng = rng.clone();
	// init the RNG 
	cRng->init(MyDim);
	// Allocate Gaussian vector
	vector<double> gaussianVec(MyDim);

	// Function to calculate the call payoff
	auto payoff = [strike](double S) {return S > strike ? S - strike : 0; };


	//interpolate
	vector<double> lambda_interpolated;
	vector<double> timeline_dt(MyDim);
	double i = 0;

	std::generate(timeline_dt.begin(), timeline_dt.end(), [i, mat, nSteps]() mutable { return i += mat / nSteps; });

	transform(timeline_dt.begin(), timeline_dt.end(), back_inserter(lambda_interpolated), [&](double x) {return interpolate(lambda, timeline, x);});

	size_t N = nPaths;
	for (size_t i = 0; i < N; i++)
	{

		// Next Gaussian vector, dimension MyDim
		cRng->nextG(gaussianVec);

		// index of our timeline
		double currentIndex = 0.0;
		// accumulate with lambda syntax
		double path = accumulate(gaussianVec.begin(), gaussianVec.end(), spot, [&](const double& acc, const double& x)
		{
			double logSpot = log(acc);
			double currentVol = lambda_interpolated[currentIndex];
			double temp = logSpot - 0.5*currentVol*currentVol*pow(acc, 2 * (beta - 1))*dt
				+ currentVol*pow(acc, beta - 1)*sqrt(dt)*x;
			currentIndex++;
			return exp(temp);
		});

		// compute payoff
		ThePayoff += payoff(path);
	}

	return ThePayoff / nPaths;
}


// With interpolation
double pMonteCarloCEVinterpCall(double spot,
	vector<double> lambda,
	vector<double> timeline,
	double strike,
	double mat,
	double beta,
	double nPaths,
	double nSteps)
{
	size_t MyDim = nSteps;
	double dt = mat / nSteps;
	vector<double> ThePayoff(nPaths + 1);
	mrg32k3a rng;

	// Allocate space for gaussian vectors and paths, one for each thread.
	ThreadPool *pool = ThreadPool::getInstance();
	const size_t nThread = pool->numThreads();
	vector<vector<double>> gaussVecs(nThread + 1); // +1 for main
	for (auto& vec : gaussVecs) vec.resize(MyDim);

	// Build one RNG per thread
	vector<unique_ptr<RNG>> rngs(nThread + 1);
	for (auto& random : rngs)
	{
		random = rng.clone();
		random->init(MyDim);
	}

	// Reserve memory for futures
	vector<TaskHandle> futures;
	futures.reserve(nPaths / BATCHSIZE + 1);

	// Function to calculate the call payoff
	auto payoff = [strike](double S) {return S > strike ? S - strike : 0; };

	
	//interpolate
	vector<double> lambda_interpolated;
	vector<double> timeline_dt(MyDim);
	double i = 0;

	std::generate(timeline_dt.begin(), timeline_dt.end(), [i, mat, nSteps]() mutable { return i += mat / nSteps; });

	transform(timeline_dt.begin(), timeline_dt.end(), back_inserter(lambda_interpolated), [&](double x) {return interpolate(lambda, timeline, x);});

	// Start 
	// Same as MC_CEV() except we send tasks to the pool instead of executing them.
	size_t firstPath = 0;
	size_t pathsLeft = nPaths;
	while (pathsLeft > 0) {
		size_t pathsInTask = min<size_t>(pathsLeft, BATCHSIZE);

		// Spawn here
		futures.push_back(pool->spawnTask([&, firstPath, pathsInTask]()
		{

			// Index of the executing thread 
			const size_t threadNum = pool->threadNum();

			// Pick the correct pre-allocated workspace for executing the thread
			vector<double>& gaussVec = gaussVecs[threadNum];

			// Pick the thread's RNG and skip it
			auto& random = rngs[threadNum];
			random->skipTo(firstPath);

			// Same procedure as serial simulation
			for (size_t i = 0; i < pathsInTask; i++) {
				// Next Gaussian vector, dimension MyDim
				random->nextG(gaussVec);
				// index of our timeline
				double k = 0.0;
				// accumulate with lambda syntax
				double path = accumulate(gaussVec.begin(), gaussVec.end(), spot, [&](const double& acc, const double& x)
				{
					double logSpot = log(acc);
					double temp = logSpot - 0.5*lambda_interpolated[k]*lambda_interpolated[k]*pow(acc, 2 * (beta - 1))*dt
						+ lambda_interpolated[k]*pow(acc, beta - 1)*sqrt(dt)*x;
					k++;
					return exp(temp);
				});
				//calculate pay off 
				ThePayoff[firstPath + i] = payoff(path);

			}


			//  Tasks must return bool
			return true;
		}));


		pathsLeft -= pathsInTask; // countdown
		firstPath += pathsInTask; // add number of tasks run, so the rng knows where to start

	}
	//  Wait and help
	for (auto& future : futures) pool->activeWait(future);

	double result = accumulate(ThePayoff.begin(), ThePayoff.end(), 0.0,
		[&](const double& acc, const double x) {return acc + x; });

	return result / nPaths;

}
	


//With AAD and interpolation 

/*
// With interpolation
double ADDMonteCarloCEVinterpCall(double spot,
	vector<double> lambda,
	vector<double> timeline,
	double strike,
	double mat,
	double beta,
	double nPaths,
	double nSteps)
{

	size_t N = nPaths;


	//Convert doubles to Number Class

	vector<Number> lambda_N(lambda.size());
	vector<Number> timeline_N(timeline.size());

	convertCollection(lambda.begin(), lambda.end(), lambda_N.begin());
	convertCollection(timeline.begin(), timeline.end(), timeline_N.begin());

	Number beta_N(beta);
	Number spot_N(spot);
	Number strike_N(strike);
	Number mat_N(mat);

	size_t MyDim = nSteps;
	Number dt = mat_N / nSteps;

	//interpolate
	vector<Number> lambda_interpolated;
	vector<double> timeline_dt(MyDim);
	double i = 0.0;

	std::generate(timeline_dt.begin(), timeline_dt.end(), [i, mat, MyDim]() mutable { return i += mat / MyDim;});

	transform(timeline_dt.begin(), timeline_dt.end(), back_inserter(lambda_interpolated),
		[&](double x) {double ip = interpolate(lambda, timeline, x); return Number(ip);});


	mrg32k3a rng;


	// Clone RNG
	auto cRng = rng.clone();


	//  AAD - 1
	//  Access to tape
	Tape& tape = *Number::tape;
	//  Clear and initialise tape
	tape.clear();
	auto resetter = setNumResultsForAAD();
	//  Put parameters on tape
	//  note that also initializes all adjoints
	
	putOnTape(lambda_N.begin(),lambda_N.end());
	putOnTape(timeline_N.begin(), timeline_N.end());
	spot_N.putOnTape();
	mat_N.putOnTape();
	beta_N.putOnTape();
	strike_N.putOnTape();
	putOnTape(lambda_interpolated.begin(), lambda_interpolated.end());

	tape.mark();


	vector<Number> ThePayoff(nPaths);
	Number zero(0.0);

	// init the RNG 
	cRng->init(MyDim);
	// Allocate Gaussian vector
	vector<double> gaussianVec(MyDim);

	// Function to calculate the call payoff
	auto payoff = [strike,zero](const Number& thisspot) {return thisspot > strike ? thisspot - strike : zero; };

	Number beta_1(beta_N - 1);
	Number exponent(2 * (beta_N - 1));
	Number onehalf(0.5);
	Number sum(0.0);

	for (size_t i = 0; i < N; i++)
	{

		//  AAD - 2
		//  Rewind tape to mark
		//  parameters stay on tape but the rest is wiped
		tape.rewindToMark();
		//


		// Next Gaussian vector, dimension MyDim
		cRng->nextG(gaussianVec);

		// accumulate with lambda syntax
		Number logSpot(log(spot_N));
		Number S(spot_N);

		for (i = 0; i < MyDim; i++ ){
			
			logSpot = logSpot - onehalf*lambda_interpolated[i] *lambda_interpolated[i] * pow(S, exponent)*dt
							+ lambda_interpolated[i] * pow(S, beta_1)*sqrt(dt)*gaussianVec[i];
			S = exp(logSpot);
		}
		Number path = S;

		
		// compute payoff
		Number thispo(payoff(path));
		ThePayoff[i] = thispo;

		sum = accumulate(ThePayoff.begin(), ThePayoff.end(), zero, [&](Number& acc, Number& x) {return acc + x;});

		sum.propagateToMark();
		
		
	} 

	
	double result = double(sum);

	//  AAD - 4
	//  Mark = limit between pre-calculations and path-wise operations
	//  Operations above mark have been propagated and accumulated
	//  We conduct one propagation mark to start
	Number::propagateMarkToStart();
	//

	tape.clear();
	
	return result / nPaths;
}
*/
