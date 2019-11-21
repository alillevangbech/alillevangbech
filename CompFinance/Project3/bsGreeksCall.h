#pragma once

double bsDelta(double spot, double strike, double mat, double vol);
double bsGamma(double spot, double strike, double mat, double vol);
double bsRho(double spot, double strike, double mat, double vol);
double bsTheta(double spot, double strike, double mat, double vol);
double bsVega(double spot, double strike, double mat, double vol);
double NormalDF(double x, double mean, double var);

