#pragma once
#include "ClosedformOption.h"
#include "CallOption.h"
#include "GammaModel.h"

class GammaCallOption : public ClosedformOption
{

public:
	GammaCallOption(gammaModel& GammaModel, CallOption& TheCallOption);
	virtual double getPrice() const;
	virtual ~GammaCallOption() {};
	virtual ClosedformOption* clone() const;

private:
	CallOption& TheCallOption;
	gammaModel& GammaModel;



};