/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_ELECTRIC_CURRENT_HPP
#define SCALR_NAMED_QUANTITY_ELECTRIC_CURRENT_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct electric_current_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 0, 0, 1, 0, 0, 0, 0>;
};

template <typename Ratio>
struct electric_current_unit {
  using dimension = electric_current_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using electric_current =
    quantity<Rep, make_unit_t<electric_current_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, 0, 0, 1, 0, 0, 0, 0> {
  using type = electric_current_dimension;
};

}  // namespace dimension

namespace unit {

struct microampers {
  using dimension = electric_current_dimension;
  using ratio = std::ratio<1, 1000000>;
};

struct milliampers {
  using dimension = electric_current_dimension;
  using ratio = std::ratio<1, 1000>;
};

struct ampers {
  using dimension = electric_current_dimension;
  using ratio = std::ratio<1>;
};

template <typename Ratio>
struct make<electric_current_dimension, Ratio> {
  using type = scalr::electric_current_unit<Ratio>;
};

template <>
struct make<electric_current_dimension, std::ratio<1, 1000000>> {
  using type = microampers;
};

template <>
struct make<electric_current_dimension, std::ratio<1, 1000>> {
  using type = milliampers;
};

template <>
struct make<electric_current_dimension, std::ratio<1>> {
  using type = ampers;
};

}  // namespace unit

using microampers = electric_current<double, std::ratio<1, 1000000>>;
using milliampers = electric_current<double, std::ratio<1, 1000>>;
using ampers = electric_current<double, std::ratio<1>>;

namespace literals {

constexpr microampers operator""_uA(long double value) {
  return microampers{value};
}

constexpr milliampers operator""_mA(long double value) {
  return milliampers{value};
}

constexpr ampers operator""_A(long double value) { return ampers{value}; }

constexpr microampers operator""_uA(unsigned long long value) {
  return microampers{value};
}

constexpr milliampers operator""_mA(unsigned long long value) {
  return milliampers{value};
}

constexpr ampers operator""_A(unsigned long long value) {
  return ampers{value};
}

}  // namespace literals
}  // namespace scalr

#endif
