#pragma once

#include <memory>
#include <set>
#include "tuple_metafunctions.h"

template<size_t dimensions, typename T, T default_value>
class Matrix {

  static_assert(dimensions > 1, "Matrix dimensions can't be less than 2.");

  template<typename Matrix, typename proxy_index_type>
  class MatrixProxy;

public:

  using outter_value_type = T;
  using index_type = generate_tuple_t<std::size_t, dimensions>;
  using data_type = decltype(std::tuple_cat(index_type{}, std::make_tuple(T{})));
  using storage_type = std::set<data_type, tuple_n_less<dimensions, data_type>>;
  using proxy_type = MatrixProxy<Matrix, index_type>;
  //using value_type = proxy_type;
  //using const_proxy_type = MatrixProxy<const Matrix, index_type>;
  //using const_value_type = const_proxy_type;

  auto size() const {
    return values.size();
  }

  auto operator[] (std::size_t index) {
    return MatrixProxy<Matrix, std::tuple<std::size_t>>{*this, std::make_tuple(index)};
  }

  auto begin() noexcept {
    return values.begin();
  }

  auto begin() const noexcept {
    return values.begin();
  }

  auto end() noexcept {
    return values.end();
  }

  auto end() const noexcept {
    return values.end();
  }

  auto cbegin() const noexcept {
    return values.cbegin();
  }

  auto cend() const noexcept {
    return values.cend();
  }

private:

  void SetValue(const index_type& index, const outter_value_type& value) {
    auto inner_value = std::tuple_cat(index, std::make_tuple(value));
    auto position = values.find(inner_value);

    if(std::cend(values) != position) {
      values.erase(position);
    }

    if(default_value != value)
      values.insert(inner_value);
/*
    if(default_value == value) {
      if(std::cend(values) != position)
        values.erase(position);
    }
    else {
      if(std::cend(values) == position)
        values.insert(inner_value);
      else
        std::get<dimensions>(*position) = value;
    }
*/
  }

  outter_value_type GetValue(const index_type& index) const {
    auto inner_value = std::tuple_cat(index, std::make_tuple(T{}));
    auto position = values.find(inner_value);
    if(std::cend(values) == position)
      return default_value;
    return std::get<dimensions>(*position);
  }

  storage_type values;

  template<typename Matrix, typename proxy_index_type>
  class MatrixProxy
  {

  public:

    using next_proxy_index_type = decltype(std::tuple_cat(proxy_index_type{}, std::make_tuple(std::size_t{})));

    constexpr static bool is_final_dimension = std::is_same<typename Matrix::index_type, proxy_index_type>::value;

    MatrixProxy(Matrix& matrix, const proxy_index_type& proxy_index)
      : matrix{matrix}, proxy_index{proxy_index} {}

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    operator typename Matrix::outter_value_type() const {
      return matrix.GetValue(proxy_index);
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    operator typename Matrix::data_type() const {
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
    auto& operator=(const typename Matrix::outter_value_type& value) {
      matrix.SetValue(proxy_index, value);
      return *this;
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    auto operator==(const typename Matrix::outter_value_type& value) const {
      return value == matrix.GetValue(proxy_index);
    }

    friend bool operator==(const typename Matrix::outter_value_type& lhs, const MatrixProxy& rhs) {
      return rhs == lhs;
    };

  private:

    Matrix& matrix;
    proxy_index_type proxy_index;
  };

};
