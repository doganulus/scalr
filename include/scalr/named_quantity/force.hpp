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

struct force_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<-2, 1, 1, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct force_unit {
  using dimension = force_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using force = quantity<Rep, make_unit_t<force_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<-2, 1, 1, 0, 0, 0, 0, 0> {
  using type = force_dimension;
};

}  // namespace dimension

namespace unit {

struct newtons {
  using dimension = force_dimension;
  using ratio = std::ratio<1>;
};

template <typename Ratio>
struct make<force_dimension, Ratio> {
  using type = scalr::force_unit<Ratio>;
};

template <>
struct make<force_dimension, std::ratio<1>> {
  using type = newtons;
};

}  // namespace unit

using newtons = force<double, std::ratio<1>>;

namespace literals {

constexpr newtons operator""_N(long double value) { return newtons{value}; }

constexpr newtons operator""_N(unsigned long long value) {
  return newtons{value};
}

}  // namespace literals

}  // namespace scalr

// IO Helpers
#if defined(ENABLE_SCALR_IO)

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::newtons>& q) {
  os << q.value() << "N";
  return os;
}

#endif