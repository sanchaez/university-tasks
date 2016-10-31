#define _USE_MATH_DEFINES
#include <cmath>
#include "math_accuracy.h"

double power(const double& value, const int& pow) {
	if (pow == 0) return 0;
	if (pow == 1) return value;	

	double result = 1.0;
	if (pow < 0) {
		for (int i = 0; i < pow; ++i) {
			result /= value;
		}

	} else {
		for (int i = 0; i < pow; ++i) {
			result *= value;
		}
	}
	return result;
}


double CosTaylorSeries::calculate(const double& x, const double& eps)
{
	double x_floored = abs(x);
	while (x_floored > M_PI / 2.) {
		x_floored -= M_PI / 2.;
	}
	x_floored = M_PI / 2. - x_floored;
	result = 0.0;
	last_u.push_back(_fn_u(x_floored, 1));
	n = 1;
	while (fabs(last_u[n - 1]) >= eps) {
		result += last_u[n - 1];
		++n;
		last_u.push_back(_fn_u(x_floored, n));
	}
	//last_u eventually becomes Rn
	result += last_u[n - 1];
	rest = last_u[n - 1];
	last_u.clear();
	return result;
}

double CosTaylorSeries::calculate_n(const double& x, const int& length)
{
	double x_floored = abs(x);
	while (x_floored > M_PI / 2.) {
		x_floored -= M_PI / 2.;
	}
	x_floored = M_PI / 2. - x_floored;
	result = 0.0;
	last_u.push_back(_fn_u(x_floored, 1));
	n = 1;
	while (n <= length) {
		result += last_u[n - 1];
		++n;
		last_u.push_back(_fn_u(x_floored, n));
	}
	//last_u eventually becomes Rn
	result += last_u[n - 1];
	rest = last_u[n - 1];
	last_u.clear();
	return result;
}

double CosTaylorSeries::_fn_u(const double& x, const int& k)
{
	if (k == 1) return x;
	return -((power(x, 2) * last_u[k - 2]) / static_cast<double>( 2 * k * (2 * k - 1)));
}
