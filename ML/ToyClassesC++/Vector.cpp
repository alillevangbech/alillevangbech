#include <cmath>
#include <iostream>
#include <cassert>
#include "Vector.hpp"
#include <vector>
/*
using namespace std;

int main(int argc, char* argv[]){






Vector<double> G = Vector<double>(2);


G[1] = 10.01;

cout << G[1] << endl;

// We start with vector methods



Vector<int> A = Vector<int>(3); // How to define a vector

A[0] = 10; // Usual way to define entrances in a vector
A[1] = 20;
A[2] = 30;

Vector<double> B = Vector<double>(3); 

 // No need for loops

// TEST BEGINS

// method for size

if (A.size() == 3) {cout << "Correct" << endl;}
else {cout << "ERROR" << endl;}


B[0] = 2.5; // Usual way to define entrances in a vector
B[1] = 11.0;
B[2] = 100.0;

// method for norm


if (A.CalculateNorm(2) < 37.4176 && A.CalculateNorm(2) > 37.4156 ) {cout << "Correct" << endl;}
else {cout << "ERROR" << endl;}

if (B.CalculateNorm(2) < 100.644 && B.CalculateNorm(2) > 100.624 ) {cout << "Correct" << endl;}
else {cout << "ERROR" << endl;}


//MATRIX 



}

// Overridden copy constructor
// Allocates memory for new vector, and copies
// entries of other vector into it

// Constructor for vector of a given size
// Allocates memory, and initialises entries
// to zero

// Overridden destructor to correctly free memory


// Method to get the size of a vector

// Overloading square brackets
// Note that this uses `zero-based' indexing,
// and a check on the validity of the index


// Overloading square brackets
// Note that this uses `zero-based' indexing,
// and a check on the validity of the index

// Overloading the assignment operator


// Overloading the unary - operator


// Overloading the binary + operator


// Overloading the binary - operator


// Overloading scalar multiplication

// Method to calculate norm (with default value p=2)
// corresponding to the Euclidean norm

