#include <iostream>
#include "print_helpers.h"
#include "equations.h"

//20: ln(x)- x + 5=0; I, H
//[a, b] = [0, 1]

double fn(double x) {
	return log(x) - x + 5;
}

double derivative(double x) {
	return 1 / x - 1;
}

typedef double (eq::Equation<double(double)>::*EquationMemberFunction)(const double &);

inline void print_table_single_test(eq::Equation<double(double)> &test_equation_object, EquationMemberFunction test_function) {
	std::cout << std::endl;
	fixed_width_print_line({ ":precision", ":x" });
	for (double precision = 1e-2; precision >= 1e-14; precision *= 1e-3) {
		fixed_width_print_line({ precision, (test_equation_object.*test_function)(precision) });
	}
	std::cout << std::endl;
};

inline void print_table_iterations_test(eq::Equation<double(double)> &test_equation_object) {
	std::cout << std::endl;
	double n_iterative,
		n_hordes;
	fixed_width_print_line({ ":precision", ":iterative_count", ":hordes_count" }, 20);
	for (double precision = 1e-2; precision >= 1e-14; precision *= 1e-3) {
		test_equation_object.iterative(precision);
		n_iterative = static_cast<double>(test_equation_object.get_last_iterations());
		test_equation_object.hordes(precision);
		n_hordes = static_cast<double>(test_equation_object.get_last_iterations());
		fixed_width_print_line({ precision, n_iterative, n_hordes }, 20);
	}
	std::cout << std::endl;
};

void main(void) {
	eq::Equation<double(double)> equations(&fn, &derivative, 5, 7);
	//iterative test results
	std::cout << "Test 1: Iterative" << std::endl;
	print_table_single_test(equations, &eq::Equation<double(double)>::iterative);
	//hordes test results
	std::cout << "Test 2: Hordes" << std::endl;
	print_table_single_test(equations, &eq::Equation<double(double)>::hordes);
	//iterations comparison test
	std::cout << "Test 3: Iterations comparison" <<  std::endl;
	print_table_iterations_test(equations);
	std::cin.get();
}