#include "csv.h"
#include "print_helpers.h"
#include "solver.h"
#include <fstream>
#include <iostream>

// TODO: experiment with iteration method described here (second method)
//: http://orloff.am.tpu.ru/chisl_metod/Lab3/iter.htm

// exact solution : {3 , 3 , 3 , 1}
const solver::matrix_type<double> my_matrix = {{13, 14, 17, 14, 146},
                                               {9, 26, 7, 9, 135},
                                               {8, 4, 24, 11, 119},
                                               {15, 11, 5, 16, 109}};

const solver::matrix_type<double> iteration_matrix = {{13, 2, 3, 4, 146},
                                                      {9, 26, 7, 9, 135},
                                                      {8, 4, 24, 11, 119},
                                                      {8, 11, 5, 36, 109}};

const unsigned int iterations_count_max = 26;

solver::result_type<double>
calculation_error(const solver::matrix_type<double>& matrix,
                  const solver::result_type<double>& result)
{
  solver::index_type          matrix_size = matrix.size();
  solver::index_type          result_size = result.size();
  solver::result_type<double> errors(result_size);

  for (solver::index_type i = 0; i < matrix_size; ++i)
  {
    errors[i] = matrix[i][result_size];
    for (solver::index_type j = 0; j < result_size; ++j)
    {
      errors[i] -= matrix[i][j] * result[j];
    }
  }
  return errors.apply(fabs);
}

int main()
{
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
  fixed_width_print_line(calculation_error(iteration_matrix, y));

  std::cout << std::endl << "Roots (iteration):" << std::endl;
  fixed_width_print_obj("it:", 5);
  fixed_width_print_line({":x1", ":x2", ":x3", ":x4"});
  auto transformed_iteration_matrix = solver::prepare_iteration(iteration_matrix);
  for (unsigned int iterations_count = 1;
       iterations_count < iterations_count_max; iterations_count += 2)
  {
    auto z = solver::iteration(transformed_iteration_matrix, iterations_count);
    fixed_width_print_obj(iterations_count, 5);
    fixed_width_print_line(z);
    fixed_width_print_obj("err:", 5);
    fixed_width_print_line(calculation_error(iteration_matrix, z));
  }

  std::cin.get();
  return 0;
}
