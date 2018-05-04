#include "homework_6.h"
#include "matrix.h"

void homework_6(std::ostream& out)
{
  
  Matrix<2, int, 0> matrix;

  for(int i{}, j{9}; i < 10; ++i, --j) {
    matrix[i][i] = i;
    matrix[i][j] = j;
  }

  for(int i{1}; i < 9; ++i) {
    for(int j{1}; j < 9; ++j) {
      if(1 != j)
        out << ' ';
      out << matrix[i][j];
    }
    out << std::endl;
  }

  out << matrix.size() << std::endl;

  for(const auto& matrix_element: matrix)
  {
    std::size_t x;
    std::size_t y;
    int v;
    std::tie(x, y, v) = matrix_element;
    out << x << y << v << std::endl;
  }
  
}
