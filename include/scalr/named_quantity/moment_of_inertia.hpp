/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_MOMENT_OF_INERTIA_HPP
#define SCALR_NAMED_QUANTITY_MOMENT_OF_INERTIA_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct moment_of_inertia_dimension {
  using signature = system_signature<0, 2, 1, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct moment_of_inertia_unit {
  using dimension = moment_of_inertia_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using moment_of_inertia =
    quantity<Rep, make_unit_t<moment_of_inertia_dimension, Ratio>>;

namespace dimension {

template <>
struct make<0, 2, 1, 0, 0, 0, 0, 0> {
  using type = moment_of_inertia_dimension;
};

}  // namespace dimension
}  // namespace scalr

#endif
