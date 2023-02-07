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

struct crackle_dimension {
  using signature = system_signature<-5, 1, 0, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct crackle_unit {
  using dimension = crackle_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using crackle = quantity<Rep, make_unit_t<crackle_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<-5, 1, 0, 0, 0, 0, 0, 0> {
  using type = crackle_dimension;
};

}  // namespace dimension
}  // namespace scalr
