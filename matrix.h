#pragma once

#include <memory>
#include <map>
#include <tuple>
#include "matrix_iterator.h"
#include "tuple_metafunctions.h"

template<size_t dimensions, typename T, T default_value>
class Matrix {

  static_assert(dimensions > 1, "Matrix dimensions can't be less than 2.");

  template<typename Matrix, typename proxy_index_type>
  class MatrixProxy;

public:

  using matrix_inner_value_type = T;
  using index_type = generate_tuple_t<std::size_t, dimensions>;
  using storage_type = std::map<index_type, T>;
  using ret_type = tuple_concat_back_t<T, index_type>;
  using proxy_type = MatrixProxy<Matrix, index_type>;
  using value_type = proxy_type;
  using const_proxy_type = MatrixProxy<const Matrix, index_type>;
  using const_value_type = const_proxy_type;

  using iterator = MatrixIterator<Matrix>;
  using const_iterator = MatrixConstIterator<Matrix>;

  auto size() const {
    return values.size();
  }

  auto operator[] (std::size_t index) {
    return MatrixProxy<Matrix, std::tuple<std::size_t>>{*this, std::make_tuple(index)};
  }

  iterator begin() noexcept
  {
    return iterator{*this, std::begin(values), std::end(values)};
  }

  const_iterator begin() const noexcept
  {
    return const_iterator{*this, std::cbegin(values), std::cend(values)};
  }

  iterator end() noexcept
  {
    return iterator{*this, std::end(values), std::end(values)};
  }

  const_iterator end() const noexcept
  {
    return const_iterator{*this, std::cend(values), std::cend(values)};
  }

  const_iterator cbegin() const noexcept
  {
    return const_iterator{*this, std::cbegin(values), std::cend(values)};
  }

  const_iterator cend() const noexcept
  {
    return const_iterator{*this, std::cend(values), std::cend(values)};
  }

private:

  void SetValue(const index_type& index, const matrix_inner_value_type& value) {
    if(default_value == value)
      values.erase(index);
    else
      values[index] = value;
  }

  matrix_inner_value_type GetValue(const index_type& index) const {
    auto value = values.find(index);
    if(std::cend(values) == value)
      return default_value;
    return value->second;
  }

  storage_type values;

  template<typename Matrix, typename proxy_index_type>
  class MatrixProxy
  {

  public:

    using next_proxy_index_type = tuple_concat_front_t<std::size_t, proxy_index_type>;

    constexpr static bool is_final_dimension = std::is_same<typename Matrix::index_type, proxy_index_type>::value;

    MatrixProxy(Matrix& matrix, const proxy_index_type& proxy_index)
      : matrix{matrix}, proxy_index{proxy_index} {}

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    operator typename Matrix::matrix_inner_value_type() const {
      return matrix.GetValue(proxy_index);
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    operator typename Matrix::ret_type() const {
      return std::tuple_cat(proxy_index, std::make_tuple(matrix.GetValue(proxy_index)));
    }

    template<typename U = MatrixProxy, std::enable_if_t<!U::is_final_dimension, int> = 0>
    auto operator[](std::size_t index) const {
      return MatrixProxy<Matrix, next_proxy_index_type>{matrix, std::tuple_cat(proxy_index, std::make_tuple(index))};
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    auto operator[](std::size_t index) const {
      std::get<std::tuple_size<U>::value - 1>(proxy_index) = index;
      return matrix.GetValue(proxy_index);
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    auto& operator[](std::size_t index) {
      std::get<std::tuple_size<U>::value - 1>(proxy_index) = index;
      return *this;
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    auto& operator=(const typename Matrix::matrix_inner_value_type& value) {
      matrix.SetValue(proxy_index, value);
      return *this;
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
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

};
