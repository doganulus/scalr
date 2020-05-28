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

struct length_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 1, 0, 0, 0, 0, 0, 0>;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using length = quantity<Rep, make_unit_t<length_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, 1, 0, 0, 0, 0, 0, 0> {
  using type = length_dimension;
};

}  // namespace dimension

namespace unit {

struct kilometers {
  using dimension = length_dimension;
  using ratio = std::kilo;
};

struct meters {
  using dimension = length_dimension;
  using ratio = std::ratio<1>;
};

struct decimeters {
  using dimension = length_dimension;
  using ratio = std::deci;
};

struct centimeters {
  using dimension = length_dimension;
  using ratio = std::centi;
};

struct millimeters {
  using dimension = length_dimension;
  using ratio = std::milli;
};

struct micrometers {
  using dimension = length_dimension;
  using ratio = std::micro;
};

struct nanometers {
  using dimension = length_dimension;
  using ratio = std::nano;
};

struct picometers {
  using dimension = length_dimension;
  using ratio = std::pico;
};

template <>
struct make<length_dimension, std::kilo> {
  using type = kilometers;
};

template <>
struct make<length_dimension, std::ratio<1>> {
  using type = meters;
};

template <>
struct make<length_dimension, std::deci> {
  using type = decimeters;
};

template <>
struct make<length_dimension, std::centi> {
  using type = centimeters;
};

template <>
struct make<length_dimension, std::milli> {
  using type = millimeters;
};

template <>
struct make<length_dimension, std::micro> {
  using type = micrometers;
};

template <>
struct make<length_dimension, std::nano> {
  using type = nanometers;
};

template <>
struct make<length_dimension, std::pico> {
  using type = picometers;
};

// Imperial length units

struct miles {
  using dimension = length_dimension;
  using ratio = std::ratio<1609344, 1000>;
};

struct yards {
  using dimension = length_dimension;
  using ratio = std::ratio<9144, 10000>;
};

struct feet {
  using dimension = length_dimension;
  using ratio = std::ratio<3048, 10000>;
};

struct inches {
  using dimension = length_dimension;
  using ratio = std::ratio<254, 10000>;
};

template <>
struct make<length_dimension, std::ratio<1609344, 1000>> {
  using type = miles;
};

template <>
struct make<length_dimension, std::ratio<9144, 10000>> {
  using type = yards;
};

template <>
struct make<length_dimension, std::ratio<3048, 10000>> {
  using type = feet;
};

template <>
struct make<length_dimension, std::ratio<254, 10000>> {
  using type = inches;
};

}  // namespace unit

using kilometers = length<double, std::kilo>;
using meters = length<double>;
using decimeters = length<double, std::deci>;
using centimeters = length<double, std::centi>;
using millimeters = length<double, std::milli>;
using micrometers = length<double, std::micro>;
using nanometers = length<double, std::nano>;
using picometers = length<double, std::pico>;

using miles = length<double, std::ratio<1609344, 1000>>;
using yards = length<double, std::ratio<9144, 10000>>;
using feet = length<double, std::ratio<3048, 10000>>;
using inches = length<double, std::ratio<254, 10000>>;

namespace literals {

constexpr kilometers operator""_km(long double value) {
  return kilometers{value};
}
constexpr meters operator""_m(long double value) { return meters{value}; }
constexpr millimeters operator""_mm(long double value) {
  return millimeters{value};
}
constexpr micrometers operator""_um(long double value) {
  return micrometers{value};
}
constexpr nanometers operator""_nm(long double value) {
  return nanometers{value};
}
constexpr picometers operator""_pm(long double value) {
  return picometers{value};
}

constexpr miles operator""_mi(long double value) { return miles{value}; }
constexpr yards operator""_yd(long double value) { return yards{value}; }
constexpr feet operator""_ft(long double value) { return feet{value}; }
constexpr inches operator""_in(long double value) { return inches{value}; }

constexpr kilometers operator""_km(unsigned long long value) {
  return kilometers{value};
}
constexpr meters operator""_m(unsigned long long value) {
  return meters{value};
}
constexpr millimeters operator""_mm(unsigned long long value) {
  return millimeters{value};
}
constexpr micrometers operator""_um(unsigned long long value) {
  return micrometers{value};
}
constexpr nanometers operator""_nm(unsigned long long value) {
  return nanometers{value};
}
constexpr picometers operator""_pm(unsigned long long value) {
  return picometers{value};
}

constexpr miles operator""_mi(unsigned long long value) { return miles{value}; }
constexpr yards operator""_yd(unsigned long long value) { return yards{value}; }
constexpr feet operator""_ft(unsigned long long value) { return feet{value}; }
constexpr inches operator""_in(unsigned long long value) {
  return inches{value};
}

}  // namespace literals

}  // namespace scalr