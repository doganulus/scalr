/*
 * Scalr: Physical quantity/unit value_typeresentation & manipulation library
 *
 * Copyright (c) 2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <limits>
#include <ratio>
#include <type_traits>

#include "scalr/dimension.hpp"
#include "scalr/unit.hpp"

namespace scalr {
template <typename Rep, typename Unit>
struct quantity;

template <typename T>
struct is_quantity : std::false_type {};

template <typename Rep, typename Unit>
struct is_quantity<quantity<Rep, Unit>> : std::true_type {};

// TODO: Use this to implement linear/rotational range
// PLUS: What about Ada-like subtypes?
template <typename Rep>
struct linear_range {
  static constexpr Rep zero() noexcept { return Rep(0); }

  static constexpr Rep max() noexcept {
    return std::numeric_limits<Rep>::max();
  }

  static constexpr Rep min() noexcept {
    return std::numeric_limits<Rep>::lowest();
  }
};

// Quantity cast implementation
// Converts a scalr::quantity to another quantity of the same dimension.
namespace detail {
template <typename TargetT, typename CommonRatio, typename CommonRep,
          bool _NumIsOne = false, bool _DenIsOne = false>
struct quantity_cast_impl {
  template <typename Rep, typename Unit>
  static constexpr TargetT cast(const quantity<Rep, Unit>& other) {
    using TargetRep = typename TargetT::value_type;
    return TargetT(
        static_cast<TargetRep>(static_cast<CommonRep>(other.value()) *
                               static_cast<CommonRep>(CommonRatio::num) /
                               static_cast<CommonRep>(CommonRatio::den)));
  }
};

template <typename TargetT, typename CommonRatio, typename CommonRep>
struct quantity_cast_impl<TargetT, CommonRatio, CommonRep, true, true> {
  template <typename Rep, typename Unit>
  static constexpr TargetT cast(const quantity<Rep, Unit>& other) {
    using TargetRep = typename TargetT::value_type;
    return TargetT(static_cast<TargetRep>(other.value()));
  }
};

template <typename TargetT, typename CommonRatio, typename CommonRep>
struct quantity_cast_impl<TargetT, CommonRatio, CommonRep, true, false> {
  template <typename Rep, typename Unit>
  static constexpr TargetT cast(const quantity<Rep, Unit>& other) {
    using TargetRep = typename TargetT::value_type;
    return TargetT(
        static_cast<TargetRep>(static_cast<CommonRep>(other.value()) /
                               static_cast<CommonRep>(CommonRatio::den)));
  }
};

template <typename TargetT, typename CommonRatio, typename CommonRep>
struct quantity_cast_impl<TargetT, CommonRatio, CommonRep, false, true> {
  template <typename Rep, typename Unit>
  static constexpr TargetT cast(const quantity<Rep, Unit>& other) {
    using TargetRep = typename TargetT::value_type;
    return TargetT(
        static_cast<TargetRep>(static_cast<CommonRep>(other.value()) *
                               static_cast<CommonRep>(CommonRatio::num)));
  }
};

}  // namespace detail

template <typename T>
using enable_if_is_quantity =
    typename std::enable_if<is_quantity<T>::value, T>::type;

template <class Target, class Rep2, class Unit2>
constexpr enable_if_is_quantity<Target> quantity_cast(
    const quantity<Rep2, Unit2>& other) {
  using common_factor =
      std::ratio_divide<typename Unit2::ratio, typename Target::ratio>;

  using TargetRep = typename Target::value_type;
  using common_value_t =
      typename std::common_type<Rep2, TargetRep, intmax_t>::type;

  using implementation =
      detail::quantity_cast_impl<Target, common_factor, common_value_t,
                                 common_factor::num == 1,
                                 common_factor::den == 1>;

  return implementation::cast(other);
}

//******************************
// Quantity Implementation
//******************************

template <typename Rep, typename Unit>
struct quantity {
  using value_type = Rep;
  using dimension = typename Unit::dimension;
  using ratio = typename Unit::ratio;
  using unit = make_unit_t<dimension, ratio>;

  using type = quantity<value_type, unit>;

  static_assert(!is_quantity<value_type>::value,
                "quantity can't have quantity as first template argument");
  static_assert(
      detail::is_unit_type<unit>::value,
      "quantity unit must have a physical unit (or be dimensionless)");
  static_assert(unit::ratio::num > 0, "physical unit ratio must be positive");

  constexpr quantity() = default;
  quantity(const quantity&) = default;
  quantity(quantity&&) = default;

  template <typename Rep2,
            typename std::enable_if<
                std::is_convertible<const Rep2&, value_type>::value &&
                    (std::is_floating_point<value_type>::value ||
                     !std::is_floating_point<Rep2>::value),
                int>::type = 0>
  constexpr explicit quantity(const Rep2& valueep)
      : value_(static_cast<value_type>(valueep)) {}

  template <typename Rep2, typename Unit2,
            typename std::enable_if<
                std::is_same<typename Unit2::dimension, dimension>::value &&
                    (std::is_floating_point<value_type>::value ||
                     (std::integral_constant<
                          bool, std::ratio_divide<typename Unit2::ratio,
                                                  ratio>::den == 1>::value &&
                      !std::is_floating_point<Rep2>::value)),
                int>::type = 0>
  constexpr quantity(const quantity<Rep2, Unit2>& other)
      : value_(quantity_cast<quantity>(other).value()) {}

  ~quantity() = default;
  quantity& operator=(const quantity&) = default;
  quantity& operator=(quantity&&) = default;

  constexpr value_type value() const { return value_; }

  constexpr quantity operator+() const { return *this; }
  constexpr quantity operator-() const { return quantity(-value_); }

  quantity& operator+=(const quantity& other) {
    value_ += other.value();
    return *this;
  }

  quantity& operator-=(const quantity& other) {
    value_ -= other.value();
    return *this;
  }

  quantity& operator*=(const value_type& valuehs) {
    value_ *= valuehs;
    return *this;
  }

  quantity& operator/=(const value_type& valuehs) {
    value_ /= valuehs;
    return *this;
  }

  template <typename Rep2, typename Unit2>
  typename std::enable_if<!std::is_floating_point<Rep>::value &&
                              !std::is_floating_point<Rep2>::value &&
                              std::is_same<typename Unit::dimension,
                                           typename Unit2::dimension>::value,
                          quantity&>::type
  operator%=(const quantity<Rep2, Unit2>& other) {
    value_ %= other.value();
    return *this;
  }

  static constexpr quantity zero() noexcept {
    return quantity(linear_range<value_type>::zero());
  }

  static constexpr quantity min() noexcept {
    return quantity(linear_range<value_type>::min());
  }

  static constexpr quantity max() noexcept {
    return quantity(linear_range<value_type>::max());
  }

 private:
  value_type value_;
};

namespace quantity_type {

template <typename Q1>
struct inverse {
  using V1 = typename Q1::value_type;
  using U1 = typename Q1::unit;

  using type = quantity<V1, unit_inverse_t<U1>>;
};

template <typename Q1, typename Q2>
struct sum {
  using V1 = typename Q1::value_type;
  using V2 = typename Q2::value_type;
  using U1 = typename Q1::unit;
  using U2 = typename Q2::unit;

  using common_value_type = typename std::common_type<V1, V2>::type;
  using unit_sum_type = scalr::unit_sum_t<U1, U2>;
  using type = quantity<common_value_type, unit_sum_type>;
};

template <typename Q1, typename Q2>
struct product {
  using V1 = typename Q1::value_type;
  using V2 = typename Q2::value_type;
  using U1 = typename Q1::unit;
  using U2 = typename Q2::unit;
  using common_value_type = typename std::common_type<V1, V2>::type;
  using unit_product_type = scalr::unit_product_t<U1, U2>;
  using type = quantity<common_value_type, unit_product_type>;
};
}  // namespace quantity_type

// Helper types
template <class Q1>
using quantity_inverse_t = typename quantity_type::inverse<Q1>::type;

template <class Q1, class... Qn>
using quantity_sum_t = typename quantity_type::sum<Q1, Qn...>::type;

template <class Q1, class... Qn>
using quantity_product_t = typename quantity_type::product<Q1, Qn...>::type;

// Comparison operators
template <typename T1, typename U1, typename T2, typename U2>
constexpr bool operator==(const quantity<T1, U1>& left,
                          const quantity<T2, U2>& right) {
  using stype = quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>>;
  return stype(left).value() == stype(right).value();
}

template <typename T1, typename U1, typename T2, typename U2>
constexpr bool operator<(const quantity<T1, U1>& left,
                         const quantity<T2, U2>& right) {
  using stype = quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>>;
  return stype(left).value() < stype(right).value();
}

template <typename T1, typename U1, typename T2, typename U2>
constexpr bool operator<=(const quantity<T1, U1>& left,
                          const quantity<T2, U2>& right) {
  return !(right < left);
}

template <typename T1, typename U1, typename T2, typename U2>
constexpr bool operator!=(const quantity<T1, U1>& left,
                          const quantity<T2, U2>& right) {
  return not(left == right);
}

template <typename T1, typename U1, typename T2, typename U2>
constexpr bool operator>(const quantity<T1, U1>& left,
                         const quantity<T2, U2>& right) {
  return right < left;
}

template <typename T1, typename U1, typename T2, typename U2>
constexpr bool operator>=(const quantity<T1, U1>& left,
                          const quantity<T2, U2>& right) {
  return !(left < right);
}

// Arithmetic operations
// The sum between two physical quantities having the same dimension
template <typename T1, typename U1, typename T2, typename U2>
constexpr quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>> operator+(
    const quantity<T1, U1>& left, const quantity<T2, U2>& right) {
  static_assert(
      std::is_same<typename U1::dimension, typename U2::dimension>::value,
      "unit dimensions must match for addition");
  using stype = quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>>;
  return stype(stype(left).value() + stype(right).value());
}

// The difference between two physical quantities having the same dimension
template <typename T1, typename U1, typename T2, typename U2>
constexpr quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>> operator-(
    const quantity<T1, U1>& left, const quantity<T2, U2>& right) {
  static_assert(
      std::is_same<typename U1::dimension, typename U2::dimension>::value,
      "unit dimensions must match for subtraction");
  using stype = quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>>;
  return stype(stype(left).value() - stype(right).value());
}

// The multiplication by scalar
template <typename Rep1, typename Rep2, typename Unit2>
constexpr quantity<typename std::common_type<Rep1, Rep2>::type, Unit2>
operator*(const quantity<Rep2, Unit2>& left, const Rep1& right) {
  using value_type = typename std::common_type<Rep1, Rep2>::type;
  using ptype = quantity<value_type, Unit2>;
  return ptype(ptype(left).value() * static_cast<value_type>(right));
}

// COMMENT: Left multiply to scalar may be ambigious 3 * 3_Hz == 9Hz or 1Hz
template <typename Rep1, typename Rep2, typename Unit2>
constexpr quantity<typename std::common_type<Rep1, Rep2>::type, Unit2>
operator*(const Rep1& left, const quantity<Rep2, Unit2>& right) {
  using value_type = typename std::common_type<Rep1, Rep2>::type;
  using ptype = quantity<value_type, Unit2>;
  return ptype(static_cast<value_type>(left) * ptype(right).value());
}

// The multiplication between two physical quantities
// TODO: Safer multiplication
template <typename T1, typename U1, typename T2, typename U2>
constexpr quantity_product_t<quantity<T1, U1>, quantity<T2, U2>> operator*(
    const quantity<T1, U1>& left, const quantity<T2, U2>& right) {
  using ptype = quantity_product_t<quantity<T1, U1>, quantity<T2, U2>>;
  return ptype(left.value() * right.value());
}

// The division by scalar
template <typename Rep1, typename Rep2, typename Unit2>
typename std::enable_if<
    std::is_convertible<Rep1,
                        typename std::common_type<Rep1, Rep2>::type>::value,
    quantity<typename std::common_type<Rep1, Rep2>::type, Unit2>>::type
operator/(const quantity<Rep2, Unit2>& left, const Rep1& right) {
  using value_type = typename std::common_type<Rep1, Rep2>::type;
  using ptype = quantity<value_type, Unit2>;
  return ptype(ptype(left).value() / static_cast<value_type>(right));
}

template <typename Rep1, typename Rep2, typename Unit2>
typename std::enable_if<std::is_convertible<Rep1, typename std::common_type<
                                                      Rep1, Rep2>::type>::value,
                        quantity<typename std::common_type<Rep1, Rep2>::type,
                                 unit_inverse_t<Unit2>>>::type
operator/(const Rep1& left, const quantity<Rep2, Unit2>& right) {
  using value_type = typename std::common_type<Rep1, Rep2>::type;
  using ptype = quantity<value_type, unit_inverse_t<Unit2>>;
  return ptype(static_cast<value_type>(left) /
               static_cast<value_type>(right.value()));
}

// The division between two quantities belonging to different dimensions
template <typename T1, typename U1, typename T2, typename U2,
          typename std::enable_if<!std::is_same<typename U1::dimension,
                                                typename U2::dimension>::value,
                                  int>::type = 0>
constexpr quantity_product_t<quantity<T1, U1>,
                             quantity_inverse_t<quantity<T2, U2>>>
operator/(const quantity<T1, U1>& left, const quantity<T2, U2>& right) {
  using ptype = quantity_product_t<quantity<T1, U1>,
                                   quantity_inverse_t<quantity<T2, U2>>>;
  return ptype(left.value() / right.value());
}

// The division between two quantities belonging to the same dimension
template <typename T1, typename U1, typename T2, typename U2,
          typename std::enable_if<std::is_same<typename U1::dimension,
                                               typename U2::dimension>::value,
                                  int>::type = 0>
constexpr typename std::common_type<T1, T2>::type operator/(
    const quantity<T1, U1>& left, const quantity<T2, U2>& right) {
  using vtype = typename std::common_type<T1, T2>::type;
  using stype = quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>>;
  return vtype(stype(left).value() / stype(right).value());
}

// The modulo between two physical quantities having the same dimension
template <typename T1, typename U1, typename T2, typename U2>
constexpr quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>> operator%(
    const quantity<T1, U1>& left, const quantity<T2, U2>& right) {
  static_assert(
      std::is_same<typename U1::dimension, typename U2::dimension>::value,
      "unit dimensions must match for modulo operation");
  using stype = quantity_sum_t<quantity<T1, U1>, quantity<T2, U2>>;
  return stype(stype(left).value() % stype(right).value());
}

template <typename Rep, typename Unit,
          typename std::enable_if<quantity<Rep, Unit>::min() <
                                      quantity<Rep, Unit>::zero(),
                                  int>::type = 0>
constexpr quantity<Rep, Unit> abs(const quantity<Rep, Unit>& value) {
  return value >= value.zero() ? value : -value;
}

}  // namespace scalr

namespace std {
template <class Dimension1, class Ratio1, class Dimension2, class Ratio2>
struct common_type<scalr::quantity<Dimension1, Ratio1>,
                   scalr::quantity<Dimension2, Ratio2>> {
  using type = scalr::quantity_sum_t<scalr::quantity<Dimension1, Ratio1>,
                                     scalr::quantity<Dimension2, Ratio2>>;
};
}  // namespace std
