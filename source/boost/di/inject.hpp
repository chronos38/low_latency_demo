//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/preprocessor.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

template <class, class>
struct named;

namespace detail {

struct named_impl {
  template <class T>
  T operator=(const T&) const;
};
static constexpr BOOST_DI_UNUSED named_impl named{};

template <class T, class TName>
struct combine_impl {
  using type = BOOST_DI_NAMESPACE::named<TName, T>;
};

template <class T>
struct combine_impl<T, aux::none_type> {
  using type = T;
};

template <class, class>
struct combine;

template <class... T1, class... T2>
struct combine<aux::type_list<T1...>, aux::type_list<T2...>> {
  using type = aux::type_list<typename combine_impl<T1, T2>::type...>;
};

template <class T1, class T2>
using combine_t = typename combine<T1, T2>::type;

}  // detail

template <class... Ts>
using inject = aux::type_list<Ts...>;

#define BOOST_DI_HAS_NAME(i, ...) BOOST_DI_IF(BOOST_DI_IBP(BOOST_DI_ELEM(i, __VA_ARGS__, )), 1, )
#define BOOST_DI_HAS_NAMES(...) \
  BOOST_DI_IF(BOOST_DI_IS_EMPTY(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_HAS_NAME, __VA_ARGS__)), 0, 1)
#define BOOST_DI_GEN_CTOR_IMPL(p, i) \
  BOOST_DI_IF(i, BOOST_DI_COMMA, BOOST_DI_EAT)() BOOST_DI_IF(BOOST_DI_IBP(p), BOOST_DI_EAT p, p)
#define BOOST_DI_GEN_CTOR(i, ...) BOOST_DI_GEN_CTOR_IMPL(BOOST_DI_ELEM(i, __VA_ARGS__, ), i)
#define BOOST_DI_GEN_ARG_NAME(p) BOOST_DI_GEN_ARG_NAME_IMPL p )
#define BOOST_DI_GEN_NONE_TYPE(p) BOOST_DI_NAMESPACE::aux::none_type
#define BOOST_DI_GEN_ARG_NAME_IMPL(p) decltype(BOOST_DI_NAMESPACE::detail::p) BOOST_DI_EAT(
#define BOOST_DI_GEN_NAME_IMPL(p, i) \
  BOOST_DI_IF(i, BOOST_DI_COMMA, BOOST_DI_EAT)() BOOST_DI_IF(BOOST_DI_IBP(p), BOOST_DI_GEN_ARG_NAME, BOOST_DI_GEN_NONE_TYPE)(p)
#define BOOST_DI_GEN_NAME(i, ...) BOOST_DI_GEN_NAME_IMPL(BOOST_DI_ELEM(i, __VA_ARGS__, ), i)

#define BOOST_DI_INJECT_TRAITS_IMPL_0(...)                                                       \
  static void ctor(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__)); \
  using type BOOST_DI_UNUSED = BOOST_DI_NAMESPACE::aux::function_traits_t<decltype(ctor)>;

#define BOOST_DI_INJECT_TRAITS_IMPL_1(...)                                                              \
  static void ctor(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__));        \
  static void name(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_NAME, __VA_ARGS__));        \
  using type BOOST_DI_UNUSED =                                                                          \
      BOOST_DI_NAMESPACE::detail::combine_t<BOOST_DI_NAMESPACE::aux::function_traits_t<decltype(ctor)>, \
                                            BOOST_DI_NAMESPACE::aux::function_traits_t<decltype(name)>>;

#define BOOST_DI_INJECT_TRAITS_EMPTY_IMPL(...) using boost_di_inject__ BOOST_DI_UNUSED = BOOST_DI_NAMESPACE::aux::type_list<>

#define BOOST_DI_INJECT_TRAITS_IMPL(...)                                                                                                                                                                                                                                                                                   \
  struct boost_di_inject__ {                                                                                                                                                                                                                                                                                               \
    BOOST_DI_CAT(BOOST_DI_INJECT_TRAITS_IMPL_, BOOST_DI_HAS_NAMES(__VA_ARGS__))(__VA_ARGS__) static_assert( \
        BOOST_DI_SIZE(__VA_ARGS__) <= BOOST_DI_CFG_CTOR_LIMIT_SIZE,                                         \
        "Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE"); \
  }

#define BOOST_DI_INJECT_TRAITS(...) \
  BOOST_DI_IF(BOOST_DI_IS_EMPTY(__VA_ARGS__), BOOST_DI_INJECT_TRAITS_EMPTY_IMPL, BOOST_DI_INJECT_TRAITS_IMPL)(__VA_ARGS__)

#define BOOST_DI_INJECT(T, ...)        \
  BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
  T(BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__))

#endif
