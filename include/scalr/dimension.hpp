/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <cstdint>
#include <type_traits>

namespace scalr {

// System: 7 Base SI dimensions + angular length
template <intmax_t T, intmax_t L, intmax_t M, intmax_t I, intmax_t K,
          intmax_t N, intmax_t J, intmax_t R>
struct system_signature {
  static constexpr intmax_t t = T;  // time dimension
  static constexpr intmax_t l = L;  // length dimension
  static constexpr intmax_t m = M;  // mass dimension
  static constexpr intmax_t i = I;  // electric current dimension
  static constexpr intmax_t k = K;  // temperature dimension
  static constexpr intmax_t n = N;  // amount of substance dimension
  static constexpr intmax_t j = J;  // luminous intensity dimension
  static constexpr intmax_t r = R;  // angular length dimension
};

namespace detail {

template <typename T>
struct is_system_signature : std::false_type {};

template <intmax_t T, intmax_t L, intmax_t M, intmax_t I, intmax_t K,
          intmax_t N, intmax_t J, intmax_t R>
struct is_system_signature<system_signature<T, L, M, I, K, N, J, R>>
    : std::true_type {};

template <typename D>
struct is_dimension {
  static constexpr bool value =
      is_system_signature<typename D::signature>::value;
};

}  // namespace detail

struct dimensionless {
  using signature = system_signature<0, 0, 0, 0, 0, 0, 0, 0>;
};

template <intmax_t T, intmax_t L, intmax_t M, intmax_t I, intmax_t K,
          intmax_t N, intmax_t J, intmax_t R>
struct unnamed_dimension {
  using signature = system_signature<T, L, M, I, K, N, J, R>;
};

namespace dimension {

template <class D1, class D2>
struct equal {
  static constexpr bool value =
      std::is_same<typename D1::signature, typename D2::signature>::value;
};

// Get dimension name from the signature if named, otherwise unnamed
template <intmax_t T, intmax_t L, intmax_t M, intmax_t I, intmax_t K,
          intmax_t N, intmax_t J, intmax_t R>
struct make {
  using type = unnamed_dimension<T, L, M, I, K, N, J, R>;
};

template <>
struct make<0, 0, 0, 0, 0, 0, 0, 0> {
  using type = dimensionless;
};

// Compile-time dimension arithmetic
template <class... Dimensions>
struct product {
  using type = dimensionless;
};

template <class D1, class D2>
struct product<D1, D2> {
  using U1 = typename D1::signature;
  using U2 = typename D2::signature;
  using type = typename make<U1::t + U2::t, U1::l + U2::l, U1::m + U2::m,
                             U1::i + U2::i, U1::k + U2::k, U1::n + U2::n,
                             U1::j + U2::j, U1::r + U2::r>::type;
};

template <class D1, class... Dn>
struct product<D1, Dn...> {
  using type = typename product<D1, typename product<Dn...>::type>::type;
};

template <class D>
struct inverse {
  using U = typename D::signature;
  using type = typename make<-U::t, -U::l, -U::m, -U::i, -U::k, -U::n, -U::j,
                             -U::r>::type;
};

template <class D, uintmax_t k>
struct uexponent {
  using type = typename product<typename uexponent<D, k - 1>::type, D>::type;
};

template <class D>
struct uexponent<D, 0> {
  using type = dimensionless;
};

template <class D, intmax_t k, bool>
struct exponent_impl {};

template <class D, intmax_t k>
struct exponent_impl<D, k, true> {
  using type = typename inverse<typename uexponent<D, -k>::type>::type;
};

template <class D, intmax_t k>
struct exponent_impl<D, k, false> {
  using type = typename uexponent<D, k>::type;
};

template <class D, intmax_t k>
struct exponent {
  using type = typename exponent_impl<D, k, (k < 0)>::type;
};

}  // namespace dimension

// Helper traits
template <int T, int L, int M, int I, int K, int N, int J, int R>
using make_dimension_t = typename dimension::make<T, L, M, I, K, N, J, R>::type;

template <class... D>
using dimension_product_t = typename dimension::product<D...>::type;

template <class D>
using dimension_inverse_t = typename dimension::inverse<D>::type;

template <class D, intmax_t k>
using dimension_exponent_t = typename dimension::exponent<D, k>::type;

template <class D1, class D2>
using dimension_equal = typename dimension::equal<D1, D2>;

}  // namespace scalr
