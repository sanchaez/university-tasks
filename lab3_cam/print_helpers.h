#pragma once
#include <initializer_list>
#include <iostream>
#include <iomanip>

template <class T>
inline void fixed_width_print_obj(T obj, const int& width = 15, const int& precision = 15) {
	std::cout << std::left << std::setw(width) << std::setprecision(precision) << obj;
}

template <class T>
void fixed_width_print_line(std::initializer_list<T> list, const int& width = 20) {
	for (T obj : list) {
		fixed_width_print_obj<T>(obj, width);
	}
	std::cout << std::endl;
}