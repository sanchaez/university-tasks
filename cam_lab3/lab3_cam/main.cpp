#include <fstream>
#include <iostream>
#include "print_helpers.h"
#include "solver.h"

// TODO: experiment with iteration method described here (second method)
//: http://orloff.am.tpu.ru/chisl_metod/Lab3/iter.htm

// exact solution : {3 , 3 , 3 , 1}
const solver::matrix_type<double> my_matrix = {{13, 14, 17, 14, 146},
                                               {9, 26, 7, 9, 135},
                                               {8, 4, 24, 11, 119},
                                               {15, 11, 5, 16, 109}};

//testing trivial matrix
// {1,1,1,1}
const solver::matrix_type<double> iteration_matrix = {
    {100, 5, 30, 11, 146}, {5, 100, 20, 10, 135}, {5, 10, 100, 4, 119}, {5, 9, 5, 90, 109}};

const unsigned int iterations_count_max = 100;

solver::result_type<double> calculation_error(
    const solver::matrix_type<double>& matrix,
    const solver::result_type<double>& result) {
  solver::index_type matrix_size = matrix.size();
  solver::index_type result_size = result.size();
  solver::result_type<double> errors(result_size);

  for (solver::index_type i = 0; i < matrix_size; ++i) {
    errors[i] = matrix[i][result_size];
    for (solver::index_type j = 0; j < result_size; ++j) {
      errors[i] -= matrix[i][j] * result[j];
    }
  }
  return abs(errors);
}

int main() {
  std::cout << "Input matrix" << std::endl;
  fixed_width_print_line({":ai1", ":ai2", ":ai3", ":ai4", ":bi"}, 10);
  fixed_width_print_matrix(my_matrix, 10);

  std::cout << std::endl << "Roots (selection method):" << std::endl;
  auto x = solver::selection(my_matrix);
  fixed_width_print_obj("x:", 3);
  fixed_width_print_line(x);
  fixed_width_print_obj("e:", 3);
  fixed_width_print_line(calculation_error(my_matrix, x));

  std::cout << std::endl << "Input matrix" << std::endl;
  fixed_width_print_line({":ai1", ":ai2", ":ai3", ":ai4", ":bi"}, 10);
  fixed_width_print_matrix(iteration_matrix, 10);

  std::cout << std::endl << "Roots (selection method):" << std::endl;
  auto y = solver::selection(iteration_matrix);
  fixed_width_print_obj("x:", 5);
  fixed_width_print_line(y);
  fixed_width_print_obj("e:", 5);
  auto eps = calculation_error(iteration_matrix, y);
  fixed_width_print_line(eps);

  auto eps_iter = eps.min();
  //fallback
  if (!eps_iter) {
    eps_iter = 1e-15;
  }
  std::cout << std::endl
            << "Roots (iteration): epsilon = " << eps_iter << std::endl;
  auto z = solver::iteration(iteration_matrix, eps_iter);
  fixed_width_print_obj("x:", 5);
  fixed_width_print_line(z);
  fixed_width_print_obj("e:", 5);
  fixed_width_print_line(calculation_error(iteration_matrix, z));
  std::cin.get();
  return 0;
}
