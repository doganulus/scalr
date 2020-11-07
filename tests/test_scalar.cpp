
#include <iostream>
#include <type_traits>

#define ENABLE_SCALR_IO
#include "scalr/scalr.hpp"

#include "third_party/catch.hpp"

TEST_CASE("Dimension") {
  using namespace scalr;

  SECTION("Arithmetic") {
    CHECK(std::is_same<time_dimension,
                       make_dimension_t<1, 0, 0, 0, 0, 0, 0, 0>>::value);

    CHECK(std::is_same<unnamed_dimension<1, 2, 3, 4, 5, 6, 7, 8>,
                       make_dimension_t<1, 2, 3, 4, 5, 6, 7, 8>>::value);

    CHECK(std::is_same<frequency_dimension,
                       dimension_inverse_t<time_dimension>>::value);

    CHECK(std::is_same<time_dimension,
                       dimension_inverse_t<frequency_dimension>>::value);

    CHECK(std::is_same<
          speed_dimension,
          dimension_product_t<length_dimension, frequency_dimension>>::value);

    CHECK(std::is_same<unnamed_dimension<1, 1, 1, 0, 0, 0, 0, 0>,
                       dimension_product_t<length_dimension, time_dimension,
                                           mass_dimension>>::value);

    CHECK(std::is_same<force_dimension,
                       dimension_product_t<
                           mass_dimension, length_dimension,
                           dimension_exponent_t<time_dimension, -2>>>::value);

    CHECK(std::is_same<dimensionless,
                       dimension_exponent_t<time_dimension, 0>>::value);

    CHECK(std::is_same<time_dimension,
                       dimension_exponent_t<time_dimension, 1>>::value);

    CHECK(std::is_same<dimension_product_t<time_dimension, time_dimension>,
                       dimension_exponent_t<time_dimension, 2>>::value);

    CHECK(std::is_same<frequency_dimension,
                       dimension_exponent_t<time_dimension, -1>>::value);

    CHECK(std::is_same<
          dimension_product_t<dimension_exponent_t<time_dimension, -2>,
                              length_dimension>,
          acceleration_dimension>::value);
  }
}

TEST_CASE("Units") {
  using namespace scalr;
  SECTION("Arithmetic") {
    CHECK(std::is_same<make_unit_t<time_dimension, std::ratio<1>>,
                       unit_inverse_t<make_unit_t<frequency_dimension,
                                                  std::ratio<1>>>>::value);

    CHECK(std::is_same<make_unit_t<time_dimension, std::ratio<2, 5>>,
                       unit_inverse_t<make_unit_t<frequency_dimension,
                                                  std::ratio<5, 2>>>>::value);

    CHECK(std::is_same<
          make_unit_t<speed_dimension>,
          unit_product_t<make_unit_t<frequency_dimension, std::kilo>,
                         make_unit_t<length_dimension, std::milli>>>::value);

    CHECK(std::is_same<
          make_unit_t<area_dimension>,
          unit_exponent_t<make_unit_t<length_dimension, std::ratio<1>>,
                          2>>::value);

    CHECK(std::is_same<
          make_unit_t<time_dimension, std::ratio<1, 10>>,
          unit_sum_t<make_unit_t<time_dimension, std::ratio<5, 2>>,
                     make_unit_t<time_dimension, std::ratio<2, 5>>>>::value);
  }
}

