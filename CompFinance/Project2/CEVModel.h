#pragma once
#include <vector>
class CEVModel
{

public:
	CEVModel(double spot_, double vol_, double beta_) : spot(spot_), vol(vol_), beta(beta_) {};
	CEVModel() : spot(100.0), vol(2), beta(0.5) {};

	double getSpot() { return CEVModel::spot;};
	double getVol() { return CEVModel::vol;};
	double getBeta() { return CEVModel::beta;};

private:
double beta;
double spot;
double vol;
};
