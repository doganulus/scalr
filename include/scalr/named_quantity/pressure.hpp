/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 * Copyright (c) 2026 Dmitriy Purgin
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_PRESSURE_HPP
#define SCALR_NAMED_QUANTITY_PRESSURE_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct pressure_dimension {
  // 7 Base SI dimensions + angle dimension
  // SI base units	s−2⋅m−1⋅kg
  using signature = system_signature<-2, -1, 1, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct pressure_unit {
  using dimension = pressure_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using pressure = quantity<Rep, make_unit_t<pressure_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<-2, -1, 1, 0, 0, 0, 0, 0> {
  using type = pressure_dimension;
};

}  // namespace dimension

namespace unit {

struct pascals {
  using dimension = pressure_dimension;
  using ratio = std::ratio<1>;
};

struct kilopascals {
  using dimension = pressure_dimension;
  using ratio = std::kilo;
};

struct bars {
  using dimension = pressure_dimension;
  using ratio = std::ratio<100000>;
};

template <typename Ratio>
struct make<pressure_dimension, Ratio> {
  using type = scalr::pressure_unit<Ratio>;
};

template <>
struct make<pressure_dimension, std::ratio<1>> {
  using type = pascals;
};

template <>
struct make<pressure_dimension, std::kilo> {
  using type = kilopascals;
};

template <>
struct make<pressure_dimension, std::ratio<100000>> {
  using type = bars;
};

}  // namespace unit

using pascals = pressure<double, std::ratio<1>>;
using kilopascals = pressure<double, std::kilo>;
using bars = pressure<double, std::ratio<100000>>;

namespace literals {

constexpr pascals operator""_Pa(long double value) { return pascals{value}; }
constexpr kilopascals operator""_kPa(long double value) {
  return kilopascals{value};
}
constexpr bars operator""_bar(long double value) { return bars{value}; }

constexpr pascals operator""_Pa(unsigned long long value) {
  return pascals{value};
}
constexpr kilopascals operator""_kPa(unsigned long long value) {
  return kilopascals{value};
}
constexpr bars operator""_bar(unsigned long long value) { return bars{value}; }

}  // namespace literals

}  // namespace scalr

// IO Helpers
#if defined(ENABLE_SCALR_IO)

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::pascals>& q) {
  os << q.value() << "Pa";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::kilopascals>& q) {
  os << q.value() << "kPa";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::bars>& q) {
  os << q.value() << "bar";
  return os;
}

#endif
#endif
