/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_VOLUME_HPP
#define SCALR_NAMED_QUANTITY_VOLUME_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct volume_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 3, 0, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct volume_unit {
  using dimension = volume_dimension;
  using ratio = Ratio;
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

struct cubic_meters {
  using dimension = volume_dimension;
  using ratio = std::ratio<1>;
};

struct liters {
  using dimension = volume_dimension;
  using ratio = std::ratio<100>;
};

struct milliliters {
  using dimension = volume_dimension;
  using ratio = std::ratio<10000>;
};

template <typename Ratio>
struct make<volume_dimension, Ratio> {
  using type = scalr::volume_unit<Ratio>;
};

template <>
struct make<volume_dimension, std::ratio<1>> {
  using type = cubic_meters;
};

template <>
struct make<volume_dimension, std::ratio<1, 1000>> {
  using type = liters;
};

template <>
struct make<volume_dimension, std::ratio<1, 1000000>> {
  using type = milliliters;
};

}  // namespace unit

using cubic_meters = volume<double, std::ratio<1>>;
using liters = volume<double, std::ratio<1, 1000>>;
using milliliters = volume<double, std::ratio<1, 1000000>>;

namespace literals {

constexpr cubic_meters operator""_m3(long double value) {
  return cubic_meters{value};
}
constexpr liters operator""_l(long double value) { return liters{value}; }

constexpr milliliters operator""_ml(long double value) {
  return milliliters{value};
}

constexpr cubic_meters operator""_m3(unsigned long long value) {
  return cubic_meters{value};
}
constexpr liters operator""_l(unsigned long long value) {
  return liters{value};
}

constexpr milliliters operator""_ml(unsigned long long value) {
  return milliliters{value};
}

}  // namespace literals

}  // namespace scalr

// IO Helpers
#if defined(ENABLE_SCALR_IO)

template <typename T>
std::ostream& operator<<(
    std::ostream& os, const scalr::quantity<T, scalr::unit::cubic_meters>& q) {
  os << q.value() << "m^3";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::liters>& q) {
  os << q.value() << "L";
  return os;
}

template <typename T>
std::ostream& operator<<(
    std::ostream& os, const scalr::quantity<T, scalr::unit::milliliters>& q) {
  os << q.value() << "mL";
  return os;
}

#endif
#endif
