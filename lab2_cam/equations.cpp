#include "equations.h"
template<typename FuncType>
eq::Equation<FuncType>::Equation(const FuncType func_default, 
						const FuncType derivative_default, 
						double range_low, double range_high)
{
	_func = func;
	_derivative = derivative;
	_range_a = range_low;
	_range_b = range_high;
}

template<typename FuncType>
double eq::Equation<FuncType>::operator()(double x)
{
	return _func(x);
}

template<typename FuncType>
double eq::Equation<FuncType>::derivative(double x)
{
	return _func(x);
}

template<typename FuncType>
double eq::Equation<FuncType>::iterative(double x, double accuracy)
{
	return 0.0;
}

template<typename FuncType>
double eq::Equation<FuncType>::hordes(double x, double accuracy)
{
	return 0.0;
}

template<typename FuncType>
double eq::Equation<FuncType>::get_iterations()
{
	return _iterations_count; 
}

template<typename FuncType>
double eq::Equation<FuncType>::lambda()
{
	double max_a = derivative(_range_a), max_b = derivative(_range_b);
	return (max_a >= max_b) ? 1 / max_a : 1 / max_b;
}
