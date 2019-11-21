
#include <windows.h>
#include "xlcall.h"
#include "framework.h"
#include "blackScholes.h"
#include "iVol.h"
#include "simpleMontoCarlo.h"
#include "bsGreeksCall.h"
#include "LinearInterpolation.h"
#include <math.h>
#include "MatrixProduct.h"
#include "RangeOfCallPrice.h"
#include "Bachelier.h"
#include "SABR.h"
#include "MonteCarloSABRsim.h"
#include "HestonSim.h"
#include "Merton.h"
#include "Mertonsim.h"


extern "C" __declspec(dllexport)
double xMultiply2Numbers(double x, double y)
{
	return x * y;
}

extern "C" __declspec(dllexport)
double xBlackScholes(double spot, double vol, double mat, double strike)
{
	return blackScholes(spot, vol, mat, strike);

}

extern "C" __declspec(dllexport)
double xiVol(double spot, double price, double mat, double strike)
{
	return iVol(spot, price, mat, strike);
}

extern "C" __declspec(dllexport)
double xbsMontoCarlo(double spot, double vol, double mat, double strike, double nPaths, double nSteps)
{
	return bsMontoCarlo(spot, vol, mat, strike, nPaths, nSteps);
}

extern "C" __declspec(dllexport)
double xBachelier(double spot, double strike, double vol, double mat)
{
	return bachelier(spot,strike,vol,mat);
}

extern "C" __declspec(dllexport)
double xvasBachelier(double spot, double strike, double vol, double mat, double MR)
{
	return vasBachelier(spot, strike, vol, mat,MR);
}


extern "C" __declspec(dllexport)
double xbsDelta(double spot, double vol, double mat, double strike)
{
	return bsDelta(spot, vol, mat, strike);
}

extern "C" __declspec(dllexport)
double xbsGamma(double spot, double vol, double mat, double strike)
{
	return bsGamma(spot, vol, mat, strike);
}

extern "C" __declspec(dllexport)
double xbsRho(double spot, double vol, double mat, double strike)
{
	return bsRho(spot, vol, mat, strike);
}

extern "C" __declspec(dllexport)
double xbsTheta(double spot, double vol, double mat, double strike)
{
	return bsTheta(spot, vol, mat, strike);
}

extern "C" __declspec(dllexport)
double xbsVega(double spot, double vol, double mat, double strike)
{
	return bsVega(spot, vol, mat, strike);
}


extern "C" __declspec(dllexport)
double xRangeFunc(FP12* matrix, double scalar)
{
	// Unpack matrix
	size_t rows = matrix->rows;
	size_t cols = matrix->columns;
	double* numbers = matrix->array;
	// Compute result
	double result = 0.0;
	for (size_t j = 0; j < cols; ++j)
	{
		double norm = 0.0;
		for (size_t i = 0; i < rows; ++i)
		{
			// To access matrix[i][j] = numbers[i*cols+j]
			double xij = numbers[i * cols + j];
			norm += xij * xij;
		}
		norm = sqrt(norm);
		result += (j + 1) * norm;
	}
	result *= scalar;
	return result;
}


extern "C" __declspec(dllexport)
double xLinearInterpolation(FP12* matrix, double x0)
{
	size_t rows = matrix->rows;
	size_t cols = matrix->columns;
	double* numbers = matrix->array;

	double tempequal = 0;
	double xtemplarger = 1000000;
	double xtempsmaller = -1000000;
	double ytemplarger;
	double ytempsmaller;
	


	for (size_t j = 0; j < cols - 1; j++)
	{
		for (size_t i = 0; i < rows; i++)
		{
			if ((numbers[i*cols + j] < x0) && (xtempsmaller < numbers[i*cols + j]))
			{
				xtempsmaller = numbers[i*cols + j];
				ytempsmaller = numbers[i*cols + j+1];
			}
			else if ((numbers[i*cols + j] > x0) && (xtemplarger > numbers[i*cols + j]))
			{
				xtemplarger = numbers[i*cols + j];
				ytemplarger = numbers[i*cols + j+1];
			}
			else if (fabs(numbers[i*cols + j] - x0) < 1.0e-8)
			{
				tempequal = numbers[i * cols + j + 1];
			}
		}
	}


	if (xtempsmaller == -1000000 && tempequal == 0)
	{
		return ytemplarger;
	}
	else if (xtemplarger == 1000000 && tempequal == 0)
	{
		return ytempsmaller;
	}
	else if (tempequal == 0)
	{
		return LinearInterpolation(xtempsmaller, ytempsmaller, xtemplarger, ytemplarger, x0);
	}
	else
	{
		return tempequal;
	}
}

