#include "matrix.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_suite_tuple_metafunctions)

BOOST_AUTO_TEST_CASE(test_is_tuple_v)
{
  BOOST_STATIC_ASSERT(false == is_tuple_v<int>);
  BOOST_STATIC_ASSERT(true == is_tuple_v<std::tuple<>>);
  BOOST_STATIC_ASSERT(true == is_tuple_v<std::tuple<int, int>>);
  BOOST_STATIC_ASSERT(true == is_tuple_v<const std::tuple<int, float>>);
  BOOST_STATIC_ASSERT(true == is_tuple_v<volatile std::tuple<int, float>>);
  BOOST_STATIC_ASSERT(true == is_tuple_v<const volatile std::tuple<int, float>>);
}

BOOST_AUTO_TEST_CASE(test_generate_tuple)
{
  BOOST_STATIC_ASSERT(true == std::is_same<std::tuple<int>,
                                          generate_tuple_t<int, 1>>::value);
  BOOST_STATIC_ASSERT(true == std::is_same<std::tuple<int, int, int>,
                                          generate_tuple_t<int, 3>>::value);
}

BOOST_AUTO_TEST_CASE(test_tuple_n_less)
{
  auto first = std::make_tuple(1, 2);
  auto second = std::make_tuple(1, 3);
  auto third = std::make_tuple(2, 1);

  auto cmp_one_element = tuple_n_less<1, decltype(first)>{};
  auto cmp_two_elements = tuple_n_less<2, decltype(first)>{};

  BOOST_CHECK(false == cmp_one_element(first, second));
  BOOST_CHECK(false == cmp_one_element(second, first));

  BOOST_CHECK(true == cmp_one_element(first, third));
  BOOST_CHECK(false == cmp_one_element(third, first));

  BOOST_CHECK(true == cmp_two_elements(first, second));
  BOOST_CHECK(false == cmp_two_elements(second, first));

  BOOST_CHECK(true == cmp_two_elements(first, third));
  BOOST_CHECK(false == cmp_two_elements(third, first));
}

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE(test_suite_matrix)

BOOST_AUTO_TEST_CASE(test_matrix_size)
{
  Matrix<2, int, 0> matrix;

  BOOST_CHECK_EQUAL(0, matrix.size());
  matrix[0][0] = 1;
  BOOST_CHECK_EQUAL(1, matrix.size());
  matrix[0][0] = 0;
  BOOST_CHECK_EQUAL(0, matrix.size());
}

BOOST_AUTO_TEST_CASE(test_matrix_canonical_assignment)
{
  Matrix<2, int, 0> matrix;

  ((matrix[100][100] = 314) = 0) = 217;
  BOOST_CHECK_EQUAL(1, matrix.size());
  BOOST_CHECK_EQUAL(217, matrix[100][100]);
}

BOOST_AUTO_TEST_CASE(test_matrix_proxy_index_reassignment)
{
  Matrix<2, int, 0> matrix;

  matrix[100][100] = 314;
  matrix[100][101] = 217;

  auto element = matrix[100][100];
  BOOST_CHECK_EQUAL(314, element);

  BOOST_CHECK_EQUAL(217, element[101]);

  BOOST_CHECK_EQUAL(0, element[102]);
}

BOOST_AUTO_TEST_CASE(test_matrix_iterator)
{
  Matrix<2, int, 0> matrix;
  std::size_t x;
  std::size_t y;
  int v;

  matrix[100][100] = 314;
  matrix[100][101] = 217;
  auto matrix_element = std::cbegin(matrix);

  std::tie(x, y, v) = *matrix_element;
  BOOST_CHECK_EQUAL(100, x);
  BOOST_CHECK_EQUAL(100, y);
  BOOST_CHECK_EQUAL(314, v);

  ++matrix_element;

  std::tie(x, y, v) = *matrix_element;
  BOOST_CHECK_EQUAL(100, x);
  BOOST_CHECK_EQUAL(101, y);
  BOOST_CHECK_EQUAL(217, v);

  ++matrix_element;

  BOOST_CHECK(std::cend(matrix) == matrix_element);
}

BOOST_AUTO_TEST_CASE(test_matrix_n_dimensions)
{
  Matrix<4, int, 0> matrix;
  std::size_t x;
  std::size_t y;
  std::size_t z;
  std::size_t t;
  int v;

  matrix[111][222][333][444] = 555;

  auto matrix_element = std::cbegin(matrix);
  std::tie(x, y, z, t, v) = *matrix_element;
  BOOST_CHECK_EQUAL(111, x);
  BOOST_CHECK_EQUAL(222, y);
  BOOST_CHECK_EQUAL(333, z);
  BOOST_CHECK_EQUAL(444, t);
  BOOST_CHECK_EQUAL(555, v);
}

BOOST_AUTO_TEST_CASE(test_matrix_wrong_dimension_assignment)
{
  Matrix<2, int, 0> matrix;
  //matrix[100] = 111;        //compile error
}

BOOST_AUTO_TEST_CASE(test_matrix_wrong_dimension_casting)
{
  Matrix<2, int, 0> matrix;
  //int value = matrix[100];  //compile error
}

auto ReturnPseudoValue_ProxyInFact()
{
  Matrix<2, int, 0> matrix;
  matrix[100][100] = 314;
  return matrix[100][100];
}

BOOST_AUTO_TEST_CASE(test_get_value_by_proxy_of_destructed_matrix)
{
  int value;
  BOOST_CHECK_THROW(value = ReturnPseudoValue_ProxyInFact(), std::bad_weak_ptr);
}

BOOST_AUTO_TEST_CASE(test_matrix_content)
{
  Matrix<2, int, 0> matrix;
  std::ostringstream oss;
  std::string result{
    "0 0 0 0 0 0 0 0 0 9\n"
    "0 1 0 0 0 0 0 0 8 0\n"
    "0 0 2 0 0 0 0 7 0 0\n"
    "0 0 0 3 0 0 6 0 0 0\n"
    "0 0 0 0 4 5 0 0 0 0\n"
    "0 0 0 0 4 5 0 0 0 0\n"
    "0 0 0 3 0 0 6 0 0 0\n"
    "0 0 2 0 0 0 0 7 0 0\n"
    "0 1 0 0 0 0 0 0 8 0\n"
    "0 0 0 0 0 0 0 0 0 9\n"
  };
  
  for(int i{}, j{9}; i < 10; ++i, --j) {
    matrix[i][i] = i;
    matrix[i][j] = j;
  }

  for(int i{}; i < 10; ++i) {
    for(int j{}; j < 10; ++j) {
      if(0 != j)
        oss << ' ';
      oss << matrix[i][j];
    }
    oss << std::endl;
  }

  BOOST_CHECK_EQUAL(oss.str(), result);
}

BOOST_AUTO_TEST_SUITE_END()
