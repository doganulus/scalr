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

template <typename Rep, typename Ratio = std::ratio<1>>
using temperature = quantity<Rep, make_unit_t<temperature_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, 0, 0, 1, 0, 0, 0, 0> {
  using type = temperature_dimension;
};

}  // namespace dimension

using kelvins = temperature<double, std::ratio<1>>;

}  // namespace scalr