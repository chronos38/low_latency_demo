// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP

#include "boost/di/fwd.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/singleton.hpp"

namespace type_traits {

template <class T>
struct scope_traits {
  using type = scopes::unique;
};

template <class T>
struct scope_traits<T&> {
  using type = scopes::singleton;
};

template <class T>
struct scope_traits<std::shared_ptr<T>> {
  using type = scopes::singleton;
};

template <class T>
struct scope_traits<boost::shared_ptr<T>> {
  using type = scopes::singleton;
};

template <class T>
struct scope_traits<std::weak_ptr<T>> {
  using type = scopes::singleton;
};

template <class T>
using scope_traits_t = typename scope_traits<T>::type;

}  // type_traits

#endif
