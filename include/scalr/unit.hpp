/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_UNIT_HPP
#define SCALR_UNIT_HPP

#include <ratio>
#include <type_traits>

#include "scalr/dimension.hpp"

namespace scalr {

/* Class template physical unit type */
template <class DimensionT, class Ratio = std::ratio<1>>
struct unnamed_unit {
  using dimension = DimensionT;
  using ratio = Ratio;
};

namespace detail {

template <intmax_t K>
struct sign : std::integral_constant<intmax_t, (K < 0) ? -1 : 1> {};

template <intmax_t K>
struct abs : std::integral_constant<intmax_t, K * sign<K>::value> {};

template <intmax_t M, intmax_t N>
struct gcd : gcd<N, (M % N)> {};

template <intmax_t M>
struct gcd<M, 0> : std::integral_constant<intmax_t, abs<M>::value> {};

template <intmax_t N>
struct gcd<0, N> : std::integral_constant<intmax_t, abs<N>::value> {};

template <intmax_t M, intmax_t N>
struct lcm : std::integral_constant<intmax_t, (M / gcd<M, N>::value) * N> {};

// gcrd: greatest common rational divisor
// gcrd(a/b, c/d) = gcd(ad, bc)/bd
// gcrd(a/b, c/d) = gcd(a,c)/lcm(b,d) if gcd(a,b) = gcd(c,d) = 1
template <class R1, class R2>
struct ratio_gcrd : std::ratio<scalr::detail::gcd<R1::num, R2::num>::value,
                               scalr::detail::lcm<R1::den, R2::den>::value> {};

// lcrm: least common rational multiple
// lcrm(a/b, c/d) = lcm(ad, bc)/bd
// lcrm(a/b, c/d) = lcm(a,c)/gcd(b,d) if gcd(a,b) = gcd(c,d) = 1
template <class R1, class R2>
struct ratio_lcrm : std::ratio<scalr::detail::lcm<R1::num, R2::num>::value,
                               scalr::detail::gcd<R1::den, R2::den>::value> {};

template <bool B>
using bool_constant = std::integral_constant<bool, B>;

template <typename T>
struct is_ratio : std::false_type {};

template <intmax_t Num, intmax_t Den>
struct is_ratio<std::ratio<Num, Den>> : std::true_type {};

template <class U>
struct is_unit_type {
  static constexpr bool value = std::integral_constant < bool,
                        is_dimension<typename U::dimension>::value
                                &&is_ratio<typename U::ratio>::value > ::value;
};

}  // namespace detail

namespace unit {

template <class U1, class U2>
struct equal {
  using D1 = typename U1::dimension;
  using R1 = typename U1::ratio;
  using D2 = typename U2::dimension;
  using R2 = typename U2::ratio;
  static constexpr bool value = std::integral_constant < bool,
                        scalr::dimension_equal<D1, D2>::value
                                &&std::ratio_equal<R1, R2>::value > ::value;
};

// Compile-time unit_type arithmetic
template <class D, class Ratio>
struct make {
  using type = unnamed_unit<D, Ratio>;
};

template <class U1, class...>
struct sum {
  using D1 = typename U1::dimension;
  using R1 = typename U1::ratio;
  using type = typename make<D1, R1>::type;
};

template <class U1, class U2>
struct sum<U1, U2> {
  static_assert(scalr::detail::is_unit_type<U1>::value,
                "template arguments must be a scalr::unit type");
  static_assert(scalr::detail::is_unit_type<U2>::value,
                "template arguments must be a scalr::unit type");

  using D1 = typename U1::dimension;
  using D2 = typename U2::dimension;
  using R1 = typename U1::ratio;
  using R2 = typename U2::ratio;

  static_assert(std::is_same<D1, D2>::value,
                "unit dimensions must match for addition/subtraction");

  using type = typename make<
      D1, std::ratio<scalr::detail::gcd<R1::num, R2::num>::value,
                     scalr::detail::lcm<R1::den, R2::den>::value>>::type;
};

template <class U1, class U2, class... Un>
struct sum<U1, U2, Un...> {
  using D1 = typename U1::dimension;
  using D2 = typename U2::dimension;
  using type = typename sum<U1, typename sum<U2, Un...>::type>::type;
};

template <class...>
struct product {
  using type = typename make<dimensionless, std::ratio<1>>::type;
};

template <class U1, class U2>
struct product<U1, U2> {
  using type = typename make<
      dimension_product_t<typename U1::dimension, typename U2::dimension>,
      std::ratio_multiply<typename U1::ratio, typename U2::ratio>>::type;
};

template <class U1, class... Un>
struct product<U1, Un...> {
  using type = typename product<U1, typename product<Un...>::type>::type;
};

template <class U>
struct inverse {
  using type =
      typename make<dimension_inverse_t<typename U::dimension>,
                    std::ratio_divide<std::ratio<1>, typename U::ratio>>::type;
};

template <class U, uintmax_t k>
struct uexponent {
  using type = typename product<typename uexponent<U, k - 1>::type, U>::type;
};

template <class U>
struct uexponent<U, 0> {
  using type = typename make<dimensionless, std::ratio<1>>::type;
};

template <class U, intmax_t k, bool>
struct exponent_impl {};

template <class U, intmax_t k>
struct exponent_impl<U, k, true> {
  using type = typename inverse<typename uexponent<U, -k>::type>::type;
};

template <class U, intmax_t k>
struct exponent_impl<U, k, false> {
  using type = typename uexponent<U, k>::type;
};

template <class U, intmax_t k>
struct exponent {
  using type = typename exponent_impl<U, k, (k < 0)>::type;
};

}  // namespace unit

// Helper traits
template <class Dimension, class Ratio = std::ratio<1>>
using make_unit_t = typename unit::make<Dimension, Ratio>::type;

template <class... D>
using unit_sum_t = typename unit::sum<D...>::type;

template <class... D>
using unit_product_t = typename unit::product<D...>::type;

template <class U>
using unit_inverse_t = typename unit::inverse<U>::type;

template <class U, intmax_t k>
using unit_exponent_t = typename unit::exponent<U, k>::type;

template <class U1, class U2>
using unit_equal = typename unit::equal<U1, U2>;

}  // namespace scalr

#endif
