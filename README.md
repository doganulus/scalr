# Scalr
Scalr is a `std::chrono` inspired C++ library for the representation and manipulation of scalar physical quantities and their units of measurements. 

Scalr is a work in progress... 

## Features and goals

* C++11/14/17 compatibility
* Compile-time dimension analysis
* User experience of popular `std::chrono` library
* Shorter and more meaningful compilation errors --> good to have 
* User-defined literals for units and std::cout support --> good to have 

## Quick look

Scalr is based on the International System (SI) of Units starting with seven base dimensions. In the nutshell, Scalr provides a generic compile-time dimension/unit arithmetic as well as defines a facade for named quantities with `std::chrono::duration` style. Using Scalr, we define physical quantities as follows:

```c++
    scalr::duration<intmax_t, std::ratio<1>> secs(120); 
    scalr::speed<double, std::ratio<10, 36>> kphs(18.0);
    scalr::length<double, std::ratio<1000>> dist(0.6);

    CHECK(dist == (kph * secs)); // Pass
```

However, the compiler would complain if you make a dimensional mistake such as: 
```c++
    static_assert(kms == (kph * secs)); // Fail
```

Such compile-time guarantees and automatic (lazy) conversions between units are the reason why you should use Scalr.

Moreover, a number of helper aliases is available for named units. For example, 

```c++
using hours = duration<intmax_t, std::ratio<3600>>;
using minutes = duration<intmax_t, std::ratio<60>>;
using seconds = duration<intmax_t>;

using meters_per_second = speed<double, std::ratio<1>>;
using kilometers_per_hour = speed<double, std::ratio<5, 18>>;
using miles_per_hour = speed<double, std::ratio<1397, 3125>>;

using kilohertz = frequency<double, std::kilo>;

using grams = mass<double, std::ratio<1, 1000>>;
```

The source directory of named quantities and unit can be found [here](https://github.com/doganulus/scalr/tree/master/include/scalr/named_quantity) for the reference. 

Finally the [design document](https://github.com/doganulus/scalr/tree/master/docs/design.md) gives more information about the underlying structures and how it works.

## Other Libraries

There are quite a number of similar libraries in existence. Some prominent ones are listed below, with my short comments.

* [boost/units](https://www.boost.org/doc/libs/1_51_0/doc/html/boost_units.html) - Heavy design. I think `std::chrono` does a better job to handle time quantities.
* [mputsz/units](https://github.com/mpusz/units) - New proposal for the future C++23 standard, not compatible with C++11/14/17. Besides I am not in favor of explicit systems as in Boost.
* [PhysUnits-CT-Cpp11](https://github.com/martinmoene/PhysUnits-CT-Cpp11) - Close to Scalr in many ways but eagerly converts quantities to base units (unlike the lazy way in std::chrono::duration implementations, which has certain advantages).
* [nholthaus/units](https://github.com/nholthaus/units) - Has some very nice features but not compatible with C++11. Besides representation type cannot be selected independently.