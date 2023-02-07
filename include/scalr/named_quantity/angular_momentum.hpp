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

struct angular_momentum_dimension {
  using signature = system_signature<-1, 1, 1, 0, 0, 0, 0, 1>;
};

template <typename Ratio>
struct angular_momentum_unit {
  using dimension = angular_momentum_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using angular_momentum =
    quantity<Rep, make_unit_t<angular_momentum_dimension, Ratio>>;

namespace dimension {

template <>
struct make<-1, 1, 1, 0, 0, 0, 0, 1> {
  using type = angular_momentum_dimension;
};

}  // namespace dimension
}  // namespace scalr
