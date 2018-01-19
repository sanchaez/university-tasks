#pragma once
#include "integral.h"
#include "solver.h"

namespace integral {
template <typename T>
class AbstractSimpsonIntegral : public AbstractRangedIntegral<T> {
  using AbstractRangedIntegral<T>::AbstractRangedIntegral;

 protected:
  virtual inline T calculate(const T& a, const T& b, size_t n) const {
    T h;
    if (a < 0 && b > 0) {
      h = (abs(a) + abs(b)) / static_cast<T>(n);
    } else {
      h = abs(b - a) / static_cast<T>(n);
    }
    T result_value = static_cast<T>(0);
    const T half_h = h / static_cast<T>(2);
    for (size_t i = 0; i < n; ++i) {
      result_value +=
          _fn(a + h * i) + 4 * _fn(a + h * i + half_h) + _fn(a + h * (i + 1));
    }
    return result_value * (h / static_cast<T>(6));
  }
};


template <typename T>
class RangedSimpsonRelativeRunge : public AbstractSimpsonIntegral<T> {
 public:
  RangedSimpsonRelativeRunge(const function_type<T>& function, const T& eps)
      : AbstractSimpsonIntegral<T>(function), _eps(eps) {}
  T operator()(const T& a, const T& b) const {
    size_t n = static_cast<size_t>(1 / sqrt(_eps));
    T result = calculate(a, b, n);
    n *= 2;
    T refined_result = calculate(a, b, n);

    while (_eps <=
           std::abs((result - refined_result) / static_cast<T>(15 * result))) {
      result = refined_result;
      n *= 2;
      refined_result = calculate(a, b, n);
    }

    return refined_result;
  }

 protected:
  T _eps;
};
}
namespace interpolator {

using size_t = solver::index_type;
template <typename T>
using result_type = solver::result_type<T>;
template <typename T>
using function_type = integral::function_type<T>;

template <typename T>
class ChebyshevPolynomial {
 public:
  ChebyshevPolynomial(const function_type<T>& function, const T& left_bound,
                      const T& right_bound, const T& eps,
                      const size_t& polynomial_size)
      : _fn(function),
        _a(left_bound),
        _b(right_bound),
        _eps(eps),
        _n(polynomial_size),
        polynomial_coefficients(_n) {
    build_polynomial();
  }

  void set_fn(const function_type<T>& function) {
    is_calculated = false;
    _fn = function
  }
  void set_n(const size_t& polynomial_size) {
    is_calculated = false;
    _n = polynomial_size;
  }
  constexpr result_type<T> polynomial() const {
    return polynomial_coefficients;
  }
  constexpr auto n() const { return _n; }

  T operator()(const T& x) { return get_polynomial(x); }
  void recalculate() {
    if (!is_calculated) {
      build_polynomial();
    }
  }
  T error() {
    T result = static_cast<T>(0);
    T x_i = _a;
    T h;
    if (a < 0 && b > 0) {
      h = (abs(a) + abs(b)) / static_cast<T>(_n);
    } else {
      h = abs(b - a) / static_cast<T>(_n);
    }
    while (x_i <= _b) {
      T value = _fn(x_i) - get_polynomial(x_i);
      result += value * value;
      x_i += h;
    }
    return sqrt(result / static_cast<T>(_n + 1));
  }

 protected:
  T polynomial_fn(const size_t& n, const T& x) const {
    switch (n) {
      case 0:
        return 1;
      case 1:
        return x;
      default:
        T tn_m1(1);
        T tn_m2(x);
        T tn;
        for (size_t i = 2; i <= n; ++i) {
          tn = (static_cast<T>(2.0) * x * tn_m2) - tn_m1;
          tn_m2 = tn_m1;
          tn_m1 = tn;
        }
        return tn;
    }
  };

  void build_polynomial() {
    solver::matrix_type<T> t_matrix(solver::row_type<T>(_n + 1), _n);
    for (unsigned int i = 0; i < _n; ++i) {
      for (unsigned int j = 0; j < _n; ++j) {
        function_type<T> coeff_fn = [&](T x) -> T {
          return polynomial_fn(i, x) * polynomial_fn(j, x);
        };
        integral::RangedSimpsonRelativeRunge<T> integral_i(coeff_fn, eps);
        t_matrix[i][j] = integral_i(_a, _b);
      }
      function_type<T> polynomial_last_fn = [&](T x) -> T {
        return _fn(x) * polynomial_fn(i, x);
      };
      integral::RangedSimpsonRelativeRunge<T> integral_i(polynomial_last_fn,
                                                         eps);
      t_matrix[i][t_matrix.size()] = integral_i(_a, _b);
    }
    polynomial_coefficients = solver::single_division(t_matrix);
    is_calculated = true;
  }

  T t_x(const T& t) { return ((_b + _a) - t * (_b - _a)) / static_cast<T>(2); }
  T x_t(const T& x) { return (static_cast<T>(2) * x - _b - _a) / (_b - _a); }
  T get_polynomial(const T& x) {
    T value = 0.0;
    for (size_t i = 0; i < polynomial_coefficients.size(); ++i) {
      value += polynomial_coefficients[i] * polynomial_fn(i, x);
    }
    return value;
  }
  function_type<T> _fn;
  T _a;
  T _b;
  T _eps;
  size_t _n;
  bool is_calculated = false;
  solver::result_type<T> polynomial_coefficients;
};
}