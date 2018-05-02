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
