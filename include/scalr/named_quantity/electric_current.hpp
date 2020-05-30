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

struct electric_current_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 0, 0, 0, 1, 0, 0, 0>;
};

template <typename Ratio>
struct electric_current_unit {
  using dimension = electric_current_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using electric_current =
    quantity<Rep, make_unit_t<electric_current_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, 0, 0, 0, 1, 0, 0, 0> {
  using type = electric_current_dimension;
};

}  // namespace dimension

namespace unit {
template <typename Ratio>
struct make<electric_current_dimension, Ratio> {
  using type = scalr::electric_current_unit<Ratio>;
};
}  // namespace unit

}  // namespace scalr