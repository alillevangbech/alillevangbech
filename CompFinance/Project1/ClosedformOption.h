#pragma once

class ClosedformOption
{
public:	//It's a base class, so all functions are inherited and we dont need to implemnet anything in the cpp file
	virtual double getPrice() const = 0; //=0 so it's a pure virtual method
	virtual ~ClosedformOption() {}
	virtual ClosedformOption* clone() const = 0;

private:

};

