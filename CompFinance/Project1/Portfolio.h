#include <vector>
#include "ClosedformOption.h"
class portfolio
{
public:
	portfolio(size_t dim, std::vector<ClosedformOption* >& OptionValueVector, std::vector<double> weights);
	~portfolio();
	double getPrice();

private:
	size_t dim;
	std::vector<ClosedformOption* > OptionValueVector;
	std::vector<double> weights;
	ClosedformOption* theClosedformOptionPtr;
};
