//==================================================================================================
/*!
  @file

  @copyright 2016 NumScale SAS

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/
//==================================================================================================
#ifndef BOOST_SIMD_ARCH_COMMON_SCALAR_FUNCTION_DIV_HPP_INCLUDED
#define BOOST_SIMD_ARCH_COMMON_SCALAR_FUNCTION_DIV_HPP_INCLUDED

#include <boost/simd/function/divides.hpp>
#include <boost/simd/detail/dispatch/function/overload.hpp>
#include <boost/simd/detail/dispatch/hierarchy.hpp>
#include <boost/simd/detail/dispatch/function/overload.hpp>
#include <boost/simd/detail/dispatch/hierarchy.hpp>
#include <boost/config.hpp>

namespace boost { namespace simd { namespace ext
{

  namespace bd = boost::dispatch;
  namespace bs = boost::simd;
  BOOST_DISPATCH_OVERLOAD ( div_
                          , (typename T)
                          , bd::cpu_
                          , bd::scalar_<bd::arithmetic_<T>>
                          , bd::scalar_<bd::arithmetic_<T>>
                          )
  {
    BOOST_MAYBEINLINE T operator()(T  a, T  b ) const BOOST_NOEXCEPT
    {
      return divides(a, b);
    }
  };

#ifdef BOOST_MSVC
  #pragma warning(push)
  #pragma warning(disable: 4723) // potential divide by 0
#endif

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

} } }

#endif