extern "C" __declspec(dllexport)
FP12* xNotMyOwnMmult(FP12* matrix1, FP12* matrix2)
{
	// Unpack matrices

	size_t rows1 = matrix1->rows;
	size_t cols1 = matrix1->columns;
	double* numbers1 = matrix1->array;
	size_t rows2 = matrix2->rows;
	size_t cols2 = matrix2->columns;
	double* numbers2 = matrix2->array;
	// Allocate result
	// Calculate size
	size_t resultRows = rows1, resultCols = cols2, resultSize = resultRows * resultCols;
	// Return an error if size is 0
	if (resultSize <= 0) return nullptr;
	// First, free all memory previously allocated
	// the function is defined in framework.h
	FreeAllTempMemory();
	// Then, allocate the memory for this result
	// We don't need to de-allocate it, that will be done by the next call
	// to this function or another function calling FreeAllTempMemory()

	// Memory size required, details in Dalton's book, section 6.2.2
	size_t memSize = sizeof(FP12) + (resultSize - 1) * sizeof(double);
	// Finally allocate, function definition in framework.h
	FP12* result = (FP12*)GetTempMemory(memSize);
	// Compute result
	result->rows = resultRows;
	result->columns = resultCols;
	matrixProduct(rows1, cols1, numbers1, rows2, cols2, numbers2, result->array);
	// Return it
	return result;
}

extern "C" __declspec(dllexport)
FP12* xbsRangeOfCallPrice(FP12* MatMatrix, FP12* StrikeMatrix, double spot, double vol)
{
	size_t rows1 = MatMatrix->rows;
	size_t cols1 = MatMatrix->columns;
	double* numbers1 = MatMatrix->array;

	size_t rows2 = StrikeMatrix->rows;
	size_t cols2 = StrikeMatrix->columns;
	double* numbers2 = StrikeMatrix->array;

	size_t resultSize = cols1;

	if (resultSize <= 0) return nullptr;

	FreeAllTempMemory();

	size_t memSize = sizeof(FP12*) + (resultSize - 1) * sizeof(double);

	FP12* result = (FP12*)GetTempMemory(memSize);

	// Compute result
	result->rows = rows1;
	result->columns = cols1;
	RangeOfCallPrice(rows1, cols1, numbers1, rows2, cols2, numbers2, spot, vol, result->array);
	// Return it
	return result;


}
/*
FP12* xKristianInterest(FP12* PriceMatrix, FP12* SizeMatrix, double APMMFixedSize, double APMMFloatSize)
{
	size_t PriceRows = PriceMatrix->rows;
	size_t PriceCols = PriceMatrix->columns;
	double* PriceNumbers = PriceMatrix->array;

	size_t SizeRows = SizeMatrix->rows;
	size_t SizeCols = SizeMatrix->columns;
	double* SizeNumbers = SizeMatrix->array;

	size_t TempRows = PriceRows * 3 +2; // A matrix will be 3*n + two additional constraints
	size_t TempCols = TempRows + 1; // b vector added to matrix
	size_t TempSize = TempCols*TempRows;

	
	size_t ResultRows = PriceRows * 2; // A matrix will be 3*n + two additional constraints
	size_t ResultCols = 1; // b vector added to matrix
	size_t ResultSize = ResultRows*ResultCols;
	

	if (TempSize <= 0) return nullptr;
	// if (ResultSize <= 0) return nullptr;
	// First, free all memory previously allocated
	// the function is defined in framework.h
	FreeAllTempMemory();
	
	// size_t memSizeResult = sizeof(FP12) + (ResultSize - 1) * sizeof(double);
	size_t memSizeTemp = sizeof(FP12) + (TempSize - 1) * sizeof(double);
	// Finally allocate, function definition in framework.h
	FP12* TempMatrix = (FP12*)GetTempMemory(memSizeTemp);
	// FP12* result = (FP12*)GetTempMemory(memSizeResult);

	//TempMatrix->rows = TempRows;
	//TempMatrix->columns = TempCols;
	//KristianTransformation(PriceRows, PriceCols, PriceNumbers, SizeRows, SizeCols, SizeNumbers, APMMFixedSize, APMMFloatSize, TempMatrix->array);
	
	double

	return TempMatrix;



	
}
*/

extern "C" __declspec(dllexport)
double xiVolSABR(double Forward0, double sigma0, double mat, double strike, double alpha, double beta, double rho)
{
	return iVolSABR(Forward0, sigma0, mat, strike, alpha, beta, rho);
}

extern "C" __declspec(dllexport)
double xMonteCarloSABRsim(double spot, double strike, double mat, double alpha, double rho, double beta, double sigma0, double nPaths, double nSteps)
{
	return MonteCarloSABRsim(spot, strike, mat, alpha, rho, beta, sigma0, nPaths, nSteps);
}

extern "C" __declspec(dllexport)
double xMontoCarloHeston(double spot, double strike, double mat, double lambda, double eps, double MeanRv, double rho, double nPaths, double nSteps)

{
	return MontoCarloHeston(spot, strike, mat, lambda, eps, MeanRv, rho, nPaths, nSteps);
}

extern "C" __declspec(dllexport)
double xMerton(double spot, double vol, double mat, double strike, double lambda, double m, double v, double nJumps)
{
	return Merton(spot, vol, mat, strike, lambda, m, v, nJumps);
}

