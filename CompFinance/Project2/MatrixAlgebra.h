#pragma once
#include <vector>
#include <stdexcept>

inline std::vector<double> tridag(const std::vector<double> a,
	const std::vector<double> b,
	const std::vector<double> c,
	const std::vector<double> r,
	std::vector<double> u)
{
	unsigned long n = u.size();
	unsigned long j;
	std::vector<double> gam(n);
	double bet = b[0];

	// One vector of workspace, gam is needed.

	if (b[0] == 0.0) throw std::invalid_argument("Error 1 in tridag");
	// If this happens then you should rewrite your equations as a set of order N ? 1, with u2 trivially eliminated.

	u[0] = r[0] / b[0];

	// Decomposition and forward substitution.
	for (j = 1; j < n; j++) {
		gam[j] = c[j - 1] / bet;
		bet = b[j] - a[j] * gam[j];

		if (bet == 0.0)	throw std::invalid_argument("Error 2 in tridag"); // Algorithm fails; see below.

		u[j] = (r[j] - a[j] * u[j - 1]) / bet;
	}

	// Backsubstitution.
	for (size_t j = n - 1; j-->0;)
	{
		u[j] -= gam[j + 1] * u[j + 1];
	}
	return u;
}




