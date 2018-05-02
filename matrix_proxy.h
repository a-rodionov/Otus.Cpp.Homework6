#pragma once

#include "tuple_metafunctions.h"

template<typename Matrix, typename proxy_index_type>
class MatrixProxy
{

public:

  using next_proxy_index_type = tuple_concat_front_t<std::size_t, proxy_index_type>;

  constexpr static const bool is_final_dimension = std::is_same<typename Matrix::index_type, proxy_index_type>::value;

  MatrixProxy(Matrix& matrix, const proxy_index_type& proxy_index)
    : _matrix{matrix}, _proxy_index{proxy_index} {}

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  operator typename Matrix::matrix_inner_value_type() const {
    return _matrix.GetValue(_proxy_index);
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  operator typename Matrix::ret_type() const {
    return std::tuple_cat(_proxy_index, std::make_tuple(_matrix.GetValue(_proxy_index)));
  }

  template<typename T = MatrixProxy, std::enable_if_t<!T::is_final_dimension, int> = 0>
  auto operator[](std::size_t index) const {
    return MatrixProxy<Matrix, next_proxy_index_type>{_matrix, std::tuple_cat(_proxy_index, std::make_tuple(index))};
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  auto operator[](std::size_t index) const {
    std::get<std::tuple_size<T>::value - 1>(_proxy_index) = index;
    return _matrix.GetValue(_proxy_index);
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  auto& operator[](std::size_t index) {
    std::get<std::tuple_size<T>::value - 1>(_proxy_index) = index;
    return *this;
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  auto& operator=(const typename Matrix::matrix_inner_value_type& value) {
    _matrix.SetValue(_proxy_index, value);
    return *this;
  }

  template<typename T = MatrixProxy, std::enable_if_t<T::is_final_dimension, int> = 0>
  auto operator==(const typename Matrix::matrix_inner_value_type& value) const {
    return value == _matrix.GetValue(_proxy_index);
  }

  friend bool operator==(const typename Matrix::matrix_inner_value_type &lhs, const MatrixProxy &rhs) {
    return rhs == lhs;
  };

private:

  Matrix& _matrix;
  proxy_index_type _proxy_index;
};
