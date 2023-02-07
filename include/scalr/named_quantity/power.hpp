/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 * Copyright (c) 2023 Dmitriy Purgin
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_POWER_HPP
#define SCALR_NAMED_QUANTITY_POWER_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {
struct power_dimension {
  // 7 Base SI dimensions + angle dimension
  // SI base units	s−3⋅m2⋅kg
  using signature = system_signature<-3, 2, 1, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct power_unit {
  using dimension = power_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using power = quantity<Rep, make_unit_t<power_dimension, Ratio>>;

// Detail and specializations
namespace dimension {
template <>
struct make<-3, 2, 1, 0, 0, 0, 0, 0> {
  using type = power_dimension;
};
}  // namespace dimension

namespace unit {

struct microwatts {
  using dimension = power_dimension;
  using ratio = std::micro;
};

struct milliwatts {
  using dimension = power_dimension;
  using ratio = std::milli;
};

struct watts {
  using dimension = power_dimension;
  using ratio = std::ratio<1>;
};

struct kilowatts {
  using dimension = power_dimension;
  using ratio = std::kilo;
};

struct megawatts {
  using dimension = power_dimension;
  using ratio = std::mega;
};

struct gigawatts {
  using dimension = power_dimension;
  using ratio = std::giga;
};
}  // namespace unit

using microwatts = power<double, std::micro>;
using milliwatts = power<double, std::milli>;
using watts = power<double, std::ratio<1>>;
using kilowatts = power<double, std::kilo>;
using megawatts = power<double, std::mega>;
using gigawatts = power<double, std::giga>;

namespace literals {
constexpr microwatts operator""_uW(long double value) {
  return microwatts{value};
}
constexpr milliwatts operator""_mW(long double value) {
  return milliwatts{value};
}
constexpr watts operator""_W(long double value) { return watts{value}; }

constexpr kilowatts operator""_kW(long double value) {
  return kilowatts{value};
}

constexpr megawatts operator""_MW(long double value) {
  return megawatts{value};
}

constexpr gigawatts operator""_GW(long double value) {
  return gigawatts{value};
}

constexpr microwatts operator""_uW(unsigned long long value) {
  return microwatts{value};
}
constexpr milliwatts operator""_mW(unsigned long long value) {
  return milliwatts{value};
}
constexpr watts operator""_W(unsigned long long value) { return watts{value}; }

constexpr kilowatts operator""_kW(unsigned long long value) {
  return kilowatts{value};
}

constexpr megawatts operator""_MW(unsigned long long value) {
  return megawatts{value};
}

constexpr gigawatts operator""_GW(unsigned long long value) {
  return gigawatts{value};
}
}  // namespace literals
}  // namespace scalr

#endif
