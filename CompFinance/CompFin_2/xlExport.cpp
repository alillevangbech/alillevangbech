
#include <windows.h>
#include <math.h>
#include "xlcall.h"
#include "framework.h"
#include "AnalyticSolutions.h"
#include "ImpliedVol.h"
#include "FiniteDifferences.h"
#include <ctime>
#include "MonteCarloCEV.h"
#include <vector>

std::vector<double> UnpackVector(FP12* x)
{

	size_t rows = x->rows;
	size_t cols = x->columns;
	double* numbers = x->array;

	std::vector<double> result(rows);

		for (size_t i = 0; i < rows; i++)
		{
			result[i] = numbers[i];
		}

		return result;
}

//	Wrappers

extern "C" __declspec(dllexport)
double xMultiply2Numbers(double x, double y)
{
	return x * y;
}


extern "C" __declspec(dllexport)
double xPlus2Numbers(double x, double y)
{
	return x + y;
}

extern "C" __declspec(dllexport)
double xBlackScholes(double spot, double vol, double mat, double strike)
{
	return BlackScholes(spot, vol, mat, strike);

}

extern "C" __declspec(dllexport)
double xBachelier(double spot, double vol, double mat, double strike)
{
	return Bachelier(spot, vol, mat, strike);
}

extern "C" __declspec(dllexport)
double xCEV(double spot, double vol, double mat, double strike, double beta)
{
	return CEV(spot, vol, mat, strike,beta);
}

extern "C" __declspec(dllexport)
double xImpliedVol(double spot, double price, double mat, double strike)
{
	return iVol(spot, price, mat, strike);
}


extern "C" __declspec(dllexport)
double xCNFiniteDifferencesCall(
	double StateMin, double StateMax,
	double StateSteps, double TimeSteps,
	double spot, double vol, double mat, double strike, double beta)
{
	return CNFiniteDifferencesCall(
		StateMin, StateMax,
		StateSteps, TimeSteps,
		spot, vol, mat, strike, beta);
}

extern "C" __declspec(dllexport)
double xCNFiniteDifferencesCall_getTimeInSeconds(
	double StateMin, double StateMax,
	double StateSteps, double TimeSteps,
	double spot, double vol, double mat, double strike, double beta)
{

	const clock_t tic = clock();

	double TestTime = CNFiniteDifferencesCall(
		StateMin, StateMax,
		StateSteps, TimeSteps,
		spot, vol, mat, strike, beta);

	return float(clock() - tic) / CLOCKS_PER_SEC;

}

extern "C" __declspec(dllexport)
FP12* xMonteCarloCEVCall(double spot,
					 double lambda,
					 double strike,
					 double beta,
					 double mat,
					 double nPaths,
					 double nSteps)
{	
	const clock_t tic = clock();

	size_t resultRows = 1, resultCols = 2, resultSize = resultRows * resultCols;
	if (resultSize <= 0) return nullptr;
	FreeAllTempMemory();
	size_t memSize = sizeof(FP12) + (resultSize - 1) * sizeof(double);
	FP12* result = (FP12*)GetTempMemory(memSize);

	result->rows = resultRows;
	result->columns = resultCols;


	result->array[0] = MonteCarloCEVCall(spot, lambda, strike, beta, mat, nPaths, nSteps);
	result->array[1] = float(clock() - tic) / CLOCKS_PER_SEC;

	return result;
}

extern "C" __declspec(dllexport)
double xTESTMonteCarloCEVCall(double spot,
	double lambda,
	double strike,
	double beta,
	double mat,
	double nPaths,
	double nSteps)
{


	return MonteCarloCEVCall(spot, lambda, strike, beta, mat, nPaths, nSteps);
	
}

extern "C" __declspec(dllexport)
double xTESTpMonteCarloCEVCall(double spot,
	double lambda,
	double strike,
	double beta,
	double mat,
	double nPaths,
	double nSteps)
{


	return pMonteCarloCEVCall(spot, lambda, strike, beta, mat, nPaths, nSteps);

}

extern "C" __declspec(dllexport)
FP12* xpMonteCarloCEVCall(double spot,
	double lambda,
	double strike,
	double mat,
	double beta,
	double nPaths,
	double nSteps)
{
	const clock_t tic = clock();

	size_t resultRows = 1, resultCols = 2, resultSize = resultRows * resultCols;
	if (resultSize <= 0) return nullptr;
	FreeAllTempMemory();
	size_t memSize = sizeof(FP12) + (resultSize - 1) * sizeof(double);
	FP12* result = (FP12*)GetTempMemory(memSize);

	result->rows = resultRows;
	result->columns = resultCols;


	result->array[0] = pMonteCarloCEVCall(spot, lambda, strike, mat, beta, nPaths, nSteps);
	result->array[1] = float(clock() - tic) / CLOCKS_PER_SEC;

	return result;
}

extern "C" __declspec(dllexport)
FP12* xMonteCarloCEVinterpCall(double spot,
	FP12* lambda,
	FP12* timeline,
	double strike,
	double mat,
	double beta,
	double nPaths,
	double nSteps)
{
	const clock_t tic = clock();

	size_t resultRows = 1, resultCols = 2, resultSize = resultRows * resultCols;
	if (resultSize <= 0) return nullptr;
	FreeAllTempMemory();
	size_t memSize = sizeof(FP12) + (resultSize - 1) * sizeof(double);
	FP12* result = (FP12*)GetTempMemory(memSize);

	result->rows = resultRows;
	result->columns = resultCols;

	vector<double> vlambdas = UnpackVector(lambda);
	vector<double> vtimeline = UnpackVector(timeline);

	result->array[0] = MonteCarloCEVinterpCall(spot, vlambdas, vtimeline, strike, mat, beta, nPaths, nSteps);;
	result->array[1] = float(clock() - tic) / CLOCKS_PER_SEC;

	return result;
}




