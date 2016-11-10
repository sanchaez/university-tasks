#include <iostream>
#include <iomanip>
#include <initializer_list>
#include "equations.h"
//20: ln(x)- x + 5=0; I, H
//[a, b] = [0, 1]

double fn(double x) {
	return log(x) - x + 5;
}

double derivative(double x) {
	return 1 / x - 1;
}

template <class T> 
inline void fixed_width_print_obj(T obj, const int& width = 15, const int& precision = 15) {
	std::cout << std::left << std::setw(width) << std::setprecision(precision) << obj;
}

template <class T> 
void fixed_width_print_line(std::initializer_list<T> list, const int& width = 15) {
	for (T obj : list) {
		fixed_width_print_obj<T>(obj, width);
	}
	std::cout << std::endl;
}


void main(void) {
	eq::Equation<double(double)> equations(&fn, &derivative, 5, 7);
	//iterative test results
	fixed_width_print_line({ ":precision", ":iterative" });
	for (double precision = 1e-2; precision >= 1e-14; precision *= 1e-3) {
		fixed_width_print_line({ precision, equations.iterative(precision) });
	}
	std::cin.get();
}