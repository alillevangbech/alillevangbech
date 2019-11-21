#pragma once
#include "BlackScholesModel.h"
#include "CallOption.h"


double BlackScholes(double spot, double vol, double mat, double strike);
double BlackScholes(BlackScholesModel& BsModel, double mat, double strike);
double BlackScholes(BlackScholesModel& BsModel, CallOption& TheCallOption);


