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

struct volume_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 3, 0, 0, 0, 0, 0, 0>;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using volume = quantity<Rep, make_unit_t<volume_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, 3, 0, 0, 0, 0, 0, 0> {
  using type = volume_dimension;
};
}  // namespace dimension

namespace unit {

struct cubic_meter {
  using dimension = volume_dimension;
  using ratio = std::ratio<1>;
};

struct liter {
  using dimension = volume_dimension;
  using ratio = std::ratio<100>;
};

struct milliliter {
  using dimension = volume_dimension;
  using ratio = std::ratio<10000>;
};

template <>
struct make<volume_dimension, std::ratio<1>> {
  using type = cubic_meter;
};

template <>
struct make<volume_dimension, std::ratio<1, 1000>> {
  using type = liter;
};

template <>
struct make<volume_dimension, std::ratio<1, 1000000>> {
  using type = milliliter;
};

}  // namespace unit

using cubic_meter = volume<double, std::ratio<1>>;
using liter = volume<double, std::ratio<1, 1000>>;
using milliliter = volume<double, std::ratio<1, 1000000>>;

namespace literals {

constexpr cubic_meter operator""_m3(long double value) {
  return cubic_meter{value};
}
constexpr liter operator""_l(long double value) { return liter{value}; }

constexpr milliliter operator""_ml(long double value) {
  return milliliter{value};
}

constexpr cubic_meter operator""_m3(unsigned long long value) {
  return cubic_meter{value};
}
constexpr liter operator""_l(unsigned long long value) { return liter{value}; }

constexpr milliliter operator""_ml(unsigned long long value) {
  return milliliter{value};
}

}  // namespace literals

}  // namespace scalr