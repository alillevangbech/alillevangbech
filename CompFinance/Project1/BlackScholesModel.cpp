#include "BlackScholesModel.h"



BlackScholesModel::BlackScholesModel(double Spot_, double Vol_) : Spot(Spot_), Vol(Vol_)
{
}


double BlackScholesModel::getSpot()
{
	return Spot;
}

double BlackScholesModel::getVol()
{
	return Vol;
}


