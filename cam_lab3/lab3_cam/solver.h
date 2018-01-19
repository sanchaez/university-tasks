#pragma once
#include <valarray>
#include "print_helpers.h"

namespace solver {

/*
  System of linear equations solver
  all functions assume that given std::valarray has
  identical row sizes and is interpreted as a matrix.
*/

// types
template <typename T>
using row_type = std::valarray<T>;
template <typename T>
using result_type = row_type<T>;
template <typename T>
using matrix_type = std::valarray<row_type<T>>;
using index_type = std::size_t;
using row_indexes_type = row_type<index_type>;
using row_mask_type = row_type<bool>;

// main element selection method
template <typename T = double>
result_type<T> selection(const matrix_type<T>& coeff_matrix) {
  // copy input
  matrix_type<T> input_matrix = coeff_matrix;
  const index_type roots_number = input_matrix.size();
  row_mask_type used_rows_mask(true, input_matrix.size()),
      used_columns_mask(true, input_matrix[0].size());

  // get triangle matrix using main element selection
  matrix_type<T> sorted_triangle_matrix(roots_number);
  row_indexes_type deleted_columns(roots_number);

  for (index_type triangle_matrix_index = roots_number - 1;
       triangle_matrix_index != static_cast<index_type>(-1);
       --triangle_matrix_index) {
    // get max element pos (get main row)
    index_type first_index_i = 0, first_index_j = 0;
    while (!used_rows_mask[first_index_i]) {
      ++first_index_i;
    }
    while (!used_columns_mask[first_index_j]) {
      ++first_index_j;
    }

    index_type max_row_pos = first_index_i, max_column_pos = first_index_j;
    T max_value = input_matrix[first_index_i][first_index_j];
    for (index_type i = first_index_i; i < input_matrix.size(); ++i) {
      if (used_rows_mask[i]) {
        for (index_type j = first_index_j; j < input_matrix[i].size() - 1;
             ++j) {
          if (used_columns_mask[j] && input_matrix[i][j] > max_value) {
            max_value = input_matrix[i][j];
            max_row_pos = i;
            max_column_pos = j;
          }
        }
      }
    }

    // save main row
    sorted_triangle_matrix[triangle_matrix_index] = input_matrix[max_row_pos];
    deleted_columns[triangle_matrix_index] = max_column_pos;

    // add main row to mask
    used_rows_mask[max_row_pos] = false;
    // substract calculated row from main matrix rows
    for (index_type idx = 0; idx < input_matrix.size(); ++idx) {
      if (used_rows_mask[idx]) {
        auto buffer = static_cast<row_type<T>>(
                          input_matrix[max_row_pos][used_columns_mask]) *
                      (input_matrix[idx][max_column_pos] / (max_value));
        input_matrix[idx][used_columns_mask] -= buffer;
      }
    }

    // add main column to mask
    used_columns_mask[max_column_pos] = false;
  }

  // calculate roots
  result_type<T> roots(T(0), roots_number);
  for (index_type i = 0; i < roots_number; ++i) {
    auto column_size = sorted_triangle_matrix[i].size() - 1;
    T buffer = sorted_triangle_matrix[i][column_size];

    for (index_type column_index = 0; column_index < column_size;
         ++column_index) {
      if (column_index != deleted_columns[i]) {
        buffer -= roots[column_index] * sorted_triangle_matrix[i][column_index];
      }
    }
    roots[deleted_columns[i]] =
        buffer / sorted_triangle_matrix[i][deleted_columns[i]];
  }
  return roots;
}

// simple iterations method
// diagonal elements assumed to be nonzero
template <typename T = double>
result_type<T> iteration(const matrix_type<T>& coeff_matrix, const T& eps) {
  matrix_type<T> transformed_matrix = prepare_iteration(coeff_matrix);
  const index_type roots_number = transformed_matrix.size();


  T q = m_norm_it(transformed_matrix);
  if (q >= 1) throw std::exception("Matrix m norm more or equal than 1");

  result_type<T> beta_vector(roots_number);
  for (index_type i = 0; i < roots_number; ++i) {
    beta_vector[i] = transformed_matrix[i][0];
  }
  result_type<bool> alpha_values_mask(true, roots_number + 1);
  alpha_values_mask[0] = false;
  result_type<T> tmp_roots(T(0), roots_number);
  result_type<T> roots(beta_vector);

  const double precision_constant = ((1 - q) / q) * eps;
  double norm_of_difference;
  do {
    tmp_roots = beta_vector;
    for (index_type i = 0; i < roots_number; ++i) {
      tmp_roots[i] += static_cast<result_type<T>>(roots *
        static_cast<result_type<T>>(transformed_matrix[i][alpha_values_mask])).sum();
    }
    norm_of_difference = abs(static_cast<row_type<T>>(roots - tmp_roots)).sum();
    roots = tmp_roots;
  } while (norm_of_difference > precision_constant);
  return roots;
}

// private fn
namespace {
template <typename T = double>
matrix_type<T> prepare_iteration(const matrix_type<T>& coeff_matrix) {
  const index_type matrix_rows = coeff_matrix.size();
  const index_type matrix_columns = coeff_matrix[0].size();
  matrix_type<T> transformed_matrix(row_type<T>(matrix_columns), matrix_rows);
  for (index_type i = 0; i < matrix_rows; ++i) {
    transformed_matrix[i][0] =
        coeff_matrix[i][matrix_columns - 1] / coeff_matrix[i][i];
    for (index_type j = 0; j < matrix_columns - 1; ++j) {
      if (i != (j)) {
        transformed_matrix[i][j + 1] = -coeff_matrix[i][j] / coeff_matrix[i][i];
      } else {
        transformed_matrix[i][j + 1] = 0;
      }
    }
  }
  return transformed_matrix;
}

template <typename T = double>
T m_norm_it(matrix_type<T> matrix) {
  const auto roots_num = matrix.size();
  row_type<T> m_norm_sums(roots_num);
  row_type<bool> mask(true, roots_num + 1);
  mask[0] = false;
  for (index_type i = 0; i < roots_num; ++i) {
    m_norm_sums[i] = abs(static_cast<row_type<T>>(matrix[i][mask])).sum();
  }
  return m_norm_sums.max();
}
}
}
