#pragma once

#include <memory>
#include <set>
#include <array>
#include "tuple_metafunctions.h"

template<size_t dimensions, typename T, T default_value>
class Matrix {

public:

  using outter_value_type = T;
  using index_element_type = std::size_t;
  using index_type = generate_tuple_t<index_element_type, dimensions>;
  using inner_value_type = decltype(std::tuple_cat(index_type{}, std::make_tuple(T{})));
  using storage_type = std::set<inner_value_type, tuple_n_less<dimensions, inner_value_type>>;

private:

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
      return std::get<dimensions>(*position);
    }

    storage_type values;
  };

  using matrix_storage_type = MatrixStorage;

  template<typename Matrix, typename proxy_index_type>
  class MatrixProxy
  {

    using next_proxy_index_type = decltype(std::tuple_cat(proxy_index_type{}, std::make_tuple(std::size_t{})));

    constexpr static bool is_final_dimension = std::is_same<index_type, proxy_index_type>::value;

  public:

    MatrixProxy(const std::weak_ptr<matrix_storage_type>& matrix_storage, const proxy_index_type& proxy_index)
      : matrix_storage{matrix_storage}, inner_index{proxy_index} {}

    operator outter_value_type() const {
      static_assert(is_final_dimension, "Error getting value because current index value doesn't match required.");
      std::shared_ptr<matrix_storage_type> own_matrix_storage{matrix_storage};
      return own_matrix_storage->GetValue(inner_index);
    }

    auto operator[](std::size_t index) const {
      static_assert(!is_final_dimension, "Error indexing because index is already complete");
      return MatrixProxy<Matrix, next_proxy_index_type>{matrix_storage, std::tuple_cat(inner_index, std::make_tuple(index))};
    }

    auto& operator=(const outter_value_type& value) {
      static_assert(is_final_dimension, "Error assigning value because current index value doesn't match required.");
      std::shared_ptr<matrix_storage_type> own_matrix_storage{matrix_storage};
      own_matrix_storage->SetValue(inner_index, value);
      return *this;
    }

    auto operator==(const outter_value_type& value) const {
      static_assert(is_final_dimension, "Error comparing with value because current index value doesn't match required.");
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
