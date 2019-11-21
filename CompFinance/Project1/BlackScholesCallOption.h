#pragma once
#include "BlackScholesModel.h"
#include "CallOption.h"
#include "ClosedformOption.h"
#include <vector>

class BSCallOption : public ClosedformOption
{
public:
	BSCallOption(BlackScholesModel& BsModel,CallOption& TheCallOption); //references so it does not need to copy
	double getPrice() const; //virtual method because it inherits from closedformoption
	~BSCallOption() {};
	virtual ClosedformOption* clone() const;

private:
	BlackScholesModel& BsModel;
	CallOption& TheCallOption;
};

