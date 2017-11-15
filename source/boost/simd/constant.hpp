//==================================================================================================
/*!
  @file

  @copyright 2016 NumScale SAS

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

**/
//==================================================================================================
#ifndef BOOST_SIMD_CONSTANT_HPP_INCLUDED
#define BOOST_SIMD_CONSTANT_HPP_INCLUDED

/*!
  @ingroup group-functions
  @defgroup group-constant Constant Functions

  These functions provide scalar and SIMD constant generators for the most usual
  constant values such as zero, numeric limits, IEEE special values.

  Constant generator functions called by either:

  - taking no arguments but requiring a template type parameter
    so that the correct type of the constant is used:
    @code
    auto x = One<float>();
    @endcode

  - taking one type-arguments from which the correct type of the constant is deduced:
    @code
    double y = 0.;
    auto x = One( as(y) );
    @endcode
**/

#include <boost/simd/constant/allbits.hpp>
#include <boost/simd/constant/constant.hpp>
#include <boost/simd/constant/valmax.hpp>
#include <boost/simd/constant/valmin.hpp>
#include <boost/simd/constant/zero.hpp>

#endif
