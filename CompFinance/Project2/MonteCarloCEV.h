#pragma once

double MonteCarloCEVCall(double spot,double lambda, double strike, double beta, double mat, double nPaths, double nSteps);

double pMonteCarloCEVCall(double spot, double lambda, double strike, double beta, double mat, double nPaths, double nSteps);

double MonteCarloCEVinterpCall(double spot, vector<double> lambda, vector<double> timeline, double strike, double mat, double beta, double nPaths, double nSteps);

double pMonteCarloCEVinterpCall(double spot, vector<double> lambda, vector<double> timeline, double strike, double mat, double beta, double nPaths, double nSteps);

//double ADDMonteCarloCEVinterpCall(double spot, vector<double> lambda, vector<double> timeline, double strike, double mat, double beta, double nPaths, double nSteps);