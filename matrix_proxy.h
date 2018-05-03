#pragma once

#include "tuple_metafunctions.h"

template<typename Matrix, typename proxy_index_type>
class MatrixProxy
{

public:

  using next_proxy_index_type = tuple_concat_front_t<std::size_t, proxy_index_type>;

  constexpr static bool is_final_dimension = std::is_same<typename Matrix::index_type, proxy_index_type>::value;

  MatrixProxy(Matrix& matrix, const proxy_index_type& proxy_index)
    : matrix{matrix}, proxy_index{proxy_index} {}

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  operator typename Matrix::matrix_inner_value_type() const {
    return matrix.GetValue(proxy_index);
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  operator typename Matrix::ret_type() const {
    return std::tuple_cat(proxy_index, std::make_tuple(matrix.GetValue(proxy_index)));
  }

  template<typename T = MatrixProxy, std::enable_if_t<!T::is_final_dimension, int> = 0>
  auto operator[](std::size_t index) const {
    return MatrixProxy<Matrix, next_proxy_index_type>{matrix, std::tuple_cat(proxy_index, std::make_tuple(index))};
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  auto operator[](std::size_t index) const {
    std::get<std::tuple_size<T>::value - 1>(proxy_index) = index;
    return matrix.GetValue(proxy_index);
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  auto& operator[](std::size_t index) {
    std::get<std::tuple_size<T>::value - 1>(proxy_index) = index;
    return *this;
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  auto& operator=(const typename Matrix::matrix_inner_value_type& value) {
    matrix.SetValue(proxy_index, value);
    return *this;
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  auto operator==(const typename Matrix::matrix_inner_value_type& value) const {
    return value == matrix.GetValue(proxy_index);
  }

  friend bool operator==(const typename Matrix::matrix_inner_value_type& lhs, const MatrixProxy& rhs) {
    return rhs == lhs;
  };

private:

  Matrix& matrix;
  proxy_index_type proxy_index;
};
