/*
 * Scalr: Physical quantity/unit representation & manipulation library
 *
 * Copyright (c) 2020-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef SCALR_NAMED_QUANTITY_ANGULAR_LENGTH_HPP
#define SCALR_NAMED_QUANTITY_ANGULAR_LENGTH_HPP

#include <ratio>

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct angular_length_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<0, 0, 0, 0, 0, 0, 0, 1>;
};

template <typename Ratio>
struct angular_length_unit {
  using dimension = angular_length_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using angular_length =
    quantity<Rep, make_unit_t<angular_length_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<0, 0, 0, 0, 0, 0, 0, 1> {
  using type = angular_length_dimension;
};

}  // namespace dimension

namespace unit {
template <typename Ratio>
struct make<angular_length_dimension, Ratio> {
  using type = scalr::angular_length_unit<Ratio>;
};

}  // namespace unit
}  // namespace scalr

#endif
