#include <iostream>
#include <iomanip>
#include "math_accuracy.h"
//cos x; [6.8; 34.9]
const double a = 6.8;
const double b = 34.9;

template<typename T> void print_element(T t, const int& width)
{
	std::cout << std::left << std::setw(width) << std::setfill(' ') << t;
}

void main(void) 
{
	const double x = (b-a)/2.;
	const int width = 20;
	const int int_width = 10;

	CosTaylorSeries series;
	std::cout	<< "Lab1 : CAM : Function calculus : cos(x) : [" << a <<
															" ; "<< b <<
															"] :" << std::endl << std::endl
									<< ":Test 1 : x=" << x << std::endl << std::endl;
	print_element(":eps", width);
	print_element(":n", int_width);
	print_element(":abs error", width);
	print_element(":rest", width);
	std::cout << std::endl;
	//1
	int n_8 = 0;
	for (int i = -2; i > -14; i += -3) {
		double eps = power(10, i);
		series.calculate(x, eps);
		if (eps >= 1e-8) {
			n_8 = series.get_n();
		}

		print_element(eps, width);
		print_element(series.get_n(), int_width);
		print_element(fabs(series.get_result() - cos(x)), width);
		print_element(fabs(series.get_rest()), width);
		std::cout << std::endl;
	}
	std::cout << std::endl
		<< ":Test 2 : n=" << n_8  << std::endl << std::endl;
	//2
	double x_i = 0.0;
	const double h = (b - a) / 10.;
	print_element(":x_i", width);
	print_element(":x_floored", width);
	print_element(":abs error", width);
	print_element(":rest", width);
	std::cout << std::endl;
	for (int i = 0; i < 10; ++i) {
		x_i = a + h*i;
		series.calculate_n(x_i, n_8);
		print_element(x_i, width);
		print_element(series.get_x_floored(), width);
		print_element(fabs(series.get_result() - cos(x_i)), width);
		print_element(fabs(series.get_rest()), width);
		std::cout << std::endl;
	}
	std::cout << std::endl
		<< "Press ENTER...";
	std::cout << std::endl;
	std::cin.get();
}