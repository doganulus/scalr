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

struct temperature_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 0, 0, 1, 0, 0, 0, 0>;
};

template <typename Ratio>
struct temperature_unit {
  using dimension = temperature_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using temperature = quantity<Rep, make_unit_t<temperature_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, 0, 0, 1, 0, 0, 0, 0> {
  using type = temperature_dimension;
};

}  // namespace dimension

namespace unit {

struct kelvins {
  using dimension = temperature_dimension;
  using ratio = std::ratio<1>;
};

template <>
struct make<temperature_dimension, std::ratio<1>> {
  using type = kelvins;
};

template <typename Ratio>
struct make<temperature_dimension, Ratio> {
  using type = scalr::temperature_unit<Ratio>;
};

}  // namespace unit

using kelvins = temperature<double, std::ratio<1>>;

}  // namespace scalr

// IO Helpers
#if defined(ENABLE_SCALR_IO)

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::kelvins>& q) {
  os << q.value() << "K";
  return os;
}

#endif