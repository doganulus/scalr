/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_POP_HPP
#define SCALR_NAMED_QUANTITY_POP_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct pop_dimension {
  using signature = system_signature<-6, 1, 0, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct pop_unit {
  using dimension = pop_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using pop = quantity<Rep, make_unit_t<pop_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<-6, 1, 0, 0, 0, 0, 0, 0> {
  using type = pop_dimension;
};

}  // namespace dimension
}  // namespace scalr

#endif
