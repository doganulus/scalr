/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_SOLID_ANGLE_HPP
#define SCALR_NAMED_QUANTITY_SOLID_ANGLE_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct solid_angle_dimension {
  using signature = system_signature<0, -2, 0, 0, 0, 0, 0, 2>;
};

template <typename Ratio>
struct solid_angle_unit {
  using dimension = solid_angle_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using solid_angle = quantity<Rep, make_unit_t<solid_angle_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, -2, 0, 0, 0, 0, 0, 2> {
  using type = solid_angle_dimension;
};

}  // namespace dimension
}  // namespace scalr

#endif
