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

#pragma once

namespace scalr {

struct mass_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 0, 1, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct mass_unit {
  using dimension = mass_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using mass = quantity<Rep, make_unit_t<mass_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, 0, 1, 0, 0, 0, 0, 0> {
  using type = mass_dimension;
};

}  // namespace dimension

namespace unit {

struct kilograms {
  using dimension = mass_dimension;
  using ratio = std::ratio<1>;
};

struct grams {
  using dimension = mass_dimension;
  using ratio = std::ratio<1, 1000>;
};

struct milligrams {
  using dimension = mass_dimension;
  using ratio = std::ratio<1, 1000000>;
};

template <typename Ratio>
struct make<mass_dimension, Ratio> {
  using type = scalr::mass_unit<Ratio>;
};

template <>
struct make<mass_dimension, std::ratio<1>> {
  using type = kilograms;
};

template <>
struct make<mass_dimension, std::ratio<1, 1000>> {
  using type = grams;
};

template <>
struct make<mass_dimension, std::ratio<1, 1000000>> {
  using type = milligrams;
};

}  // namespace unit

using kilograms = mass<double, std::ratio<1>>;
using grams = mass<double, std::ratio<1, 1000>>;
using milligrams = mass<double, std::ratio<1, 1000000>>;

namespace literals {

constexpr kilograms operator""_kg(long double value) {
  return kilograms{value};
}
constexpr grams operator""_g(long double value) { return grams{value}; }

constexpr milligrams operator""_mg(long double value) {
  return milligrams{value};
}

constexpr kilograms operator""_kg(unsigned long long value) {
  return kilograms{value};
}
constexpr grams operator""_g(unsigned long long value) { return grams{value}; }

constexpr milligrams operator""_mg(unsigned long long value) {
  return milligrams{value};
}

}  // namespace literals

}  // namespace scalr