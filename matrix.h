#pragma once

#include <memory>
#include <map>
#include <tuple>
#include "matrix_proxy.h"
#include "matrix_iterator.h"

template<size_t dimensions, typename T>
class Matrix {

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

  friend class MatrixProxy<Matrix, index_type>;
  friend class MatrixProxy<const Matrix, index_type>;

  explicit Matrix(const matrix_inner_value_type& default_value)
    : default_value{default_value} {}

  explicit Matrix(matrix_inner_value_type&& default_value)
    : default_value{std::forward<matrix_inner_value_type>(default_value)} {}

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

  matrix_inner_value_type default_value;
  storage_type values;

};
