//==================================================================================================
/*!
  @file

  @copyright 2016 NumScale SAS

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

**/
//==================================================================================================
#ifndef BOOST_SIMD_REDUCTION_HPP_INCLUDED
#define BOOST_SIMD_REDUCTION_HPP_INCLUDED

namespace boost {
namespace simd {
/*!
  @ingroup group-functions
  @defgroup group-reduction Reduction Functions

  These functions provide algorithms for in-register reduction and prefix-scan operations.

  They are usually only defined for **SIMD** types but may, in some
  cases such as @ref sum or  @ref prod, have a valid scalar semantic.

    <center>
|                      |                         |                    |                            |
|:--------------------:|:-----------------------:|:------------------:|:--------------------------:|
| @ref all             | @ref any                | @ref compare_equal | @ref compare_greater_equal |
| @ref compare_greater | @ref compare_less_equal | @ref compare_less  | @ref compare_not_equal     |
| @ref cummax          | @ref cummin             | @ref cumprod       | @ref cumsum                |
| @ref dot             | @ref hmsb               | @ref isincluded    | @ref isincluded_c          |
| @ref maximum         | @ref minimum            | @ref nbtrue        | @ref none                  |
| @ref prod            | @ref sum                |                    |                            |
     </center>
**/
}
}

#endif
