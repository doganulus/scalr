/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020 Dogan Ulus
 * Copyright (c) 2023 Dmitriy Purgin
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
struct electric_potential_dimension {
  // 7 Base SI dimensions + angle dimension
  // SI base units	kg⋅m2⋅s−3⋅A−1
  using signature = system_signature<1, 1, 1, 1, 0, 0, 0, 0>;
};

template <typename Ratio>
struct electric_potential_unit {
  using dimension = electric_potential_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using electric_potential =
    quantity<Rep, make_unit_t<electric_potential_dimension, Ratio>>;

// Detail and specializations
namespace dimension {
template <>
struct make<1, 1, 1, 1, 0, 0, 0, 0> {
  using type = electric_potential_dimension;
};
}  // namespace dimension

namespace unit {

struct microvolts {
  using dimension = electric_potential_dimension;
  using ratio = std::ratio<1, 1000000>;
};

struct millivolts {
  using dimension = electric_potential_dimension;
  using ratio = std::ratio<1, 1000>;
};

struct volts {
  using dimension = electric_potential_dimension;
  using ratio = std::ratio<1>;
};
}  // namespace unit

using microvolts = electric_potential<double, std::ratio<1, 1000000>>;
using millivolts = electric_potential<double, std::ratio<1, 1000>>;
using volts = electric_potential<double, std::ratio<1>>;

namespace literals {

constexpr microvolts operator""_uV(long double value) {
  return microvolts{value};
}
constexpr millivolts operator""_mV(long double value) {
  return millivolts{value};
}
constexpr volts operator""_V(long double value) { return volts{value}; }

constexpr microvolts operator""_uV(unsigned long long value) {
  return microvolts{value};
}
constexpr millivolts operator""_mV(unsigned long long value) {
  return millivolts{value};
}
constexpr volts operator""_V(unsigned long long value) { return volts{value}; }
}  // namespace literals
}  // namespace scalr
