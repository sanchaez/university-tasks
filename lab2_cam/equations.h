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
			const double& range_low, const double& range_high)
		{
			_func = func_default;
			_derivative = derivative_default;
			_range_a = range_low;
			_range_b = range_high;
		}
		double	operator()(const double& x) { return _func(x); }
		double	derivative(const double& x) { return _derivative(x); }
		int		get_last_iterations()	const { return _iterations_count; }
		double get_last_error() { return _error; }
		void set_range(const double &a, const double &b) { _range_a = a; _range_b = b; }
		double get_range_a() const { return _range_a; }
		double get_range_b() const { return _range_b; }
		//
		double iterative(const double& precision) {
			const double lambda_constant = lambda_factor();
			const double q_constant = q_factor();

			std::function<double(double)> phi_function([&](double x) { return x - lambda_constant * _func(x); });

			const double precision_constant = abs((1 - q_constant) / q_constant * precision);
			double x0 = (_range_b - _range_a) / 1.7,
				xn = phi_function(x0);
			_iterations_count = 1;
			_error = abs(xn - x0);
			while (_error > precision_constant) {
				x0 = xn;
				xn = phi_function(xn);
				++_iterations_count;
				_error = abs(xn - x0);
			};
			x0 = xn;
			xn = phi_function(xn);
			_error = abs(xn - x0);
			return xn;
		}

		double hordes(const double& precision) {
			std::function<double(double, double)> iteration_function(\
				[&](double x, double c) { return x - (_func(x) * (x - c)) / (_func(x) - _func(c)); });

			double x0 = _range_b,
				xn = iteration_function(_range_b, _range_a);
			const double min_derivative_range_constant = min_derivative_range();
			_iterations_count = 1;
			_error = abs(_func(xn) / min_derivative_range_constant);
			while (_error > precision) {
				x0 = xn;
				xn = iteration_function(xn, _range_b);
				++_iterations_count;
				_error = abs(_func(xn) / min_derivative_range_constant);
			}
			return xn;
		}
	private:
		double min_derivative_range() {
			double min_a = _derivative(_range_a),
				min_b = _derivative(_range_b);
			return (min_a <= min_b) ? min_a : min_b;
		}

		double max_derivative_range() {
			double max_a = _derivative(_range_a),
				max_b = _derivative(_range_b);
			return (max_a >= max_b) ? max_a : max_b;
		}

		double lambda_factor() {
			return 1 / max_derivative_range();
		};

		double q_factor() {
			return 1 - min_derivative_range() / max_derivative_range();
		}
		FuncType *_func, *_derivative;
		double _range_a;
		double _range_b;
		int _iterations_count;
		double _error;
	};
}