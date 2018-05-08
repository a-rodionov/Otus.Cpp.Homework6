#pragma once

#include <memory>
#include <set>
#include <array>
#include "tuple_metafunctions.h"

template<size_t dimensions, typename T, T default_value>
class Matrix {

public:

  static constexpr size_t index_elements_count = dimensions;

  using outter_value_type = T;
  using index_element_type = std::size_t;
  using index_type = generate_tuple_t<index_element_type, index_elements_count>;
  using inner_value_type = decltype(std::tuple_cat(index_type{}, std::make_tuple(T{})));
  using storage_type = std::set<inner_value_type, tuple_n_less<index_elements_count, inner_value_type>>;

private:

  template<typename Matrix>
  struct MatrixStorage
  {
    void SetValue(const index_type& index, const outter_value_type& value) {
      auto inner_value = std::tuple_cat(index, std::make_tuple(value));
      auto position = values.find(inner_value);
      if(std::cend(values) != position)
        values.erase(position);
      if(default_value != value)
        values.insert(inner_value);
    }

    auto GetValue(const index_type& index) const {
      auto inner_value = std::tuple_cat(index, std::make_tuple(outter_value_type{}));
      auto position = values.find(inner_value);
      if(std::cend(values) == position)
        return default_value;
      return std::get<index_elements_count>(*position);
    }

    storage_type values;
  };

  using matrix_storage_type = MatrixStorage<Matrix>;

  template<typename Matrix, typename proxy_index_type>
  class MatrixProxy
  {

    using next_proxy_index_type = decltype(std::tuple_cat(proxy_index_type{}, std::make_tuple(std::size_t{})));

    constexpr static bool is_final_dimension = std::is_same<index_type, proxy_index_type>::value;

  public:

    MatrixProxy(const std::weak_ptr<matrix_storage_type>& matrix_storage, const proxy_index_type& proxy_index)
      : matrix_storage{matrix_storage}, inner_index{proxy_index} {}

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    operator outter_value_type() const {
      std::shared_ptr<matrix_storage_type> own_matrix_storage{matrix_storage};
      return own_matrix_storage->GetValue(inner_index);
    }

    template<typename U = MatrixProxy, std::enable_if_t<!U::is_final_dimension, int> = 0>
    auto operator[](std::size_t index) const {
      return MatrixProxy<Matrix, next_proxy_index_type>{matrix_storage, std::tuple_cat(inner_index, std::make_tuple(index))};
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    auto operator[](std::size_t index) const {
      auto matrixProxy{*this};
      std::get<std::tuple_size<proxy_index_type>::value - 1>(matrixProxy.inner_index) = index;
      return matrixProxy;
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    auto& operator[](std::size_t index) {
      std::get<std::tuple_size<proxy_index_type>::value - 1>(inner_index) = index;
      return *this;
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    auto& operator=(const outter_value_type& value) {
      std::shared_ptr<matrix_storage_type> own_matrix_storage{matrix_storage};
      own_matrix_storage->SetValue(inner_index, value);
      return *this;
    }

    template<typename U = MatrixProxy, std::enable_if_t<U::is_final_dimension, int> = 0>
    auto operator==(const outter_value_type& value) const {
      std::shared_ptr<matrix_storage_type> own_matrix_storage{matrix_storage};
      return value == own_matrix_storage->GetValue(inner_index);
    }

    friend bool operator==(const outter_value_type& lhs, const MatrixProxy& rhs) {
      return rhs == lhs;
    };

  private:

    std::weak_ptr<matrix_storage_type> matrix_storage;
    proxy_index_type inner_index;
  };

public:

  static_assert(dimensions > 1, "Matrix dimensions can't be less than 2.");

  Matrix()
    :matrix_storage{new matrix_storage_type{}} {}

  auto size() const {
    return matrix_storage->values.size();
  }

  auto operator[] (std::size_t index) const {
    return MatrixProxy<Matrix, std::tuple<std::size_t>>{matrix_storage, std::make_tuple(index)};
  }

  auto begin() noexcept {
    return matrix_storage->values.begin();
  }

  auto begin() const noexcept {
    return matrix_storage->values.begin();
  }

  auto end() noexcept {
    return matrix_storage->values.end();
  }

  auto end() const noexcept {
    return matrix_storage->values.end();
  }

  auto cbegin() const noexcept {
    return matrix_storage->values.cbegin();
  }

  auto cend() const noexcept {
    return matrix_storage->values.cend();
  }

private:

  std::shared_ptr<matrix_storage_type> matrix_storage;
};
