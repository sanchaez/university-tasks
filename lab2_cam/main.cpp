#include <iostream>
#include <iomanip>
#include "equations.h"
//20: ln(x)- x + 5=0; I, H
//[a, b] = [0, 1]
double fn(double x) {
	return log(x) - x + 5;
}

double derivative(double x) {
	return 1 / x - 1;
}

void main(void) {
	eq::Equation<double(*)(double)> equations(&fn, &derivative, 0, 1);
	std::cout << equations(5) << fn(5);
	std::cin.get();
}