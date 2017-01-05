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
 public:
  CubicSpline(const function_type<T>& function, const size_t& nsteps,
              const T& a = 0, const T& b = 1)
      : _fn(function), _n(nsteps), _a(a), _b(b), _value_changed(false) {
    rebuild_spline();
  };
  T operator()(const T& x) const {
    const spline_t<T>* used_spline = nullptr;
    if (x <= _spline[0].x) {
      used_spline = &_spline[0];
    } else if (x >= _spline[_n - 1].x) {
      used_spline = &_spline[_n - 1];
    } else {
      // binary search
      size_t i = 0, j = _n - 1;
      while (i + 1 < j) {
        size_t k = i + (j - i) / 2;
        if (x <= _spline[k].x) {
          j = k;
        } else {
          i = k;
        }
      }
      used_spline = &_spline[j];
    }
    // return value
    T delta = x - used_spline->x;
    return      used_spline->a +
      delta * (used_spline->b +
        delta * (used_spline->c / static_cast<T>(2) +
          delta * used_spline->d / static_cast<T>(6)));;
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

  void rebuild_spline() {
    solver::matrix_type<T> c_matrix =
        solver::matrix_type<T>(solver::row_type<T>(static_cast<T>(0), _n + 1), _n);
    _spline.clear();
    _spline.resize(_n);

    // filling matrix
    T h;
    if (a < 0 && b > 0) {
      h = (abs(a) + abs(b)) / static_cast<T>(_n);
    } else {
      h = abs(b - a) / static_cast<T>(_n);
    }

    //fill x and y table
    _spline[0].x = _a;
    _spline[0].a = _fn(_a);
    for (size_t i = 1; i < _n - 1; ++i) {
      _spline[i].x = _spline[i - 1].x + h;
      _spline[i].a = _fn(_spline[i].x);
    }
    _spline[_n - 1].x = _b;
    _spline[_n - 1].a = _fn(_b);

    // fill diagonal matrix
    // c_0 = 0; c_n = 0;
    // using relative distance
    c_matrix[0][0] = static_cast<T>(1);
    for (size_t i = 1; i < _n - 1; ++i) {
      c_matrix[i][i - 1] = _spline[i].x - _spline[i - 1].x;
      c_matrix[i][i + 1] = _spline[i + 1].x - _spline[i].x;
      c_matrix[i][i] = 2 * (c_matrix[i][i - 1] + c_matrix[i][i + 1]);
      c_matrix[i][_n] =
          6 *((_spline[i + 1].a - _spline[i].a) / c_matrix[i][i + 1] 
            - (_spline[i].a - _spline[i - 1].a) / c_matrix[i][i - 1]);
    }
    c_matrix[_n - 1][_n - 1] = static_cast<T>(1);

    // filing spline with matrix solution
    _spline[0].b = 0;//_spline[0].a / h;
    _spline[0].c = 0;
    _spline[0].d = 0;

    solver::result_type<T> c_result = solver::gauss_jordan(c_matrix);
    //fallback
    if (!c_result.size()) {
      c_result = solver::selection(c_matrix);
    }
    for (size_t i = 1; i < _n; ++i) {
      const auto h_i = _spline[i].x - _spline[i - 1].x;
      _spline[i].c = c_result[i];
      _spline[i].d = (_spline[i].c - _spline[i-1].c) / h_i;
      _spline[i].b = 
        h_i * (2*_spline[i].c + _spline[i - 1].c) / static_cast<T>(6)
        + (_spline[i].a - _spline[i - 1].a) / h_i;
    }
  };

 protected:
  function_type<T> _fn;
  size_t _n;
  T _a;
  T _b;
  bool _value_changed;

 private:
  std::vector<spline_t<T>> _spline;
};
}