extern "C" __declspec(dllexport)
FP12* xpMonteCarloCEVinterpCall(double spot,
	FP12* lambda,
	FP12* timeline,
	double strike,
	double mat,
	double beta,
	double nPaths,
	double nSteps)
{
	const clock_t tic = clock();

	size_t resultRows = 1, resultCols = 2, resultSize = resultRows * resultCols;
	if (resultSize <= 0) return nullptr;
	FreeAllTempMemory();
	size_t memSize = sizeof(FP12) + (resultSize - 1) * sizeof(double);
	FP12* result = (FP12*)GetTempMemory(memSize);

	result->rows = resultRows;
	result->columns = resultCols;

	vector<double> vlambdas = UnpackVector(lambda);
	vector<double> vtimeline = UnpackVector(timeline);

	result->array[0] = pMonteCarloCEVinterpCall(spot, vlambdas, vtimeline, strike, mat, beta, nPaths, nSteps);;
	result->array[1] = float(clock() - tic) / CLOCKS_PER_SEC;

	return result;
}









//	Registers

extern "C" __declspec(dllexport) int xlAutoOpen(void)
{
	XLOPER12 xDLL;	

	Excel12f(xlGetName, &xDLL, 0);

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMultiply2Numbers"),
		(LPXLOPER12)TempStr12(L"BBB"),
		(LPXLOPER12)TempStr12(L"xMultiply2Numbers"),
		(LPXLOPER12)TempStr12(L"x, y"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Multiplies 2 numbers"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xPlus2Numbers"),
		(LPXLOPER12)TempStr12(L"BBB"),
		(LPXLOPER12)TempStr12(L"xPlus2Numbers"),
		(LPXLOPER12)TempStr12(L"x, y"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Plus 2 numbers"),
		(LPXLOPER12)TempStr12(L""));



	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xBlackScholes"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xBlackScholes"),
		(LPXLOPER12)TempStr12(L"spot,vol,mat,strike"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Black & Scholes formula"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xBachelier"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xBachelier"),
		(LPXLOPER12)TempStr12(L"spot,vol,mat,strike"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Bachelier formula"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xCEV"),
		(LPXLOPER12)TempStr12(L"BBBBBB"),
		(LPXLOPER12)TempStr12(L"xCEV"),
		(LPXLOPER12)TempStr12(L"spot,vol,mat,strike,beta"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements CEV formula"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xImpliedVol"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xImpliedVol"),
		(LPXLOPER12)TempStr12(L"spot,price,mat,strike"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"returns implied volatility"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xCNFiniteDifferencesCall"),
		(LPXLOPER12)TempStr12(L"BBBBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xCNFiniteDifferencesCall"),
		(LPXLOPER12)TempStr12(L"StateMin,StateMax,StateSteps,TimeSteps,spot,vol,mat,strike,beta"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"returns CNFiniteDiffCallCEV"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xCNFiniteDifferencesCall_getTimeInSeconds"),
		(LPXLOPER12)TempStr12(L"BBBBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xCNFiniteDifferencesCall_getTimeInSeconds"),
		(LPXLOPER12)TempStr12(L"StateMin,StateMax,StateSteps,TimeSteps,spot,vol,mat,strike,beta"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"returns time for CNFiniteDiffCallCEV"),
		(LPXLOPER12)TempStr12(L""));


	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMonteCarloCEVCall"),
		(LPXLOPER12)TempStr12(L"K%BBBBBBB"),
		(LPXLOPER12)TempStr12(L"xMonteCarloCEVCall"),
		(LPXLOPER12)TempStr12(L"spot,lambda,strike,mat,beta,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Monte Carlo pricing for the CEV model"),
		(LPXLOPER12)TempStr12(L""));


	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xTESTMonteCarloCEVCall"),
		(LPXLOPER12)TempStr12(L"BBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xTESTMonteCarloCEVCall"),
		(LPXLOPER12)TempStr12(L"spot,lambda,strike,mat,beta,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Monte Carlo pricing for the CEV model"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xTESTpMonteCarloCEVCall"),
		(LPXLOPER12)TempStr12(L"BBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xTESTpMonteCarloCEVCall"),
		(LPXLOPER12)TempStr12(L"spot,lambda,strike,mat,beta,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Monte Carlo pricing for the CEV model"),
		(LPXLOPER12)TempStr12(L""));


	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xpMonteCarloCEVCall"),
		(LPXLOPER12)TempStr12(L"K%BBBBBBB"),
		(LPXLOPER12)TempStr12(L"xpMonteCarloCEVCall"),
		(LPXLOPER12)TempStr12(L"spot,lambda,strike,mat,beta,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements parallel Monte Carlo pricing for the CEV model"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMonteCarloCEVinterpCall"),
		(LPXLOPER12)TempStr12(L"K%BK%K%BBBBB"),
		(LPXLOPER12)TempStr12(L"xMonteCarloCEVinterpCall"),
		(LPXLOPER12)TempStr12(L"spot,lambda,timeline,strike,mat,beta,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements parallel time dependent Monte Carlo pricing for the CEV model"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xpMonteCarloCEVinterpCall"),
		(LPXLOPER12)TempStr12(L"K%BK%K%BBBBB"),
		(LPXLOPER12)TempStr12(L"xpMonteCarloCEVinterpCall"),
		(LPXLOPER12)TempStr12(L"spot,lambda,timeline,strike,mat,beta,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements parallel time dependent Monte Carlo pricing for the CEV model"),
		(LPXLOPER12)TempStr12(L""));




	/* Free the XLL filename */
	Excel12f(xlFree, 0, 1, (LPXLOPER12)&xDLL);

	return 1;
}

