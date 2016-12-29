#define _USE_MATH_DEFINES
#include "math_accuracy.h"

double power(const double& value, const int& pow) {
	if (pow == 0) return 0;
	if (pow == 1) return value;	

	double result = 1.0;
	if (pow < 0) {
		for (int i = 0; i > pow; --i) {
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
	//2*pi
	x_floored = fabs(x) - floor(fabs(x) / (2 * M_PI)) * 2 * M_PI;
	//pi
	if (x_floored > M_PI)
	{
		x_floored = 2 * M_PI - x_floored;
	}
	//pi/2
	if (x_floored > M_PI_2)
	{
		x_floored = M_PI - x_floored;
		return _fn_u(x_floored, 1, -1, eps);
	} else{
		return _fn_u(x_floored, 1, 1, eps);
	}
	last_u.clear();

}

double CosTaylorSeries::calculate_n(const double& x, const int& length)
{	
	//2*pi
	x_floored = fabs(x) - floor(fabs(x) / (2 * M_PI)) * 2 * M_PI;
	//pi
	if (x_floored > M_PI)
	{
		x_floored = 2 * M_PI - x_floored;
	}
	//pi/2
	if (x_floored > M_PI_2)
	{
		x_floored = M_PI - x_floored;
		return _fn_n(x_floored, 1, -1, length);
	}
	else {
		return _fn_n(x_floored, 1, 1, length);
	}

}

double CosTaylorSeries::_fn_u(const double& x, const int& k, const double& prev, const double& eps)
{
	double next = - (prev * x * x )/ (2 * k * (2 * k - 1));
	n = k;
	if (fabs(next) < eps) {
		rest = next;
		return prev + next;
	} else {
		result = prev + _fn_u(x, k + 1, next, eps);
		return result;
	}
}

double CosTaylorSeries::_fn_n(const double & x, const int & k, const double & prev, const int & length)
{
	double next = -(prev * x * x) / (2 * k * (2 * k - 1));
	n = k;
	if (k < length) {
		result = prev + _fn_n(x, k + 1, next, length);
		return result;
	} else {
		rest = next;
		return prev + next;
	}
}

