#include "homework_6.h"
#include "matrix.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_suite_tuple_metafunctions)

BOOST_AUTO_TEST_CASE(test_tuple_concat_front)
{
  BOOST_STATIC_ASSERT(true == std::is_same<std::tuple<std::string, int>,
                                          tuple_concat_front_t<std::string, std::tuple<int>>>::value);
  BOOST_STATIC_ASSERT(false == std::is_same<std::tuple<std::string, int>,
                                          tuple_concat_front_t<int, std::tuple<std::string>>>::value);
}

BOOST_AUTO_TEST_CASE(test_tuple_concat_back)
{
  BOOST_STATIC_ASSERT(false == std::is_same<std::tuple<std::string, int>,
                                          tuple_concat_back_t<std::string, std::tuple<int>>>::value);
  BOOST_STATIC_ASSERT(true == std::is_same<std::tuple<std::string, int>,
                                          tuple_concat_back_t<int, std::tuple<std::string>>>::value);
}

BOOST_AUTO_TEST_CASE(test_generate_tuple)
{
  BOOST_STATIC_ASSERT(true == std::is_same<std::tuple<int>,
                                          generate_tuple_t<int, 1>>::value);
  BOOST_STATIC_ASSERT(true == std::is_same<std::tuple<int, int, int>,
                                          generate_tuple_t<int, 3>>::value);
}

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE(test_suite_matrix)

BOOST_AUTO_TEST_CASE(test_suite_matrix_size)
{
  Matrix<2, int, 0> matrix;

  BOOST_CHECK_EQUAL(0, matrix.size());
  matrix[0][0] = 1;
  BOOST_CHECK_EQUAL(1, matrix.size());
  matrix[0][0] = 0;
  BOOST_CHECK_EQUAL(0, matrix.size());
}

BOOST_AUTO_TEST_CASE(test_suite_matrix_canonical_assignment)
{
  Matrix<2, int, 0> matrix;

  ((matrix[100][100] = 314) = 0) = 217;
  BOOST_CHECK_EQUAL(1, matrix.size());
  BOOST_CHECK_EQUAL(217, matrix[100][100]);
}

BOOST_AUTO_TEST_CASE(test_suite_matrix_iterator)
{
  Matrix<2, int, 0> matrix;
  std::size_t x;
  std::size_t y;
  int v;

  matrix[100][100] = 314;
  matrix[100][101] = 217;
  auto matrix_element = std::cbegin(matrix);

  std::tie(x, y, v) = static_cast<decltype(matrix)::ret_type>(*matrix_element);
  BOOST_CHECK_EQUAL(100, x);
  BOOST_CHECK_EQUAL(100, y);
  BOOST_CHECK_EQUAL(314, v);

  ++matrix_element;

  std::tie(x, y, v) = static_cast<decltype(matrix)::ret_type>(*matrix_element);
  BOOST_CHECK_EQUAL(100, x);
  BOOST_CHECK_EQUAL(101, y);
  BOOST_CHECK_EQUAL(217, v);
}

BOOST_AUTO_TEST_CASE(test_suite_matrix_dimensions)
{
  Matrix<4, int, 0> matrix;
  std::size_t x;
  std::size_t y;
  std::size_t z;
  std::size_t t;
  int v;

  matrix[111][222][333][444] = 555;

  auto matrix_element = std::cbegin(matrix);
  std::tie(x, y, z, t, v) = static_cast<decltype(matrix)::ret_type>(*matrix_element);
  BOOST_CHECK_EQUAL(111, x);
  BOOST_CHECK_EQUAL(222, y);
  BOOST_CHECK_EQUAL(333, z);
  BOOST_CHECK_EQUAL(444, t);
  BOOST_CHECK_EQUAL(555, v);
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(test_suite_homework)

BOOST_AUTO_TEST_CASE(test_homework_6_valid)
{
  std::string result{
    "1 0 0 0 0 0 0 8\n"
    "0 2 0 0 0 0 7 0\n"
    "0 0 3 0 0 6 0 0\n"
    "0 0 0 4 5 0 0 0\n"
    "0 0 0 4 5 0 0 0\n"
    "0 0 3 0 0 6 0 0\n"
    "0 2 0 0 0 0 7 0\n"
    "1 0 0 0 0 0 0 8\n"
    "18\n"
    "099\n"
    "111\n"
    "188\n"
    "222\n"
    "277\n"
    "333\n"
    "366\n"
    "444\n"
    "455\n"
    "544\n"
    "555\n"
    "633\n"
    "666\n"
    "722\n"
    "777\n"
    "811\n"
    "888\n"
    "999\n"
  };
  std::ostringstream oss;

  homework_6(oss);
  BOOST_CHECK_EQUAL(oss.str(), result);
}

BOOST_AUTO_TEST_SUITE_END()
