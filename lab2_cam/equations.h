#pragma once
#include <functional>
#include <cmath>
namespace eq {
	template <typename FuncType>
	class Equation {
	public:
		Equation() = delete;
		explicit Equation(const FuncType func_default, 
					      const FuncType derivative_default, 
						  double range_low, double range_high);
		double operator()(double x);
		double derivative(double x);
		double iterative(double x, double accuracy);
		double hordes(double x, double accuracy);
		double get_iterations();
	private:
		double lambda();
		const FuncType _func, _derivative;
		double _range_a;
		double _range_b;
		int _iterations_count;
	};
}