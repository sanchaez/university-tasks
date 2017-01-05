#include "cubic_interpolation.h"
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

//using gauss-jordan solving as primary method
//initial data
const long double a = 5;
const long double b = 20;
const interpolator::size_t steps = 100;
long double test_function(const long double& x) {
  return 1.65L * log10(abs(sin(3.L * x / 2.L))) / 12.L;
}


void print_spline(interpolator::size_t density, std::string test_file_path) {
  long double h;
  if (a < 0 && b > 0) {
    h = (abs(a) + abs(b)) / density;
  } else {
    h = abs(b - a) / density;
  }
  std::ofstream results_test_file(test_file_path);
  long double x = a;
  std::cout << "Calculating spline...";
  interpolator::CubicSpline<long double> spline(test_function, steps, a, b);
  std::cout << "DONE" << std::endl;

  for (interpolator::size_t i = 1; i < density; ++i) {
    std::cout << '\r';
    std::cout << "Wrote "<< std::setw(5) << std::left << i << "roots to " << test_file_path << "...";
    results_test_file << x << ";" << spline(x) << std::endl;
    x += h;
  }
  std::cout << '\r';
  std::cout << "Wrote " << std::setw(5) << std::left << density << "roots to " << test_file_path << "...";
  results_test_file << b << ";" << spline(b) << std::endl;
  std::cout << "DONE" << std::endl;
  results_test_file.close();
}


int main() {
  print_spline(steps * 5, "test1.csv");
  std::cout << "Press Enter...";
  std::cin.get();
  return 0;
}