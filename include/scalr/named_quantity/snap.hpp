/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2023 Dogan Ulus
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

struct snap_dimension {
  using signature = system_signature<-4, 1, 0, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct snap_unit {
  using dimension = snap_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using snap = quantity<Rep, make_unit_t<snap_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<-4, 1, 0, 0, 0, 0, 0, 0> {
  using type = snap_dimension;
};

}  // namespace dimension
}  // namespace scalr
