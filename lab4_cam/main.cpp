#include "integral.h"
#include "print_helpers.h"

constexpr long double test_function(const long double &x)
{
  return x * x * x * (x * x / 4L + 16 * x / 5L + x * 1.7L);
}

constexpr long double test_antiderivative(const long double &x)
{
  return x * x * x * (x * x / 24L + x * .3L);
}
constexpr long double test_fourth_derivative(const long double &x)
{
  return 30 * x + 36;
}

constexpr long double a = 2;
constexpr long double b = 11;
constexpr long double eps = 1e-5;
// determine h
const long double M   = abs(test_fourth_derivative(b));
const long double h   = sqrt((12 * eps) / ((b - a) * M));

int main()
{
  integral::RangedIntegral<long double>(test_function, h);

  return 0;
}