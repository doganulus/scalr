# Core Design

Core library design is divided into three conceptual levels: Dimension types, Unit types, and Quantity class.

## Dimension types

The Scalr library is built upon the International System (SI) of Units, which specifies 7 fundamental dimensions for physical quantities - time, length, mass, temperature, electric current, amount of substance, and luminous intensity - each with its own base unit (second, meter, kilogram, kelvin, ampere, mole, and candela). Additionally, Scalr recognizes _angular length_ as an independent 8th dimension with the unit of arcmeters, which is not a standard SI unit.

```c++
// System: 7 fundamental SI dimensions + angular length
template <intmax_t T, intmax_t L, intmax_t M, intmax_t I, intmax_t K,
          intmax_t N, intmax_t J, intmax_t R>
struct system_signature {
  static constexpr intmax_t t = T;  // time dimension
  static constexpr intmax_t l = L;  // length dimension
  static constexpr intmax_t m = M;  // mass dimension
  static constexpr intmax_t i = I;  // electric current dimension
  static constexpr intmax_t k = K;  // temperature dimension
  static constexpr intmax_t n = N;  // amount of substance dimension
  static constexpr intmax_t j = J;  // luminous intensity dimension
  static constexpr intmax_t r = R;  // angular length dimension
};
```

In Scalr, a dimension is represented as a type that having a system_signature. The default dimension type is called `unnamed_dimension`, which is a struct template that can handle various combinations of fundamental dimensions.

```c++
template <intmax_t T, intmax_t L, intmax_t M, intmax_t I, intmax_t K,
          intmax_t N, intmax_t J, intmax_t R>
struct unnamed_dimension {
  using signature = system_signature<T, L, M, I, K, N, J, R>;
};
```

In contrast, a named dimension has a fixed `system_signature`. Scalr offers several named dimensions, including...

```c++
struct time_dimension { // in scalr/named_quantity/time.hpp
  using signature = system_signature<1, 0, 0, 0, 0, 0, 0, 0>;
};
struct angle_dimension { // in scalr/named_quantity/angle.hpp
  using signature = system_signature<0, -1, 0, 0, 0, 0, 0, 1>;
};
struct force_dimension { // in scalr/named_quantity/force.hpp
  using signature = system_signature<-2, 1, 1, 0, 0, 0, 0, 0>;
};
```

Scalr employs template metaprogramming with struct templates to support arithmetic operations for dimensions:

```c++
scalr::make_dimension_t<T, L, M, I, K, N, J, R>
scalr::dimension_inverse_t<Dimension>
scalr::dimension_product_t<Dimensions...>
scalr::dimension_exponent_t<Dimension, k>
scalr::dimension_equal<Dimension1, Dimension2>::value
```

## Unit types

In Scalr, a unit is represented as a type that contains two member types: a dimension and a ratio. The default unit type is called `unnamed_unit`, which is a struct template that can handle various combinations of dimensions and ratios.

```c++
template <class Dimension, class Ratio = std::ratio<1>>
struct unnamed_unit {
  using dimension = Dimension;
  using ratio = Ratio;
};
```

In contrast, a named unit has a fixed `dimension` and a `ratio`. Scalr offers several named units, including...

```c++
struct seconds { // defined in scalr/named_quantity/time.hpp
  using dimension = time_dimension; // base unit: second
  using ratio = std::ratio<1>;
};
struct miles_per_hour {  // defined in scalr/named_quantity/speed.hpp
  using dimension = speed_dimension; // base unit: meters per second
  using ratio = std::ratio<1397, 3125>; 
};
struct radians {   // defined in scalr/named_quantity/angle.hpp
  using dimension = angle_dimension; 
  using ratio = std::ratio<99532, 625378>;  // 1/(2*PI) Approx.
};
```

There is no other structure for the base unit of a dimension. It is implicit that the unit with `ratio<1>` is the base unit. 

Scalr employs template metaprogramming with struct templates to support arithmetic operations for units:

```c++
scalr::make_unit_t<Dimension, Ratio>
scalr::unit_inverse_t<Unit>
scalr::unit_sum_t<Units...>
scalr::unit_product_t<Units...>
scalr::unit_exponent_t<Unit, k>
scalr::unit_equal<Unit1, Unit2>::value
```

## Quantity Class

The quantity class is a wrapper class for the arithmetic type of `Rep` similar to `std::chrono::duration` and supports compile-time dimension analysis, unit conversions, and basic arithmetic. 

```c++
template <typename Rep, typename Unit>
struct quantity;
```

The quantity class supports basic comparison and arithmetic operations, which casts the result into a derived quantity type depending on the operand or causes a compilation error if the operation is illegal. 

### Named Quantities and Helpers

On the top of these mechanisms, Scalr provides a support for named quantities for a better user experience. The first level contains aliases for `quantity` class for each named quantity.

```c++
template <typename Rep, typename Ratio = std::ratio<1>>
using duration = quantity<ValueT, make_unit_t<time_dimension, Ratio>;

template <typename Rep, typename Ratio = std::ratio<1>>
using length = quantity<ValueT, make_unit_t<length_dimension, Ratio>;

template <typename Rep, typename Ratio = std::ratio<1>>
using speed = quantity<ValueT, make_unit_t<speed_dimension, Ratio>;
```

The full list of named quantities can be found [here]() and each can be included independently. Then not included named quantities would appear with `unnamed_unit`s of `unnamed_dimension`s.

Similar to `std::chrono::duration`, Scalr further provides predefined aliases for each named quantity. For example, the `speed` quantity has the following helper definitions.

```c++
using meters_per_second = speed<double, std::ratio<1>>;
using kilometers_per_hour = speed<double, std::ratio<5, 18>>;
using miles_per_hour = speed<double, std::ratio<1397, 3125>>;
```

Finally user-defined literals can be enabled via `using namespace scalr::literals`, which adds `_mps`, `_kph` and `_mph` suffixes to construct these types, respectively.
