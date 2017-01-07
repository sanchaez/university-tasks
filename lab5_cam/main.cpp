#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "chebyshev_interpolation.h"

const long double a = 3;
const long double b = 12;
const long double eps = 1e-2;
const interpolator::size_t nstart = 40;

long double test_function(const long double& x) {
  return 0.025*exp(x - 5)*sin(2.5*x);
}


void print_polynomial(interpolator::size_t density, interpolator::size_t n, std::string test_file_path) {
  std::ofstream results_test_file(test_file_path);

  auto n_actual = n;
  std::cout << "Calculating polynomial for " << test_file_path << "...";
  interpolator::ChebyshevPolynomial<long double> polynomial(test_function, a, b, eps, n_actual);
  std::cout << "DONE" << std::endl;
  auto error = polynomial.error();
    while (error > eps){
      ++n_actual;
      std::cout << "Recalculating n: " << n_actual;
      polynomial.set_n(n_actual);
      polynomial.recalculate();
      error = polynomial.error();
      std::cout << "; Error : " << error << std::endl;
    }
    std::cout << "Last n: " << n_actual << "; Error : " << error << std::endl;
  long double x = a;
  long double h;
  if (a < 0 && b > 0) {
    h = (abs(a) + abs(b)) / density;
  } else {
    h = abs(b - a) / density;
  }
  for (interpolator::size_t i = 1; i < density; ++i) {
    std::cout << '\r';
    std::cout << "Wrote " << std::setw(5) << std::left << i << " roots to " << test_file_path << "...";
    results_test_file << x << ";" << polynomial(x) << std::endl;
    x += h;
  }
  std::cout << '\r';
  std::cout << "Wrote " << std::setw(5) << std::left << density << " roots to " << test_file_path << "...";
  results_test_file << b << ";" << polynomial(b) << std::endl;
  std::cout << "DONE" << std::endl;
  results_test_file.close(); 
}

int main() {
  print_polynomial(500, nstart, "test.csv");
  std::cout << "Press Enter...";
  std::cin.get();
  return 0;
}
