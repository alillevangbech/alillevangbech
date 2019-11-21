#include "MatrixProduct.h"



void matrixProduct(
	size_t rows1, size_t cols1, double* numbers1,
	size_t rows2, size_t cols2, double* numbers2,
	double* results)
{
	size_t resultRows = rows1, resultCols = cols2;

	for (size_t i = 0; i < resultRows; i++) for (size_t j= 0; j < resultCols;j++)
	{
		double tempRes = 0.0;
		for (size_t k = 0; k < cols1; k++)
		
		tempRes += numbers1[i*cols1 + k] * numbers2[i*cols2 + j];
		results[i*resultCols + j] = tempRes;
		


	}


}