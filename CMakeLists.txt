cmake_minimum_required(VERSION 3.2)

# Определение PATCH_VERSION для локальной сборки и сборки на travis
set(PATCH_VERSION 1)
if($ENV{TRAVIS_BUILD_NUMBER})
  set(PATCH_VERSION $ENV{TRAVIS_BUILD_NUMBER})
endif()

project(matrix VERSION 1.0.${PATCH_VERSION})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Создание целей
add_executable(matrix main.cpp)

add_library(matrix_lib STATIC homework_6.cpp)

add_executable(matrix_test test.cpp)

# Настройка целей

# для всех целей
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wpedantic -Wall -Wextra)

target_link_libraries(matrix matrix_lib)

#target_compile_definitions(matrix_test PRIVATE BOOST_TEST_DYN_LINK)
target_link_libraries(matrix_test 
  Boost::unit_test_framework
  matrix_lib
)

install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT camelot58@gmail.com)

include (CPack)

enable_testing()

add_test(test_suite_tuple_metafunctions matrix_test)
add_test(test_suite_matrix matrix_test)
add_test(test_suite_homework matrix_test)
