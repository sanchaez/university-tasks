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
result_type<T> iteration(const matrix_type<T>& transformed_matrix,
                         const unsigned int& iterations_num) {
  const index_type roots_number = transformed_matrix.size();
  result_type<T> roots(T(0), roots_number);
  // starting vector
  // Roots for iterations
  result_type<T> tmp_roots(T(0), roots_number);

  for (index_type i = 0; i < static_cast<index_type>(iterations_num); ++i) {
    for (index_type j = 0; j < roots_number; ++j) {
      tmp_roots[j] = transformed_matrix[j][0];
      for (index_type k = 0; k < roots_number; ++k) {
        tmp_roots[j] += roots[k] * transformed_matrix[j][k + 1];
      }
    }
    roots = tmp_roots;
  }
  return roots;
}

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
result_type<T> gauss_jordan(const matrix_type<T>& coeff_matrix) {
  matrix_type<T> matrix = coeff_matrix;
  index_type roots_num = matrix.size();

  // Direct path

  for (index_type i = 0; i < roots_num; ++i) {
    if (matrix[i][i] == static_cast<T>(0)) return result_type<T>();
    matrix[i] /= matrix[i][i];

    row_type<bool> masked(true, roots_num + 1);
    for (index_type mask_idx = 0; mask_idx < i; ++mask_idx) {
      masked[mask_idx] = true;
    }
    for (index_type k = 0; k < i; ++k) {
        matrix[k][masked] -= row_type<T>(matrix[i][masked]) * matrix[k][i];
    }
    for (index_type k = i + 1; k < roots_num; ++k) {
        matrix[k][masked] -= row_type<T>(matrix[i][masked]) * matrix[k][i];
    }
  }
  result_type<T> roots(roots_num);
  for (index_type i = 0; i < roots_num; ++i) {
    roots[i] = matrix[i][roots_num];
  }
  return roots;
}
}
