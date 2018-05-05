#include <iostream>
#include "matrix.h"

int main(int argc, char const* argv[])
{
  try
  {
    Matrix<2, int, 0> matrix;

    for(int i{}, j{9}; i < 10; ++i, --j) {
      matrix[i][i] = i;
      matrix[i][j] = j;
    }

    for(int i{1}; i < 9; ++i) {
      for(int j{1}; j < 9; ++j) {
        if(1 != j)
          std::cout << ' ';
        std::cout << matrix[i][j];
      }
      std::cout << std::endl;
    }

    std::cout << matrix.size() << std::endl;

    for(const auto& matrix_element: matrix)
    {
      std::size_t x;
      std::size_t y;
      int v;
      std::tie(x, y, v) = matrix_element;
      std::cout << x << y << v << std::endl;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}