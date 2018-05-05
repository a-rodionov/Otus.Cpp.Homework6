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

  template<typename Matrix>
  class MatrixProxy
  {

  public:

    MatrixProxy(const std::weak_ptr<matrix_storage_type>& matrix_storage, std::size_t index)
      : matrix_storage{matrix_storage}, index_position{} {
        SetIndex(index);
      }

    operator outter_value_type() const {
      if(index_position != index_elements_count)
        throw std::logic_error("The last dimension index isn't reached yet.");
      std::shared_ptr<matrix_storage_type> own_matrix_storage{matrix_storage};
      return own_matrix_storage->GetValue(make_tuple_from_container(inner_index));
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

    auto& operator=(const outter_value_type& value) {
      if(index_position != index_elements_count)
        throw std::logic_error("The last dimension index isn't reached yet.");
      std::shared_ptr<matrix_storage_type> own_matrix_storage{matrix_storage};
      own_matrix_storage->SetValue(make_tuple_from_container(inner_index), value);
      return *this;
    }

    auto operator==(const outter_value_type& value) const {
      if(index_position != index_elements_count)
        throw std::logic_error("The last dimension index isn't reached yet.");
      std::shared_ptr<matrix_storage_type> own_matrix_storage{matrix_storage};
      return value == own_matrix_storage->GetValue(make_tuple_from_container(inner_index));
    }

    friend bool operator==(const outter_value_type& lhs, const MatrixProxy& rhs) {
      return rhs == lhs;
    };

  private:

    void SetIndex(std::size_t index) {
      if(index_position == index_elements_count)
        inner_index[index_position-1] = index;
      else
        inner_index[index_position++] = index;
    }

    std::weak_ptr<matrix_storage_type> matrix_storage;
    size_t index_position;
    std::array<index_element_type, index_elements_count> inner_index;
  };

public:

  static_assert(dimensions > 1, "Matrix dimensions can't be less than 2.");

  Matrix()
    :matrix_storage{new matrix_storage_type{}} {}

  auto size() const {
    return matrix_storage->values.size();
  }

  auto operator[] (std::size_t index) {
    return MatrixProxy<Matrix>{matrix_storage, index};
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
