#pragma once
#include <cmath>
#include <functional>

namespace integral {
template <typename T>
using function_type = std::function<T(T)>;
using size_t = unsigned long long;

template <typename T>
class AbstractRangedIntegral {
 public:
  AbstractRangedIntegral(const function_type<T>& function) : _fn(function){};
  virtual T operator()(const T& a, const T& b) const { return T(); };
  constexpr function_type<T> fn() const { return _fn; }
  virtual void set_fn(const function_type<T>& function) { _fn = function; }

 protected:
  function_type<T> _fn;
};

template <typename T>
class AbstractTrapeziumIntegral : public AbstractRangedIntegral<T> {
  using AbstractRangedIntegral<T>::AbstractRangedIntegral;

 protected:
  virtual inline T calculate(const T& a, const T& b, size_t n) const {
    T result_value = (_fn(a) + _fn(b)) / static_cast<T>(2);
    T h = (b - a) / static_cast<T>(n);
    for (size_t i = 1; i < n; ++i) {
      result_value += _fn(a + static_cast<T>(i) * h);
    }
    result_value *= h;
    return result_value;
  }
};

template <typename T>
class RangedTrapeziumStep : public AbstractTrapeziumIntegral<T> {
 public:
  RangedTrapeziumStep(const function_type<T>& function, const T& hstep)
      : AbstractTrapeziumIntegral<T>(function), _hstep(hstep){};
  T operator()(const T& a, const T& b) const {
    return calculate(a, b, static_cast<size_t>((b - a) / _hstep));
  }
  void set_step(const T& hstep) { _hstep = hstep; }
  constexpr T step() const { return _hstep; }

 protected:
  T _hstep;
};

template <typename T>
class RangedIntegralExact : public AbstractRangedIntegral<T> {
 public:
  using AbstractRangedIntegral<T>::AbstractRangedIntegral;
  T operator()(const T& a, const T& b) const { return _fn(b) - _fn(a); }
};

template <typename T>
class RangedTrapeziumRunge : public AbstractTrapeziumIntegral<T> {
 public:
  RangedTrapeziumRunge(const function_type<T>& function, const T& eps)
      : AbstractTrapeziumIntegral<T>(function), _eps(eps) {}
  T operator()(const T& a, const T& b) {
    size_t n = static_cast<size_t>(1 / sqrt(_eps));
    T integral_result_n = calculate(a, b, n);
    n *= 2;
    T integral_result_2n = calculate(a, b, n);
    T rest = abs(integral_result_n - integral_result_2n) / static_cast<T>(3);
    while (rest > _eps) {
      n *= 2;
      integral_result_n = integral_result_2n;
      integral_result_2n = calculate(a, b, n);
      rest = abs(integral_result_n - integral_result_2n) / static_cast<T>(3);
    }
    _hstep = (b - a) / static_cast<T>(n);
    return integral_result_2n;
  };
  void set_eps(const T& eps) { _eps = eps; }
  constexpr T eps() const { return _eps; }
  constexpr T step() const { return _hstep; }

 protected:
  T _eps;
  T _hstep;

};
}
