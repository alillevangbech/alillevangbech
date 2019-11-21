#pragma once

class BlackScholesModel {

public:
	BlackScholesModel(double spot, double vol); //constructor
	double getSpot(); //simple get method
	double getVol();
private:
	const double Spot; //const because we dont want these variables to change
	const double Vol;
};
