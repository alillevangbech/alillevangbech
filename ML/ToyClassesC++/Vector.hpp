#ifndef VECTORHEADERDEF
#define VECTORHEADERDEF
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>






template <class T>
class Vector
{
private:
	std::vector<T> mData; // data stored in vector
	int mSize; // size of vector
	
public:
	std::vector<T> const& getStorage() const;
	Vector<T>(int size);
	int size() const;
	T& operator[](int i); // zero-based indexing
	T const& operator[] (int i)const; // zero-based indexing
	// assignment
	Vector<T>& operator=(const Vector<T>& otherVector);
	Vector<T> operator-() const; // unary -
	Vector<T> operator+(const Vector<T>& v1) const; // binary +
	Vector<T> operator-(const Vector<T>& v1) const; // binary -
	// scalar multiplication
	Vector<T> operator*(double a) const;
	// p-norm method
	double CalculateNorm(int p=2) const;
};

template <class T>
Vector<T>::Vector(int size)
{
	assert(size > 0);
	mSize = size;
	mData.resize(mSize);
	for (int i=0; i<mSize; i++)
	{
		mData[i] = 0.0;
	}
}


template <class T>
int Vector<T>::size() const
{
	return mSize;
}


template <class T>
T& Vector<T>::operator[](int i)
{
	assert(i >= 0);
	assert(i < mSize);
	return mData[i];
}


template <class T>
T const& Vector<T>::operator[](int i)const{
	assert(i >= 0);
	assert(i < mSize);
	return mData[i];
}


template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& otherVector){
	assert(mSize == otherVector.mSize);
	for (int i=0; i<mSize; i++)
	{
		mData[i] = otherVector.mData[i];
	}
	return *this;
}

template <class T>
Vector<T> Vector<T>::operator+(const Vector& v1) const
{
	assert(mSize == v1.mSize);
	Vector v(mSize);
	for (int i=0; i<mSize; i++)
	{
		v[i] = mData[i] + v1.mData[i];
	}
	return v;
}

template <class T>
Vector<T> Vector<T>::operator-() const
{
	Vector v(mSize);
	for (int i=0; i<mSize; i++)
	{
		v[i] = -mData[i];
	}
	return v;
}

template <class T>
Vector<T> Vector<T>::operator-(const Vector<T>& v1) const
{
	assert(mSize == v1.mSize);	
	Vector<T> v(mSize);
	for (int i=0; i<mSize; i++)
	{
		v[i] = mData[i]-v1.mData[i];
	}
	return v;
}

template <class T>
Vector<T> Vector<T>::operator*(double a) const
{
	Vector v(mSize);
	for (int i=0; i<mSize; i++)
	{
		v[i] = a*mData[i];
	}
	return v;
}

template <class T>
double Vector<T>::CalculateNorm(int p) const
{
	double norm_val, sum = 0.0;
	for (int i=0; i<mSize; i++)
	{
		sum += pow(fabs(mData[i]), p);
	}
	norm_val = pow(sum, 1.0/((double)(p)));
	return norm_val;
}

template <class T>
std::vector<T> const& Vector<T>::getStorage() const
{
	return mData;
}





#endif
