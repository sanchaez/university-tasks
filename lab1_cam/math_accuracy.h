#pragma once
#include <vector>
#include <cmath>

class CosTaylorSeries {
public:
	CosTaylorSeries() = default;
	double calculate(const double& x, const double& eps);
	double calculate_n(const double& x, const int& length);



	double get_rest() const { return rest; }
	int get_n() const { return n; }
	double get_result() const { return result; }
	double get_x_floored() const { return x_floored; }
protected:
	double _fn_u(const double & x, const int & k, const double & prev, const double & eps);
	double _fn_n(const double & x, const int & k, const double & prev, const int & n);
	int n;
	double rest;
	double result;
	double x_floored;
	std::vector<double> last_u;

};

double power(const double& value, const int& pow);
