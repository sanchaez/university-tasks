#pragma once
#include <functional>
#include <cmath>
namespace eq {
	template <typename FuncType>
	class Equation {
	public:
		Equation() = delete;
		explicit Equation(const FuncType *func_default, const FuncType *derivative_default);
		double operator()(double x);
		double derivative(double x);
		double iterative(double x, double accuracy);
		double hordes(double x, double accuracy);
	private:
		double lambda();
		const FuncType *_func, *_derivative;
		static const double range_a = 0;
		static const double range_b = 1;
		int _iterations_count;
	};
}