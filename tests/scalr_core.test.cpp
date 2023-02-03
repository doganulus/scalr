#include "scalr/scalr.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using namespace scalr;
using namespace scalr::literals;

TEST_CASE("Dimensions") {
  SECTION("Dimension Arithmetic") {
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

    CHECK(
        std::is_same<power_dimension,
                     dimension_product_t<electric_current_dimension,
                                         electric_potential_dimension>>::value);
  }
}


TEST_CASE("Units") {
  SECTION("Unit Arithmetic") {
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

  SECTION("Length") {
    scalr::kilometers km{3.218688};

    CHECK(scalr::meters(km).value() == Catch::Approx(3218.688));
    CHECK(scalr::decimeters(km).value() == Catch::Approx(32186.88));
    CHECK(scalr::centimeters(km).value() == Catch::Approx(321868.8));
    CHECK(scalr::millimeters(km).value() == 3218688);
    CHECK(scalr::micrometers(km).value() == 3218688000);
    CHECK(scalr::nanometers(km).value() == 3218688000000);
    CHECK(scalr::picometers(km).value() == 3218688000000000);

    CHECK(scalr::miles(km).value() == Catch::Approx(2.0));

    SECTION("Literals") {
      CHECK(42_cm == scalr::centimeters{42});
      CHECK(42.42_cm == scalr::centimeters{42.42});
    }
  }

  SECTION("Imperial Length") {
    scalr::miles mi{2};

    CHECK(scalr::yards(mi).value() == 3520);
    CHECK(scalr::feet(mi).value() == 10560);
    CHECK(scalr::inches(mi).value() == 126720);

    CHECK(scalr::kilometers(mi).value() == Catch::Approx(3.218688));
  }

  SECTION("Speeds") {
    CHECK(72_kph == 20_mps);
    CHECK(72_kph == (20_m / 1000_ms));

    CHECK(scalr::miles_per_hour(72_kph).value() ==
          Catch::Approx(scalr::miles_per_hour(44.7387).value()));
  }

  SECTION("Angles") {
    CHECK(scalr::reduced_radians(3.14159_rad).value() ==
          Catch::Approx(scalr::reduced_radians(0.5).value()));

    CHECK(scalr::radians(90_deg).value() ==
          Catch::Approx(scalr::radians(1.57079).value()));

    CHECK(45_deg == scalr::reduced_radians(0.125));
    CHECK(scalr::gradians(100) == scalr::reduced_radians(0.25));
  }

  SECTION("Electric Current") {

    CHECK(4.2_mA == 4200_uA);
    CHECK(42_mA == 0.042_A);
    CHECK(4000_mA == 4_A);
  }

  SECTION("Electric Potential") {

    CHECK(4.2_mV == 4200_uV);
    CHECK(42_mV == 0.042_V);
    CHECK(4000_mV == 4_V);
  }

  SECTION("Power") {

    CHECK(1_A * 1_V == 1_W);
    CHECK(100_mA * 12_V == 1.2_W);

    CHECK(1_mps * 1_N == 1_W);
    CHECK(80_kg * 9.81_mps2 * 3_m / 5_s == 470.88_W);
  }

  SECTION("Quantity Conversion") {
    scalr::seconds secs{12};
    scalr::minutes mins{4};

    scalr::milliseconds d = mins + secs;

    CHECK(d.value() == 252000);
  }

  SECTION("Quantity Arithmetic") {
    scalr::microseconds msecs = scalr::microseconds(1200);
    scalr::nanoseconds nsecs = scalr::nanoseconds(1200);

    scalr::nanoseconds sum = msecs + nsecs;
    scalr::nanoseconds diff = msecs - nsecs;

    CHECK(sum == scalr::nanoseconds(1201200));
    CHECK(diff == scalr::nanoseconds(1198800));
  }

  SECTION("Example Newton Mechanics") {
    scalr::meters dist = scalr::meters(12);
    scalr::seconds dt = scalr::seconds(100);

    scalr::speed<double> avg_speed = dist / dt;

    CHECK(avg_speed == scalr::meters_per_second(0.12));

    scalr::acceleration<double> a = avg_speed / dt;

    CHECK(a == scalr::acceleration<double>(0.0012));

    scalr::newtons f = scalr::kilograms(1000) * a;

    CHECK(f == scalr::newtons(1.2));
  }

}