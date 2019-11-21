#include <math.h>
#include "LinearInterpolation.h"




double LinearInterpolation(double x1, double y1, double x2, double y2, double x0)
{

	return ((y2 - y1) / (x2 - x1))*(x0 - x1) + y1;

}