/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_AREA_HPP
#define SCALR_NAMED_QUANTITY_AREA_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct area_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 2, 0, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct area_unit {
  using dimension = area_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using area = quantity<Rep, make_unit_t<area_dimension, Ratio>>;

// Detail and specializations
namespace dimension {
template <>
struct make<0, 2, 0, 0, 0, 0, 0, 0> {
  using type = area_dimension;
};
}  // namespace dimension

namespace unit {

struct square_meters {
  using dimension = area_dimension;
  using ratio = std::ratio<1>;
};

struct acres {
  using dimension = area_dimension;
  using ratio = std::ratio<100>;
};

struct hectares {
  using dimension = area_dimension;
  using ratio = std::ratio<10000>;
};

struct square_kilometers {
  using dimension = area_dimension;
  using ratio = std::ratio<1000000>;
};

template <typename Ratio>
struct make<area_dimension, Ratio> {
  using type = scalr::area_unit<Ratio>;
};

template <>
struct make<area_dimension, std::ratio<1>> {
  using type = square_meters;
};

template <>
struct make<area_dimension, std::ratio<100>> {
  using type = acres;
};

template <>
struct make<area_dimension, std::ratio<10000>> {
  using type = hectares;
};

template <>
struct make<area_dimension, std::ratio<1000000>> {
  using type = square_kilometers;
};

}  // namespace unit

using square_meters = area<double, std::ratio<1>>;
using acres = area<double, std::ratio<1>>;
using hectares = area<double, std::ratio<10000>>;
using square_kilometers = area<double, std::ratio<1000000>>;

namespace literals {

constexpr square_meters operator""_m2(long double value) {
  return square_meters{value};
}
constexpr hectares operator""_ha(long double value) { return hectares{value}; }
constexpr square_kilometers operator""_km2(long double value) {
  return square_kilometers{value};
}

constexpr square_meters operator""_m2(unsigned long long value) {
  return square_meters{value};
}
constexpr hectares operator""_ha(unsigned long long value) {
  return hectares{value};
}
constexpr square_kilometers operator""_km2(unsigned long long value) {
  return square_kilometers{value};
}

}  // namespace literals

}  // namespace scalr

// IO Helpers
#if defined(ENABLE_SCALR_IO)

template <typename T>
std::ostream& operator<<(
    std::ostream& os, const scalr::quantity<T, scalr::unit::square_meters>& q) {
  os << q.value() << "m^2";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::hectares>& q) {
  os << q.value() << "ha";
  return os;
}

template <typename T>
std::ostream& operator<<(
    std::ostream& os,
    const scalr::quantity<T, scalr::unit::square_kilometers>& q) {
  os << q.value() << "km^2";
  return os;
}

#endif
#endif
