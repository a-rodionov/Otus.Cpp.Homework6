#pragma once

#include <memory>
#include <set>
#include <array>
#include "tuple_metafunctions.h"

template<size_t dimensions, typename T, T default_value>
class Matrix {

  static_assert(dimensions > 1, "Matrix dimensions can't be less than 2.");

  template<typename Matrix>
  class MatrixProxy;

public:

  static constexpr size_t index_elements_count = dimensions;

  using outter_value_type = T;
  using index_element_type = std::size_t;
  using index_type = generate_tuple_t<index_element_type, index_elements_count>;
  using inner_value_type = decltype(std::tuple_cat(index_type{}, std::make_tuple(T{})));
  using storage_type = std::set<inner_value_type, tuple_n_less<dimensions, inner_value_type>>;
  //using proxy_type = MatrixProxy<Matrix, index_type>;
  //using value_type = proxy_type;
  //using const_proxy_type = MatrixProxy<const Matrix, index_type>;
  //using const_value_type = const_proxy_type;

  auto size() const {
    return values.size();
  }

  auto operator[] (std::size_t index) {
    return MatrixProxy<Matrix>{*this, index};
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

  template<typename Matrix>
  class MatrixProxy
  {

  public:

    MatrixProxy(Matrix& matrix, std::size_t index)
      : matrix{matrix}, index_position{} {
        SetIndex(index);
      }

    operator typename Matrix::outter_value_type() const {
      if(index_position != Matrix::index_elements_count)
        throw std::logic_error("");
      return matrix.GetValue(make_tuple_from_container(inner_index));
    }

    auto operator[](std::size_t index) const {
      auto matrixProxy{*this};
      matrixProxy.SetIndex(index);
      return matrixProxy;
    }

    auto& operator[](std::size_t index) {
      SetIndex(index);
      return *this;
    }

    auto& operator=(const typename Matrix::outter_value_type& value) {
      if(index_position != Matrix::index_elements_count)
        throw std::logic_error("");
      matrix.SetValue(make_tuple_from_container(inner_index), value);
      return *this;
    }

    auto operator==(const typename Matrix::outter_value_type& value) const {
      if(index_position != Matrix::index_elements_count)
        throw std::logic_error("");
      return value == matrix.GetValue(make_tuple_from_container(inner_index));
    }

    friend bool operator==(const typename Matrix::outter_value_type& lhs, const MatrixProxy& rhs) {
      return rhs == lhs;
    };

  private:

    void SetIndex(std::size_t index) {
      if(index_position == Matrix::index_elements_count)
        inner_index[index_position-1] = index;
      else
        inner_index[index_position++] = index;
    }

    Matrix& matrix;
    size_t index_position;
    std::array<typename Matrix::index_element_type, Matrix::index_elements_count> inner_index;
  };

};
