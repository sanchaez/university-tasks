#pragma once
#include "integral.h"
#include "solver.h"

namespace interpolator {

using size_t = solver::index_type;
template <typename T>
using result_type = solver::result_type<T>;
template <typename T>
using function_t<T> = integral::function_type<T>;

template <typename T>
class AbstractPolynomial {
public:
  AbstractPolynomial(const function_type<T>& function, const T& left_bound,
             const T& right_bound, const size_t& polynomial_size)
      : _fn(function), _n(polynomial_size) {
    if (a < 0 && b > 0) {
      _h = (abs(a) + abs(b)) / static_cast<T>(_n);
    } else {
      _h = abs(b - a) / static_cast<T>(_n);
    }
    build_polynomial();
  };

  void set_fn(const function_type<T> function) { is_calculated = false; _fn = function };
  void set_n(const size_t& polynomial_size) { is_calculated = false; _n = polynomial_size; };
  constexpr result_type<T> polynomial() const { return polynomial_coefficients; };
  constexpr auto n() const { return _n; }

  T operator(const T& x) const {
    if (is_calculated) {
      build_polynomial();
    }
    T value = 0.0;
    for (size_t i = 0; i < m_polynomial.size(); ++i) {
      value += polynomial_coefficients * polynomial_fn(x, i);
    }
    return value;
  };

  T error() {
    T result = 0.0;
    size_t n((_b - _a) / sqrt(_eps));
    T x_i = _a;
    for (unsigned long int i = 0; i <= n; ++i) {
      T value = _fn(x_i) - (*this)(x_i);
      result += value * value;
      x_i += _h;
    }
    return sqrt(result / static_cast<T>(n + 1));
  }

 protected:
  virtual T polynomial_fn(const size_t&, const T&) = 0;
  virtual void build_polynomial() {
    solver::matrix_type<T> t_matrix(row_type<T>(_n + 1), _n);
    for (unsigned int i = 0; i < _n; ++i) {
      for (unsigned int j = 0; j < _n; ++j) {
        t_matrix[i][j] = integral::RangedTrapeziumRunge(
          [&](T x)->T { return polynomial_fn(i, x) * polynomial_fn(j, x)},
          _a, _b, 1e-15))();
      }
      t_matrix[i][_n] = integral::RangedTrapeziumRunge(
        [&](T x)->T { return _fn(x) * polynomial_fn(i, x)},
        _a, _b, 1e-15;
    }
    polynomial_coefficients = solver::selection(t_matrix);
  };
  function_type<T> _fn;
  size_t _n;
  T _a;
  T _b;
  T _h;
  bool is_calculated = true;
  solver::result_type<T> polynomial_coefficients;
};

template <typename T>
class ChebyshevPolynomial : AbstractPolynomial<T> {
  using AbstractPolynomial<T>::AbstractPolynomial;

 protected:
  T polynomial_fn(const size_t& n, const T& x) {
    switch (n) {
      case 0:
        return 1;
      case 1:
        return x;
      case 2:
        return 2 * x * x - 1;
      default:
        T tn_m1(1);
        T tn_m2(x);
        T tn;
        for (size_t i = 3; i <= n; ++i) {
          tn = (static_cast<T>(2.0) * x * tn_m1) - tn_m2;
          tn_m2 = tn_m1;
          tn_m1 = tn;
        }
        return tn;
    }
  };

};
}