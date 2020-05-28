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

struct frequency_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<-1, 0, 0, 0, 0, 0, 0, 0>;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using frequency = quantity<Rep, make_unit_t<frequency_dimension, Ratio>>;

// Detail and specializations
namespace dimension {

template <>
struct make<-1, 0, 0, 0, 0, 0, 0, 0> {
  using type = frequency_dimension;
};

}  // namespace dimension

namespace unit {
struct terahertz {
  using dimension = frequency_dimension;
  using ratio = std::tera;
};

struct gigahertz {
  using dimension = frequency_dimension;
  using ratio = std::giga;
};

struct megahertz {
  using dimension = frequency_dimension;
  using ratio = std::mega;
};

struct kilohertz {
  using dimension = frequency_dimension;
  using ratio = std::kilo;
};

struct hertz {
  using dimension = frequency_dimension;
  using ratio = std::ratio<1>;
};

struct millihertz {
  using dimension = frequency_dimension;
  using ratio = std::milli;
};

template <>
struct make<frequency_dimension, std::tera> {
  using type = terahertz;
};

template <>
struct make<frequency_dimension, std::giga> {
  using type = gigahertz;
};

template <>
struct make<frequency_dimension, std::mega> {
  using type = megahertz;
};

template <>
struct make<frequency_dimension, std::kilo> {
  using type = kilohertz;
};

template <>
struct make<frequency_dimension, std::ratio<1>> {
  using type = hertz;
};

template <>
struct make<frequency_dimension, std::milli> {
  using type = millihertz;
};

}  // namespace unit

// Helper aliases
using terahertz = frequency<double, std::tera>;
using gigahertz = frequency<double, std::giga>;
using megahertz = frequency<double, std::mega>;
using kilohertz = frequency<double, std::kilo>;
using hertz = frequency<double>;
using millihertz = frequency<double, std::milli>;

namespace literals {

constexpr terahertz operator""_THz(long double value) {
  return terahertz{value};
}
constexpr gigahertz operator""_Ghz(long double value) {
  return gigahertz{value};
}
constexpr megahertz operator""_MHz(long double value) {
  return megahertz{value};
}
constexpr kilohertz operator""_kHz(long double value) {
  return kilohertz{value};
}
constexpr hertz operator""_Hz(long double value) { return hertz{value}; }

constexpr millihertz operator""_mHz(long double value) {
  return millihertz{value};
}

constexpr terahertz operator""_THz(unsigned long long value) {
  return terahertz{value};
}
constexpr gigahertz operator""_Ghz(unsigned long long value) {
  return gigahertz{value};
}
constexpr megahertz operator""_MHz(unsigned long long value) {
  return megahertz{value};
}
constexpr kilohertz operator""_kHz(unsigned long long value) {
  return kilohertz{value};
}
constexpr hertz operator""_Hz(unsigned long long value) { return hertz{value}; }

constexpr millihertz operator""_mHz(unsigned long long value) {
  return millihertz{value};
}
}  // namespace literals

}  // namespace scalr