extern "C" __declspec(dllexport)
double xMertonsim(double spot, double vol, double mat, double strike, double lambda, double m, double v, double nJumps, double nPaths, double nSteps)
{
	return Mertonsim(spot, vol, mat, strike, lambda, m, v, nJumps, nPaths, nSteps);
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
		(LPXLOPER12)TempStr12(L"number 1, number 2"));

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
		(LPXLOPER12)TempStr12(L"xiVol"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xiVol"),
		(LPXLOPER12)TempStr12(L"spot,price,mat,strike"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements implied volatility"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xbsMontoCarlo"),
		(LPXLOPER12)TempStr12(L"BBBBBBB"),
		(LPXLOPER12)TempStr12(L"xbsMontoCarlo"),
		(LPXLOPER12)TempStr12(L"spot,vol,mat,strike,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"MontoCarlo sim for bs"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xBachelier"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xBachelier"),
		(LPXLOPER12)TempStr12(L"spot,strike,vol,mat"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Bachelier formula"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xvasBachelier"),
		(LPXLOPER12)TempStr12(L"BBBBBB"),
		(LPXLOPER12)TempStr12(L"xvasBachelier"),
		(LPXLOPER12)TempStr12(L"spot,strike,vol,mat,MR"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements vasBachelier formula"),
		(LPXLOPER12)TempStr12(L""));


	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xbsDelta"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xbsDelta"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,vol"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"bs Delta for call"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xbsGamma"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xbsGamma"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,vol"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"bs Gamma for call"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xbsRho"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xbsRho"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,vol"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"bs Rho for call"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xbsTheta"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xbsTheta"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,vol"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"bs Theta for call"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xbsVega"),
		(LPXLOPER12)TempStr12(L"BBBBB"),
		(LPXLOPER12)TempStr12(L"xbsVega"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,vol"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"bs Vega for call"),
		(LPXLOPER12)TempStr12(L""));


	
	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xLinearInterpolation"),
		(LPXLOPER12)TempStr12(L"BK%B"),
		(LPXLOPER12)TempStr12(L"xLinearInterpolation"),
		(LPXLOPER12)TempStr12(L"matrix,x0"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Linear Interpolation"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xNotMyOwnMmult"),
		(LPXLOPER12)TempStr12(L"K%K%K%"),
		(LPXLOPER12)TempStr12(L"xNotMyOwnMmult"),
		(LPXLOPER12)TempStr12(L"matrix1, matrix2"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Matrix product"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xbsRangeOfCallPrice"),
		(LPXLOPER12)TempStr12(L"K%K%K%BB"),
		(LPXLOPER12)TempStr12(L"xbsRangeOfCallPrice"),
		(LPXLOPER12)TempStr12(L"MatMatrix, StrikeMatrix,spot,vol"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Matrix product"),
		(LPXLOPER12)TempStr12(L""));
/*
	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xKristianInterest"),
		(LPXLOPER12)TempStr12(L"K%K%K%BB"),
		(LPXLOPER12)TempStr12(L"xKristianInterest"),
		(LPXLOPER12)TempStr12(L"PriceMatrix, SizeMatrix,APMMFIXEDSIZE,APMMFLOATSIZE"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"test"),
		(LPXLOPER12)TempStr12(L""));

	*/
	
	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xiVolSABR"),
		(LPXLOPER12)TempStr12(L"BBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xiVolSABR"),
		(LPXLOPER12)TempStr12(L"Foward0,Sigma0,mat,strike,alpha,beta,rho"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements implied volatility for the SABR model"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xSABRvol"),
		(LPXLOPER12)TempStr12(L"BBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xSABRvol"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,alpha,rho,beta,sigma0"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements SABR formula"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xSABRsim"),
		(LPXLOPER12)TempStr12(L"BBBBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xSABRsim"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,alpha,rho,beta,sigma0,SABR, nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements SABR simulation"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMonteCarloSABRsim"),
		(LPXLOPER12)TempStr12(L"BBBBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xMonteCarloSABRsim"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,alpha,rho,beta,sigma0,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements SABR simulation"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMontoCarloHeston"),
		(LPXLOPER12)TempStr12(L"BBBBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xMontoCarloHeston"),
		(LPXLOPER12)TempStr12(L"spot,strike,mat,lambda,eps,MeanRv,rho,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Heston simulation"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMerton"),
		(LPXLOPER12)TempStr12(L"BBBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xMerton"),
		(LPXLOPER12)TempStr12(L"spot, vol,mat,strike,lambda,m,v,nJumps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Heston simulation"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMertonsim"),
		(LPXLOPER12)TempStr12(L"BBBBBBBBBBB"),
		(LPXLOPER12)TempStr12(L"xMertonsim"),
		(LPXLOPER12)TempStr12(L"spot, vol,mat,strike,lambda,m,v,nJumps,nPaths,nSteps"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Implements Heston simulation"),
		(LPXLOPER12)TempStr12(L""));
	

	
	/* Free the XLL filename */
	Excel12f(xlFree, 0, 1, (LPXLOPER12)&xDLL);

	return 1;
}
