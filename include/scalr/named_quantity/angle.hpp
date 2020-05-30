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
#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct angle_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, -1, 0, 0, 0, 0, 0, 1>;
};

template <typename Ratio>
struct angle_unit {
  using dimension = angle_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using angle = quantity<Rep, make_unit_t<angle_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, -1, 0, 0, 0, 0, 0, 1> {
  using type = angle_dimension;
};

}  // namespace dimension

namespace unit {

struct reduced_radians {  // revolution, rotation, radians/2*PI
  using dimension = angle_dimension;
  using ratio = std::ratio<1>;
};

struct radians {
  using dimension = angle_dimension;
  using ratio = std::ratio<99532, 625378>;  // 1/(2*PI) Approx.
};

struct gradians {
  using dimension = angle_dimension;
  using ratio = std::ratio<1, 400>;
};

struct degrees {
  using dimension = angle_dimension;
  using ratio = std::ratio<1, 360>;
};

template <typename Ratio>
struct make<angle_dimension, Ratio> {
  using type = scalr::angle_unit<Ratio>;
};

template <>
struct make<angle_dimension, std::ratio<1>> {
  using type = reduced_radians;
};

template <>
struct make<angle_dimension, std::ratio<99532, 625378>> {
  using type = radians;
};

template <>
struct make<angle_dimension, std::ratio<1, 400>> {
  using type = gradians;
};

template <>
struct make<angle_dimension, std::ratio<1, 360>> {
  using type = degrees;
};

}  // namespace unit

// namespace quantity_type {

// // This operation static division
// template <typename Q1>
// struct angularize {
//   using V1 = typename Q1::value_type;
//   using U1 = typename Q1::unit_type;
//   using angular_type =
//       unit_product_t<U1, angular_unit<99532, 625378>>;  // 1/(2*PI) Approx.
//   using type = quantity<V1, angular_type>;
// };
// }  // namespace quantity_type

using reduced_radians = angle<double>;
using radians = angle<double, std::ratio<99532, 625378>>;
using gradians = angle<double, std::ratio<1, 400>>;
using degrees = angle<double, std::ratio<1, 360>>;

namespace literals {

constexpr radians operator""_rad(long double value) { return radians{value}; }
constexpr radians operator""_deg(long double value) { return degrees{value}; }
constexpr radians operator""_grad(long double value) { return gradians{value}; }

constexpr radians operator""_rad(unsigned long long value) {
  return radians{value};
}
constexpr radians operator""_deg(unsigned long long value) {
  return degrees{value};
}
constexpr radians operator""_grad(unsigned long long value) {
  return gradians{value};
}

}  // namespace literals

}  // namespace scalr