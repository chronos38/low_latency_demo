//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_UTILITY_HPP
#define BOOST_DI_AUX_UTILITY_HPP

struct _ {
  _(...) {}
};

namespace aux {

template <class T>
using owner = T;

template <class...>
struct valid {
  using type = int;
};

template <class... Ts>
using valid_t = typename valid<Ts...>::type;

template <class...>
struct type {};

struct none_type {};

template <class T, T>
struct non_type {};

template <class...>
struct always {
  static constexpr auto value = true;
};

template <class...>
struct never {
  static constexpr auto value = false;
};

template <class T, class...>
struct identity {
  using type = T;
};

template <class...>
struct type_list {
  using type = type_list;
};

template <bool...>
struct bool_list {
  using type = bool_list;
};

template <class T1, class T2>
struct pair {
  using type = pair;
  using first = T1;
  using second = T2;
};

template <class... Ts>
struct inherit : Ts... {
  using type = inherit;
};

template <class...>
struct join;

template <>
struct join<> {
  using type = type_list<>;
};

template <class... TArgs>
struct join<type_list<TArgs...>> {
  using type = type_list<TArgs...>;
};

template <class... TArgs1, class... TArgs2>
struct join<type_list<TArgs1...>, type_list<TArgs2...>> {
  using type = type_list<TArgs1..., TArgs2...>;
};

template <class... TArgs1, class... TArgs2, class... Ts>
struct join<type_list<TArgs1...>, type_list<TArgs2...>, Ts...> {
  using type = typename join<type_list<TArgs1..., TArgs2...>, Ts...>::type;
};

template <class... TArgs>
using join_t = typename join<TArgs...>::type;

template <int...>
struct index_sequence {
  using type = index_sequence;
};

#if __has_builtin(__make_integer_seq)  // __pph__
template <class T, T...>
struct integer_sequence;
template <int... Ns>
struct integer_sequence<int, Ns...> {
  using type = index_sequence<Ns...>;
};
template <int N>
struct make_index_sequence_impl {
  using type = typename __make_integer_seq<integer_sequence, int, N>::type;
};
#else   // __pph__
template <int>
struct make_index_sequence_impl;

template <>
struct make_index_sequence_impl<0> : index_sequence<> {};

template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};

template <>
struct make_index_sequence_impl<2> : index_sequence<0, 1> {};

template <>
struct make_index_sequence_impl<3> : index_sequence<0, 1, 2> {};

template <>
struct make_index_sequence_impl<4> : index_sequence<0, 1, 2, 3> {};

template <>
struct make_index_sequence_impl<5> : index_sequence<0, 1, 2, 3, 4> {};

template <>
struct make_index_sequence_impl<6> : index_sequence<0, 1, 2, 3, 4, 5> {};

template <>
struct make_index_sequence_impl<7> : index_sequence<0, 1, 2, 3, 4, 5, 6> {};

template <>
struct make_index_sequence_impl<8> : index_sequence<0, 1, 2, 3, 4, 5, 6, 7> {};

template <>
struct make_index_sequence_impl<9> : index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8> {};

template <>
struct make_index_sequence_impl<10> : index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9> {};
#endif  // __pph__

template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;

}  // aux

#endif
