#pragma once
#include <functional>
#include <cmath>
namespace eq {
	template <class FuncType>
	class Equation {
	public:
		Equation() = delete;
		explicit Equation(FuncType func_default, 
					      FuncType derivative_default, 
						  double range_low, double range_high)
		{
			_func = func_default;
			_derivative = derivative_default;
			_range_a = range_low;
			_range_b = range_high;
		}
		double operator()(double x) { return _func(x); }
		double derivative(double x) { return _derivative(x); }
		double iterative(double x, double accuracy) {
			return 0.0;
		}
		double hordes(double x, double accuracy) {
			return 0.0;
		}
		double get_iterations()		{ return _iterations_count;	}
	private:
		double lambda() {
			double max_a = derivative(_range_a), 
				max_b = derivative(_range_b);
			return (max_a >= max_b) ? 1 / max_a : 1 / max_b;
		};
		FuncType *_func, *_derivative;
		double _range_a;
		double _range_b;
		int _iterations_count;
	};
}