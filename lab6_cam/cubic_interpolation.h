#pragma once
#include <functional>
#include <vector>
#include "solver.h"

namespace interpolator {
template <typename T>
using function_type = std::function<T(T)>;
using size_t = solver::index_type;
template <typename T>
struct spline_t {
  T a, b, c, d, x;
};

template <typename T>
class CubicSpline {
  CubicSpline(const function_type<T>& function, const size_t& nsteps, const T& a = 0,
         const T& b = 1)
      : _fn(function), _n(nsteps), _a(a), _b(b), _value_changed(true){};
  T operator()(const T& x) const {
    if (_value_changed) {
      build_spline();
      _value_changed = false;
    }
    spline_t<T>& used_spline;
    if (x <= _spline[0].x) {
      used_spline = _spline[0];
    } else if (x >= splines[n - 1].x) {
      used_spline = _spline[n - 1];
    } else {
      // binary search
      size_t i = 0, j = n - 1;
      while (i <= j) {
        size_t k = i + (j - i) / 2;
        if (x <= _spline[k].x) {
          j = k;
        } else {
          i = k;
        }
      }
      used_spline = _spline[j];
    }
    // return value
    T delta = x - used_spline.x;
    return used_spline.a 
      + delta * (used_spline.b 
        + delta * (used_spline.c / static_cast<T>(2) 
          + delta * (used_spline.d / static_cast<T>(6))));
  };

  void set_steps(size_t nsteps) {
    _n = nsteps;
    _value_changed = true;
  };

  void set_range(const T& a, const T& b) {
    _a = a;
    _b = b;
    _value_changed = true;
  };

 protected:
  void build_spline() {
    solver::matrix_type<T> ñ_matrix =
        solver::matrix_type<T>(solver::row_type<T>(_n + 1), _n);
    const T h = (_b - _a) / static_cast<T>(_n);
    // c_0 = 0; c_n = 0;
    ñ_matrix[0][0] = static_cast<T>(1);
    ñ_matrix[_n - 1][_n - 1] = static_cast<T>(1);
    ñ_matrix[0][_n] = static_cast<T>(0);
    ñ_matrix[_n - 1][_n] = static_cast<T>(0);
    _spline[0].x = _a;
    _spline[_n - 1].x = _b;
    _spline[0].a = _fn(_a);
    _spline[_n - 1].a = _fn(_b);
    // filling matrix
    for (size_t i = 1; i < _n - 1; ++i) {
      ñ_matrix[i][i] = static_cast<T>(2) * h;
      ñ_matrix[i][i - 1] = h;
      // if (i < _n - 1)
      ñ_matrix[i][i + 1] = h;
      // optimize _spline fill and calculation
      _spline[i].x = _spline[i - 1].x + h;
      _spline[i].a = _fn(_spline[i].x);
      ñ_matrix[i][_n] = static_cast<T>(6) * (_fn(_spline[i].x + h) -
                                             static_cast<T>(2) * _spline[i].a +
                                             _fn(_spline[i - 1].x))
                                             / h;
    }

    solver::result_type<T> c_coeff = solver::selection(c_matrix);

    // filing spline with matrix solution
    for (int i = 1; i < _n; ++i) {
      _spline[i - 1].c = c_coeff[i];
      _spline[i - 1].b = h * c_coeff[i] / static_cast<T>(2);
      _spline[i - 1].d = (c_coeff[i] - c_coeff[i - 1]) / h;
    }
  };

  std::vector<spline_t<T>> _spline;
  function_type<T> _fn;
  size_t _n;
  T _a;
  T _b;
  bool _value_changed;
};
}