#pragma once


class CallOption
{
public:
	CallOption(double Expiry, double Strike); //constructor


	double getExpiry(); //simple get methods
	double getStrike();

private:
	const double Expiry; //const because it needs to stay the same
	const double Strike;

};

