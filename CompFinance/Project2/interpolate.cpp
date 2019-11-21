#include <math.h>
#include <vector>
#include "matrix.h"
#include <algorithm>
#include <iterator>
#include "mrg32k3a.h"

double interpolate(vector<double> lambda, vector<double> timeline, double x) {

	//logarithmic search
	auto ub_it = upper_bound(timeline.begin(),timeline.end(), x); 

	//Flat extrapolations
	if (timeline.begin() == ub_it) { 
		return *(lambda.begin()); 
	}
	if (timeline.end() == ub_it) {
		return *(lambda.end()-1);
	}

	//Linear interpolation
	size_t n = distance(timeline.begin(), ub_it) - 1;
	auto x1 = timeline.begin()[n];
	auto y1 = lambda.begin()[n];
	auto x2 = timeline.begin()[n + 1];
	auto y2 = lambda.begin()[n + 1];

	return y1 + (x - x1) / (x2 - x1) * (y2 - y1);

}
