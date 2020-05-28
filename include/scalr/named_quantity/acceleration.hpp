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

struct acceleration_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<-2, 1, 0, 0, 0, 0, 0, 0>;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using acceleration = quantity<Rep, make_unit_t<acceleration_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<-2, 1, 0, 0, 0, 0, 0, 0> {
  using type = acceleration_dimension;
};

}  // namespace dimension

namespace unit {

struct meters_per_second_squared {
  using dimension = acceleration_dimension;
  using ratio = std::ratio<1>;
};

template <>
struct make<acceleration_dimension, std::ratio<1>> {
  using type = meters_per_second_squared;
};
}  // namespace unit

using meters_per_second_squared = acceleration<double, std::ratio<1>>;

namespace literals {

constexpr meters_per_second_squared operator""_mps2(long double value) {
  return meters_per_second_squared{value};
}

constexpr meters_per_second_squared operator""_mps2(unsigned long long value) {
  return meters_per_second_squared{value};
}

}  // namespace literals

}  // namespace scalr