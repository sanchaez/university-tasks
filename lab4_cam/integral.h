#pragma once
#include <cmath>
#include <functional>

namespace integral
{
template <typename T>
using function_type = std::function<T(T)>;

template <typename T>
class RangedIntegral
{
public:
  RangedIntegral() = default;
  RangedIntegral(const function_type<T>& function, const T& hstep)
      : _fn(function), _hstep(hstep){};
  T operator()(const T& a, const T& b)
  {
    unsigned long long n            = (b - a) / h;
    T                  result_value = (_fn(a) + _fn(b)) / 2.0;
    h                               = (b - a) / n;
    for (unsigned long long i = 1; i < n; ++i)
    {
      result_value += func(a + i * h);
    }
    result_value *= h;
    return result_value;
  }

  void set_fn(const function_type<T>& function) { _fn = function; }
  void set_step(const T& hstep) { _hstep = hstep; }
  const function_type<T> fn() { return _fn; }
  const T step() { return _hstep; }

private:
  function_type<T> _fn;
  T                _hstep;
};

template <typename T>
class RangedIntegralExact
{
public:
  RangedIntegralExact() = default;
  RangedIntegralExact(const function_type<T>& antiderivative) : _fn(function){};
  T operator()(const T& a, const T& b)
  {
    return _fn(b) - _fn(a);
  }

  void set_fn(const function_type<T>& function) { _fn = function; }
  const function_type<T>              fn() { return _fn; }

private:
  function_type<T> _fn;
};
}
