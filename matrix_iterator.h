#pragma once

template<typename Matrix>
class MatrixIterator
{

public:

  using Self = MatrixIterator<Matrix>;
  using inner_iterator = typename Matrix::storage_type::iterator;

  using value_type = typename Matrix::proxy_type;
  using pointer = typename Matrix::proxy_type*;
  using reference = typename Matrix::proxy_type&;
  using difference_type = typename Matrix::storage_type::difference_type;
  using iterator_category = std::forward_iterator_tag;

  explicit MatrixIterator(Matrix& matrix, const inner_iterator& position, const inner_iterator& end)
    : matrix{matrix}, position{position}, end{end} { GetValue(); }

  reference operator*() const
  {
    return *value.get();
  }

  pointer operator->() const
  {
    return value.get();
  }

  Self& operator++()
  {
    ++position;
    GetValue();
    return *this;
  }

  Self operator++(int)
  {
    Self tmp{matrix, position, end};
    ++position;
    GetValue();
    return tmp;
  }

  bool operator==(const Self& other) const
  {
    return position == other.position;
  }

  bool operator!=(const Self& other) const
  {
    return position != other.position;
  }

private:

  void GetValue() {
    if(position == end)
      value.reset(nullptr);
    else
      value.reset(new value_type{matrix, position->first});
  }

  Matrix& matrix;
  inner_iterator position;
  inner_iterator end;
  std::unique_ptr<value_type> value;
};

template<typename Matrix>
class MatrixConstIterator
{

public:

  using Self = MatrixConstIterator<Matrix>;
  using inner_iterator = typename Matrix::storage_type::const_iterator;

  using value_type = typename Matrix::const_proxy_type;
  using pointer = const typename Matrix::const_proxy_type*;
  using reference = const typename Matrix::const_proxy_type&;
  using difference_type = typename Matrix::storage_type::difference_type;
  using iterator_category = std::forward_iterator_tag;

  explicit MatrixConstIterator(const Matrix& matrix, const inner_iterator& position, const inner_iterator& end)
    : matrix{matrix}, position{position}, end{end} { GetValue(); }

  reference operator*() const
  {
    return *value.get();
  }

  pointer operator->() const
  {
    return value.get();
  }

  Self& operator++()
  {
    ++position;
    GetValue();
    return *this;
  }

  Self operator++(int)
  {
    Self tmp{matrix, position, end};
    ++position;
    GetValue();
    return tmp;
  }

  bool operator==(const Self& other) const
  {
    return position == other.position;
  }

  bool operator!=(const Self& other) const
  {
    return position != other.position;
  }

private:

  void GetValue() {
    if(position == end)
      value.reset(nullptr);
    else
      value.reset(new value_type{matrix, position->first});
  }

  const Matrix& matrix;
  inner_iterator position;
  inner_iterator end;
  std::unique_ptr<value_type> value;
};