TEST_CASE("Quantities") {
  SECTION("Duration") {
    scalr::hours h{1};

    CHECK(scalr::minutes(h).value() == 60);
    CHECK(scalr::seconds(h).value() == 3600);
    CHECK(scalr::milliseconds(h).value() == 3600000);
    CHECK(scalr::microseconds(h).value() == 3600000000);
    CHECK(scalr::nanoseconds(h).value() == 3600000000000);
    CHECK(scalr::picoseconds(h).value() == 3600000000000000);
  }

  SECTION("Le Metre") {
    scalr::kilometers km{3.218688};

    CHECK(scalr::meters(km).value() == Approx(3218.688));
    CHECK(scalr::decimeters(km).value() == Approx(32186.88));
    CHECK(scalr::centimeters(km).value() == Approx(321868.8));
    CHECK(scalr::millimeters(km).value() == 3218688);
    CHECK(scalr::micrometers(km).value() == 3218688000);
    CHECK(scalr::nanometers(km).value() == 3218688000000);
    CHECK(scalr::picometers(km).value() == 3218688000000000);

    CHECK(scalr::miles(km).value() == Approx(2.0));
  }

  SECTION("Imperial Length") {
    scalr::miles mi{2};

    CHECK(scalr::yards(mi).value() == 3520);
    CHECK(scalr::feet(mi).value() == 10560);
    CHECK(scalr::inches(mi).value() == 126720);

    CHECK(scalr::kilometers(mi).value() == Approx(3.218688));
  }

  SECTION("Speeds") {
    using namespace scalr::literals;

    CHECK(72_kph == 20_mps);
    CHECK(72_kph == (20_m / 1000_ms));

    CHECK(scalr::miles_per_hour(72_kph).value() ==
          Approx(scalr::miles_per_hour(44.7387).value()));
  }

  SECTION("Angles") {
    using namespace scalr::literals;
    CHECK(scalr::reduced_radians(3.14159_rad).value() ==
          Approx(scalr::reduced_radians(0.5).value()));

    CHECK(scalr::radians(90_deg).value() ==
          Approx(scalr::radians(1.57079).value()));

    CHECK(45_deg == scalr::reduced_radians(0.125));
    CHECK(scalr::gradians(100) == scalr::reduced_radians(0.25));
  }

  SECTION("Conversion") {
    scalr::seconds secs{12};
    scalr::minutes mins{4};

    scalr::milliseconds d = mins + secs;

    CHECK(d.value() == 252000);
  }

  SECTION("Addition/Subtraction") {
    scalr::microseconds msecs = scalr::microseconds(1200);
    scalr::nanoseconds nsecs = scalr::nanoseconds(1200);

    scalr::nanoseconds sum = msecs + nsecs;
    scalr::nanoseconds diff = msecs - nsecs;

    CHECK(sum == scalr::nanoseconds(1201200));
    CHECK(diff == scalr::nanoseconds(1198800));
  }

  SECTION("Scalar Multiplication/Division/Modulo") {
    using namespace scalr::literals;
    CHECK((12_mm * 1000.0) == (120_m / 10.0));
    CHECK((25.0 / 100_s) == (2.5 / 10_s));
    CHECK((100_s % 17_s) == 15_s);
    CHECK((2_min % 17_s) == 1_s);
    CHECK((2_min / 17_s) == 7);        // 120/17 == 7 (integer division)
    CHECK((119.0_s / 17.0_s) == 7.0);  // 119/17 == 7 (float division)
  }

  SECTION("Dimensional Multiplication/Division") {
    using namespace scalr::literals;
    CHECK((12_mm * 1000.0) == (120_m / 10.0));
  }

  SECTION("Newton Mechanics") {
    scalr::meters dist = scalr::meters(12);
    scalr::seconds dt = scalr::seconds(100);

    scalr::speed<double> avg_speed = dist / dt;

    CHECK(avg_speed == scalr::meters_per_second(0.12));

    scalr::acceleration<double> a = avg_speed / dt;

    CHECK(a == scalr::acceleration<double>(0.0012));

    scalr::newtons f = scalr::kilograms(1000) * a;

    CHECK(f == scalr::newtons(1.2));
  }

  SECTION("Console help") {
    scalr::speed<double, std::ratio<1, 2>> my_speed =
        scalr::length<double>(100) / scalr::duration<intmax_t>(12);
  }

  SECTION("Console help") {
    std::cout << scalr::seconds(100) << std::endl;
    std::cout << scalr::hours(100) << std::endl;
  }
}
