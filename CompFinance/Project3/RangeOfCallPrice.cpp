#include "RangeOfCallPrice.h"
#include "blackScholes.h"


void RangeOfCallPrice(size_t rows1, size_t cols1, double* numbers1,
	size_t rows2, size_t cols2, double* numbers2, double spot, double vol,
	double* results)
{

	size_t resultRows = rows1, resultCols = cols1;
	double mat, strike;


	
	for (size_t i = 0; i < resultRows; i++) for (size_t j = 0; j < resultCols; j++)
	{
		mat = numbers1[i*resultCols + j];
		strike = numbers2[i*resultCols + j];

		results[i*resultCols + j] = blackScholes(spot, vol, mat, strike);

	}


}
