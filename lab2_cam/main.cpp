#include <iostream>
#include <iomanip>
#include "equations.h"
//20: ln(x)- x + 5=0; I, H

double fn(double x) {
	return log(x) - x + 5;
}

double derivative(double x) {
	return 1 / x - 1;
}

void main(void) {
	std::cin.get();
}