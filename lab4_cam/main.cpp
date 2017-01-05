#include <vector>
#include "integral.h"
#include "print_helpers.h"

constexpr long double test_function(const long double& x) {
  return x * x * x * (x * x / 4L + 16L * x / 5L - x * 1.7L);
}

constexpr long double test_antiderivative(const long double& x) {
  return x * x * x * x * (x * x / 24L + x * .3L);
}
constexpr long double test_fourth_derivative(const long double& x) {
  return 5L * x * x * x + 18L * x * x;
}

constexpr long double a = 2L;
constexpr long double b = 11L;
std::vector<long double> deltas;
using namespace std;
using namespace integral;

void single_step_test() {
  deltas.clear();
  constexpr long double eps = 1e-5L;
  cout << "Single step calculation" << endl;
  fixed_width_print_line({":eps", ":hstep", ":value", ":delta"});
  // determine h
  RangedTrapeziumStep<long double> approx_solution(test_function, 0);
  RangedIntegralExact<long double> exact_solution(test_antiderivative);
  for (long double eps = 1e-4; eps > 1e-8; eps *= .1) {
    const long double h =
        sqrt((12L * eps) / ((b - a) * abs(test_fourth_derivative(b))));
    approx_solution.set_step(h);
    auto x = approx_solution(a, b);
    auto delta = abs(x - exact_solution(a, b));
    deltas.push_back(delta);
    fixed_width_print_line({eps, h, x, delta});
  }

  cout << endl;
}

void runge_method_test() {
  RangedTrapeziumRunge<long double> approx_solution(test_function, 0);
  RangedIntegralExact<long double> exact_solution(test_antiderivative);
  cout << "Runge step calculation" << endl;
  fixed_width_print_line({":eps", ":hstep", ":delta"});
  for (auto delta_it = deltas.begin(); delta_it != deltas.end(); ++delta_it) {
    approx_solution.set_eps(*delta_it);
    auto x = approx_solution(a, b);
    fixed_width_print_line(
        {*delta_it, approx_solution.step(), abs(x - exact_solution(a, b))});
  }
  cout << endl;
}
int main() {
  cout << "Function: x^3(x^2/4 + 16x/5 - 1.7x);" << endl
       << "Range = [" << a << ", " << b << "]" << endl;
  single_step_test();
  runge_method_test();
  cout << "Press Enter...";
  cin.get();
  return 0;
}