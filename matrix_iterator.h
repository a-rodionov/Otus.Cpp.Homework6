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
    : _matrix{matrix}, _position{position}, _end{end} { GetValue(); }

  reference operator*() const
  {
    return *_value.get();
  }

  pointer operator->() const
  {
    return _value.get();
  }

  Self& operator++()
  {
    ++_position;
    GetValue();
    return *this;
  }

  Self operator++(int)
  {
    Self tmp{_matrix, _position, _end};
    ++_position;
    GetValue();
    return tmp;
  }

  bool operator==(const Self& other) const
  {
    return _position == other._position;
  }

  bool operator!=(const Self& other) const
  {
    return _position != other._position;
  }

private:

  void GetValue() {
    if(_position == _end)
      _value.reset(nullptr);
    else
      _value.reset(new value_type{_matrix, _position->first});
  }

  Matrix& _matrix;
  inner_iterator _position;
  inner_iterator _end;
  std::unique_ptr<value_type> _value;
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
    : _matrix{matrix}, _position{position}, _end{end} { GetValue(); }

  reference operator*() const
  {
    return *_value.get();
  }

  pointer operator->() const
  {
    return _value.get();
  }

  Self& operator++()
  {
    ++_position;
    GetValue();
    return *this;
  }

  Self operator++(int)
  {
    Self tmp{_matrix, _position, _end};
    ++_position;
    GetValue();
    return tmp;
  }

  bool operator==(const Self& other) const
  {
    return _position == other._position;
  }

  bool operator!=(const Self& other) const
  {
    return _position != other._position;
  }

private:

  void GetValue() {
    if(_position == _end)
      _value.reset(nullptr);
    else
      _value.reset(new value_type{_matrix, _position->first});
  }

  const Matrix& _matrix;
  inner_iterator _position;
  inner_iterator _end;
  std::unique_ptr<value_type> _value;
};
