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

struct speed_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<-1, 1, 0, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct speed_unit {
  using dimension = speed_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using speed = quantity<Rep, make_unit_t<speed_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<-1, 1, 0, 0, 0, 0, 0, 0> {
  using type = speed_dimension;
};

}  // namespace dimension

namespace unit {

struct meters_per_second {
  using dimension = speed_dimension;
  using ratio = std::ratio<1>;
};

struct kilometers_per_hour {
  using dimension = speed_dimension;
  using ratio = std::ratio<5, 18>;
};

struct miles_per_hour {
  using dimension = speed_dimension;
  using ratio = std::ratio<1397, 3125>;
};

template <typename Ratio>
struct make<speed_dimension, Ratio> {
  using type = scalr::speed_unit<Ratio>;
};

template <>
struct make<speed_dimension, std::ratio<1>> {
  using type = meters_per_second;
};

template <>
struct make<speed_dimension, std::ratio<5, 18>> {
  using type = kilometers_per_hour;
};

template <>
struct make<speed_dimension, std::ratio<1397, 3125>> {
  using type = miles_per_hour;
};

}  // namespace unit

using meters_per_second = speed<double, std::ratio<1>>;
using kilometers_per_hour = speed<double, std::ratio<5, 18>>;
using miles_per_hour = speed<double, std::ratio<1397, 3125>>;

namespace literals {

constexpr kilometers_per_hour operator""_kph(long double value) {
  return kilometers_per_hour{value};
}
constexpr miles_per_hour operator""_mph(long double value) {
  return miles_per_hour{value};
}
constexpr meters_per_second operator""_mps(long double value) {
  return meters_per_second{value};
}

constexpr kilometers_per_hour operator""_kph(unsigned long long value) {
  return kilometers_per_hour{value};
}
constexpr miles_per_hour operator""_mph(unsigned long long value) {
  return miles_per_hour{value};
}
constexpr meters_per_second operator""_mps(unsigned long long value) {
  return meters_per_second{value};
}

}  // namespace literals

}  // namespace scalr

// IO Helpers
#if defined(ENABLE_SCALR_IO)

template <typename T>
std::ostream& operator<<(
    std::ostream& os,
    const scalr::quantity<T, scalr::unit::meters_per_second>& q) {
  os << q.value() << "mps";
  return os;
}

template <typename T>
std::ostream& operator<<(
    std::ostream& os,
    const scalr::quantity<T, scalr::unit::kilometers_per_hour>& q) {
  os << q.value() << "kph";
  return os;
}

template <typename T>
std::ostream& operator<<(
    std::ostream& os,
    const scalr::quantity<T, scalr::unit::miles_per_hour>& q) {
  os << q.value() << "mph";
  return os;
}

#endif