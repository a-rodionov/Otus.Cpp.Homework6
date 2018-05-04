#pragma once

template<typename T, typename U>
struct tuple_concat_front{};

template<typename T, typename ... Args>
struct tuple_concat_front<T, std::tuple<Args...>>
{
    typedef std::tuple<T, Args...> type;
};

template<typename T, typename U>
using tuple_concat_front_t = typename tuple_concat_front<T, U>::type;



template<typename T, typename U>
struct tuple_concat_back{};

template<typename T, typename ... Args>
struct tuple_concat_back<T, std::tuple<Args...>>
{
    typedef std::tuple<Args..., T> type;
};

template<typename T, typename U>
using tuple_concat_back_t = typename tuple_concat_back<T, U>::type;



template<typename T, size_t N>
struct generate_tuple
{
  static_assert(0 != N, "Second argument can't be equal to zero.");
  typedef tuple_concat_front_t<T, typename generate_tuple<T, N-1>::type> type;
};

template<typename T>
struct generate_tuple<T, 1>
{
  typedef std::tuple<T> type;
};

template<typename T, size_t N>
using generate_tuple_t = typename generate_tuple<T, N>::type;

template<typename ... Args_1, typename ... Args_2, std::size_t ... Indices>
bool tuple_is_equal(const std::tuple<Args_1...>& lhs, const std::tuple<Args_2...>& rhs, std::index_sequence<Indices...> ) {
  return std::tie(std::get<Indices>(lhs)...) == std::tie(std::get<Indices>(rhs)...);
}

template<typename ... Args_1, typename ... Args_2> 
bool tuple_is_equal(const std::tuple<Args_1...>& lhs, const std::tuple<Args_2...>& rhs) {
  const auto min_size = std::min(sizeof...(Args_1), sizeof...(Args_2));
  return tuple_is_equal(lhs, rhs, std::make_index_sequence<min_size>{});
}

template <typename T>
struct is_tuple : std::false_type{};

template <typename ... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type{};

template <typename T>
constexpr bool is_tuple_v = is_tuple<std::remove_cv_t<T>>::value;

template<typename ... Args, std::size_t ... Indices>
bool tuple_n_less_func(const std::tuple<Args...>& lhs,
                       const std::tuple<Args...>& rhs,
                       std::index_sequence<Indices...> ) {
  return std::tie(std::get<Indices>(lhs)...) < std::tie(std::get<Indices>(rhs)...);
}

template<size_t N, typename T>
struct tuple_n_less {

  static_assert(is_tuple_v<T>, "Only std::tuple<Args...> is allowed as type parameter.");
  static_assert(N <= std::tuple_size<T>::value, "Integer template parameter N must be less or equal to size of T=std::tuple<Args...>.");
  

  bool operator()(const T& lhs, const T& rhs) const {
    return tuple_n_less_func(lhs, rhs, std::make_index_sequence<N>{});
  }
};
