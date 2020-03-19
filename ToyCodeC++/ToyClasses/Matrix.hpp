#ifndef MATRIXHEADERDEF
#define MATRIXHEADERDEF
#include "Vector.hpp"
#include <vector>

template<class T>
class Matrix
{
private:
	std::vector<T> mData; // entries of matrix
	int mNumRows, mNumCols; // dimensions
public:
	Matrix(const Matrix& otherMatrix);
	Matrix(int numRows, int numCols);
	int GetNumberOfRows() const;
	int GetNumberOfColumns() const;
	T& operator()(int i, int j);
	T const& operator()(int i, int j)const;
	//overloaded assignment operator
	Matrix<T>& operator=(const Matrix<T>& otherMatrix);
	Matrix<T> operator-() const; // unary -
	Matrix<T> operator+(const Matrix<T>& m1) const; // binary +
	Matrix<T> operator-(const Matrix<T>& m1) const; // binary -
	// scalar multiplication
	Matrix<T> operator*(double a) const;
};

template <class T>
Vector<T> operator*(const Matrix<T>& m, const Vector<T>& v)
{
	int original_vector_size = v.size();
	assert(m.GetNumberOfColumns() == original_vector_size);
	int new_vector_length = m.GetNumberOfRows();
	Vector<T> new_vector(new_vector_length);

	for (int i=0; i<new_vector_length; i++)
	{
		for (int j=0; j<original_vector_size; j++)
		{
			new_vector[i] += m(i,j)*v[j];
		}
	}

	return new_vector;
}

template <class T>
Vector<T> operator*(const Vector<T>& v, const Matrix<T>& m)
{
	int original_vector_size = v.size();
	assert(m.GetNumberOfRows() == original_vector_size);
	int new_vector_length = m.GetNumberOfColumns();
	Vector<T> new_vector(new_vector_length);

	for (int i=0; i<new_vector_length; i++)
	{
		for (int j=0; j<original_vector_size; j++)
		{
			new_vector[i] += v[j]*m(j,i);
		}
	}

	return new_vector;
}




template <class T>
Matrix<T>::Matrix(const Matrix& otherMatrix)
{
	mNumRows = otherMatrix.mNumRows;
	mNumCols = otherMatrix.mNumCols;

	mData.resize(mNumCols*mNumRows);
	
	for (int i=0; i<mNumRows; i++)
	{
		for (int j=0; j<mNumCols; j++)
		{
			mData[i*mNumCols + j] = otherMatrix.mData[i*mNumCols + j];
		}
	}
}






template <class T>
Matrix<T>::Matrix(int numRows, int numCols)
{
	assert(numRows > 0);
	assert(numCols > 0);
	mNumRows = numRows;
	mNumCols = numCols;
	mData.resize(mNumRows*mNumCols);
	
}


template <class T>
int Matrix<T>::GetNumberOfRows() const
{
	return mNumRows;
}

template <class T>
int Matrix<T>::GetNumberOfColumns() const
{
	return mNumCols;
}

template <class T>
T& Matrix<T>::operator()(int i, int j)
{
	assert(i >= 0);
	assert(i < mNumRows);
	assert(j >= 0);
	assert(j < mNumCols);
	return mData[i*mNumCols + j];
}

template <class T>
T const& Matrix<T>::operator()(int i, int j)const
{
	assert(i >= 0);
	assert(i < mNumRows);
	assert(j >= 0);
	assert(j < mNumCols);
	return mData[i*mNumCols + j];
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& otherMatrix)
{
	assert(mNumRows = otherMatrix.mNumRows);
	assert(mNumCols = otherMatrix.mNumCols);

	for (int i=0; i<mNumRows; i++)
	{
		for (int j=0; j<mNumCols; j++)
		{
			mData[i*mNumCols + j] = otherMatrix.mData[i*mNumCols + j];
		}
	}
	return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator-() const
{
	Matrix<T> mat(mNumRows, mNumCols);
	for (int i=0; i<mNumRows; i++)
	{
		for (int j=0; j<mNumCols; j++)
		{
			mat(i,j) = -mData[i*mNumCols + j];
		}
	}
	return mat;
}

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& m1) const
{
	assert(mNumRows == m1.mNumRows);
	assert(mNumCols == m1.mNumCols);
	Matrix mat(mNumRows, mNumCols);
	for (int i=0; i<mNumRows; i++)
	{
		for (int j=0; j<mNumCols; j++)
		{
			mat(i,j) = mData[i*mNumCols + j] + m1.mData[i*mNumCols + j];
		}
	}
	return mat;
}

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& m1) const
{
   assert(mNumRows == m1.mNumRows);
   assert(mNumCols == m1.mNumCols);
   Matrix mat(mNumRows, mNumCols);
   for (int i=0; i<mNumRows; i++)
   {
      for (int j=0; j<mNumCols; j++)
      {
         mat(i,j) = mData[i*mNumCols + j] - m1.mData[i*mNumCols + j];
      }
   }
   return mat;
}

template <class T>
Matrix<T> Matrix<T>::operator*(double a) const
{
   Matrix<T> mat(mNumRows, mNumCols);
   for (int i=0; i<mNumRows; i++)
   {
      for (int j=0; j<mNumCols; j++)
      {
         mat(i,j) = a*mData[i*mNumCols + j];
      }
   }
   return mat;
}



#endif
