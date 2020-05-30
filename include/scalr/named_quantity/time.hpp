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

#if defined(ENABLE_SCALR_IO)
#include <ostream>
#endif

#include "scalr/dimension.hpp"
#include "scalr/quantity.hpp"
#include "scalr/unit.hpp"

namespace scalr {

struct time_dimension {
  // 7 Base SI dimensions + angle dimension
  using signature = system_signature<1, 0, 0, 0, 0, 0, 0, 0>;
};

template <typename Ratio>
struct time_unit {
  using dimension = time_dimension;
  using ratio = Ratio;
};

template <typename Rep, typename Ratio = std::ratio<1>>
using duration = quantity<Rep, make_unit_t<time_dimension, Ratio>>;

// Specializations
namespace dimension {
template <>
struct make<1, 0, 0, 0, 0, 0, 0, 0> {
  using type = time_dimension;
};

}  // namespace dimension

namespace unit {

struct hours {
  using dimension = time_dimension;
  using ratio = std::ratio<3600>;
};

struct minutes {
  using dimension = time_dimension;
  using ratio = std::ratio<60>;
};

struct seconds {
  using dimension = time_dimension;
  using ratio = std::ratio<1>;
};

struct milliseconds {
  using dimension = time_dimension;
  using ratio = std::milli;
};

struct microseconds {
  using dimension = time_dimension;
  using ratio = std::micro;
};

struct nanoseconds {
  using dimension = time_dimension;
  using ratio = std::nano;
};

struct picoseconds {
  using dimension = time_dimension;
  using ratio = std::pico;
};

template <typename Ratio>
struct make<time_dimension, Ratio> {
  using type = scalr::time_unit<Ratio>;
};

template <>
struct make<time_dimension, std::ratio<3600>> {
  using type = hours;
};

template <>
struct make<time_dimension, std::ratio<60>> {
  using type = minutes;
};

template <>
struct make<time_dimension, std::ratio<1>> {
  using type = seconds;
};

template <>
struct make<time_dimension, std::milli> {
  using type = milliseconds;
};

template <>
struct make<time_dimension, std::micro> {
  using type = microseconds;
};

template <>
struct make<time_dimension, std::nano> {
  using type = nanoseconds;
};

template <>
struct make<time_dimension, std::pico> {
  using type = picoseconds;
};

}  // namespace unit

// Helper aliases
using hours = duration<intmax_t, std::ratio<3600>>;
using minutes = duration<intmax_t, std::ratio<60>>;
using seconds = duration<intmax_t>;
using milliseconds = duration<intmax_t, std::ratio<1, 1000>>;
using microseconds = duration<intmax_t, std::ratio<1, 1000000>>;
using nanoseconds = duration<intmax_t, std::ratio<1, 1000000000>>;
using picoseconds = duration<intmax_t, std::ratio<1, 1000000000000>>;

namespace literals {

constexpr duration<long double, std::ratio<3600>> operator""_h(
    long double value) {
  return duration<long double, std::ratio<3600>>{value};
}
constexpr duration<long double, std::ratio<60>> operator""_min(
    long double value) {
  return duration<long double, std::ratio<60>>{value};
}
constexpr duration<long double> operator""_s(long double value) {
  return duration<long double>{value};
}
constexpr duration<long double, std::milli> operator""_ms(long double value) {
  return duration<long double, std::milli>{value};
}
constexpr duration<long double, std::micro> operator""_us(long double value) {
  return duration<long double, std::micro>{value};
}
constexpr duration<long double, std::nano> operator""_ns(long double value) {
  return duration<long double, std::nano>{value};
}
constexpr duration<long double, std::pico> operator""_ps(long double value) {
  return duration<long double, std::pico>{value};
}

constexpr hours operator""_h(unsigned long long value) { return hours{value}; }
constexpr minutes operator""_min(unsigned long long value) {
  return minutes{value};
}
constexpr seconds operator""_s(unsigned long long value) {
  return seconds{value};
}
constexpr milliseconds operator""_ms(unsigned long long value) {
  return milliseconds{value};
}
constexpr microseconds operator""_us(unsigned long long value) {
  return microseconds{value};
}
constexpr nanoseconds operator""_ns(unsigned long long value) {
  return nanoseconds{value};
}
constexpr picoseconds operator""_ps(unsigned long long value) {
  return picoseconds{value};
}

}  // namespace literals

}  // namespace scalr

// IO Helpers
#if defined(ENABLE_SCALR_IO)

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::hours>& q) {
  os << q.value() << "h";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::minutes>& q) {
  os << q.value() << "min";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const scalr::quantity<T, scalr::unit::seconds>& q) {
  os << q.value() << "s";
  return os;
}

template <typename T>
std::ostream& operator<<(
    std::ostream& os, const scalr::quantity<T, scalr::unit::milliseconds>& q) {
  os << q.value() << "ms";
  return os;
}

template <typename T>
std::ostream& operator<<(
    std::ostream& os, const scalr::quantity<T, scalr::unit::microseconds>& q) {
  os << q.value() << "us";
  return os;
}

template <typename T>
std::ostream& operator<<(
    std::ostream& os, const scalr::quantity<T, scalr::unit::nanoseconds>& q) {
  os << q.value() << "ns";
  return os;
}

template <typename T>
std::ostream& operator<<(
    std::ostream& os, const scalr::quantity<T, scalr::unit::picoseconds>& q) {
  os << q.value() << "ps";
  return os;
}

#endif