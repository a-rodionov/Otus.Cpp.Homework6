#pragma once

#include <tuple>

template <typename T>
struct is_tuple : std::false_type{};

template <typename ... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type{};

template <typename T>
constexpr bool is_tuple_v = is_tuple<std::remove_cv_t<T>>::value;



template<typename T, size_t N>
struct generate_tuple
{
  static_assert(0 != N, "Second argument can't be equal to zero.");
  typedef decltype(std::tuple_cat(typename generate_tuple<T, N-1>::type{}, std::make_tuple(T{}))) type;
};

template<typename T>
struct generate_tuple<T, 1>
{
  typedef std::tuple<T> type;
};

template<typename T, size_t N>
using generate_tuple_t = typename generate_tuple<T, N>::type;




template<typename ... Args, std::size_t ... Indices>
bool tuple_n_less_func(const std::tuple<Args...>& lhs,
                       const std::tuple<Args...>& rhs,
                       std::index_sequence<Indices...> ) {
  return std::tie(std::get<Indices>(lhs)...) < std::tie(std::get<Indices>(rhs)...);
}

template<size_t N, typename T>
struct tuple_n_less {

  static_assert(is_tuple_v<T>, "Only std::tuple<Args...> is allowed as type parameter.");
  static_assert(0 < std::tuple_size<T>::value, "std::tuple<Args...> mustn't be empty.");
  static_assert(N <= std::tuple_size<T>::value, "Integer template parameter N must be less or equal to size of T=std::tuple<Args...>.");
  

  bool operator()(const T& lhs, const T& rhs) const {
    return tuple_n_less_func(lhs, rhs, std::make_index_sequence<N>{});
  }
};



template<typename T, std::size_t N, std::size_t ... Indices>
auto make_tuple_from_array_impl(const std::array<T, N>& values, std::index_sequence<Indices...>) {
  return std::tuple_cat(std::make_tuple(values[Indices]...));
}

template<typename T, std::size_t N>
auto make_tuple_from_array(const std::array<T, N>& values) {
  return make_tuple_from_array_impl(values, std::make_index_sequence<N>{});
}
