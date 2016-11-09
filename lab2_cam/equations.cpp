#include "equations.h"

template<typename FuncType>
eq::Equation::Equation(const FuncType *func, const FuncType *derivative)
{
	_func = func;
	_derivative = derivative;
}

template<typename FuncType>
double eq::Equation::operator()(double x)
{
	return _func(x);
}

template<typename FuncType>
double eq::Equation::derivative(double x)
{
	return _func(x);
}

template<typename FuncType>
double eq::Equation::iterative(double x, double accuracy)
{
	return 0.0;
}

template<typename FuncType>
double eq::Equation::hordes(double x, double accuracy)
{
	return 0.0;
}

template<typename FuncType>
double eq::Equation<FuncType>::lambda()
{
	double max_a = derivative(range_a), max_b = derivative(range_b);
	return 0.0;
}
