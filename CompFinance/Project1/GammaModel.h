#pragma once


class gammaModel {

public:
	gammaModel(double alpha, double beta);
	

	double getAlpha();
	double getBeta();

private:
	const double alpha;
	const double beta;

};