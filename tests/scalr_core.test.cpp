#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "scalr/scalr.hpp"

using namespace scalr::literals;

TEST_CASE("Dimensions") {
  SECTION("Dimension Arithmetic") {
    STATIC_CHECK(
        std::is_same<scalr::time_dimension,
                     scalr::make_dimension_t<1, 0, 0, 0, 0, 0, 0, 0>>::value);

    STATIC_CHECK(
        std::is_same<scalr::unnamed_dimension<1, 2, 3, 4, 5, 6, 7, 8>,
                     scalr::make_dimension_t<1, 2, 3, 4, 5, 6, 7, 8>>::value);

    STATIC_CHECK(
        std::is_same<scalr::frequency_dimension,
                     scalr::dimension_inverse_t<scalr::time_dimension>>::value);

    STATIC_CHECK(
        std::is_same<
            scalr::time_dimension,
            scalr::dimension_inverse_t<scalr::frequency_dimension>>::value);

    STATIC_CHECK(
        std::is_same<
            scalr::speed_dimension,
            scalr::dimension_product_t<scalr::length_dimension,
                                       scalr::frequency_dimension>>::value);

    STATIC_CHECK(
        std::is_same<scalr::unnamed_dimension<1, 1, 1, 0, 0, 0, 0, 0>,
                     scalr::dimension_product_t<scalr::length_dimension,
                                                scalr::time_dimension,
                                                scalr::mass_dimension>>::value);

    STATIC_CHECK(
        std::is_same<scalr::force_dimension,
                     scalr::dimension_product_t<
                         scalr::mass_dimension, scalr::length_dimension,
                         scalr::dimension_exponent_t<scalr::time_dimension,
                                                     -2>>>::value);

    STATIC_CHECK(std::is_same<
                 scalr::dimensionless,
                 scalr::dimension_exponent_t<scalr::time_dimension, 0>>::value);

    STATIC_CHECK(std::is_same<
                 scalr::time_dimension,
                 scalr::dimension_exponent_t<scalr::time_dimension, 1>>::value);

    STATIC_CHECK(std::is_same<
                 scalr::dimension_product_t<scalr::time_dimension,
                                            scalr::time_dimension>,
                 scalr::dimension_exponent_t<scalr::time_dimension, 2>>::value);

    STATIC_CHECK(
        std::is_same<
            scalr::frequency_dimension,
            scalr::dimension_exponent_t<scalr::time_dimension, -1>>::value);

    STATIC_CHECK(
        std::is_same<scalr::dimension_product_t<
                         scalr::dimension_exponent_t<scalr::time_dimension, -2>,
                         scalr::length_dimension>,
                     scalr::acceleration_dimension>::value);

    STATIC_CHECK(std::is_same<scalr::power_dimension,
                              scalr::dimension_product_t<
                                  scalr::electric_current_dimension,
                                  scalr::electric_potential_dimension>>::value);
  }
}

TEST_CASE("Units") {
  SECTION("Unit Arithmetic") {
    STATIC_CHECK(
        std::is_same<scalr::make_unit_t<scalr::time_dimension, std::ratio<1>>,
                     scalr::unit_inverse_t<scalr::make_unit_t<
                         scalr::frequency_dimension, std::ratio<1>>>>::value);

    STATIC_CHECK(std::is_same<
                 scalr::make_unit_t<scalr::time_dimension, std::ratio<2, 5>>,
                 scalr::unit_inverse_t<scalr::make_unit_t<
                     scalr::frequency_dimension, std::ratio<5, 2>>>>::value);

    STATIC_CHECK(std::is_same<
                 scalr::make_unit_t<scalr::speed_dimension>,
                 scalr::unit_product_t<
                     scalr::make_unit_t<scalr::frequency_dimension, std::kilo>,
                     scalr::make_unit_t<scalr::length_dimension, std::milli>>>::
                     value);

    STATIC_CHECK(std::is_same<
                 scalr::make_unit_t<scalr::area_dimension>,
                 scalr::unit_exponent_t<
                     scalr::make_unit_t<scalr::length_dimension, std::ratio<1>>,
                     2>>::value);

    STATIC_CHECK(
        std::is_same<
            scalr::make_unit_t<scalr::time_dimension, std::ratio<1, 10>>,
            scalr::unit_sum_t<
                scalr::make_unit_t<scalr::time_dimension, std::ratio<5, 2>>,
                scalr::make_unit_t<scalr::time_dimension, std::ratio<2, 5>>>>::
            value);
  }
}

TEST_CASE("Quantities") {
  SECTION("Quantity Common Type") {
    STATIC_CHECK(
        std::is_same<
            scalr::quantity<double, scalr::unit::seconds>,
            std::common_type_t<scalr::quantity<double, scalr::unit::seconds>,
                               scalr::quantity<double, scalr::unit::seconds>>>::
            value);
    STATIC_CHECK(
        std::is_same<scalr::quantity<double, scalr::unit::seconds>,
                     std::common_type_t<
                         scalr::quantity<double, scalr::unit::seconds>,
                         scalr::quantity<int, scalr::unit::seconds>>>::value);
    STATIC_CHECK(
        std::is_same<scalr::quantity<int, scalr::unit::seconds>,
                     std::common_type_t<
                         scalr::quantity<int, scalr::unit::seconds>,
                         scalr::quantity<int, scalr::unit::seconds>>>::value);
    STATIC_CHECK(std::is_same<
                 scalr::quantity<int, scalr::unit::milliseconds>,
                 std::common_type_t<
                     scalr::quantity<int, scalr::unit::seconds>,
                     scalr::quantity<int, scalr::unit::milliseconds>>>::value);
    STATIC_CHECK(
        std::is_same<
            scalr::quantity<int, scalr::time_unit<std::ratio<1, 6>>>,
            std::common_type_t<
                scalr::quantity<int, scalr::time_unit<std::ratio<1, 3>>>,
                scalr::quantity<int, scalr::time_unit<std::ratio<1, 2>>>>>::
            value);
    STATIC_CHECK(
        std::is_same<
            scalr::quantity<double, scalr::time_unit<std::ratio<1, 6>>>,
            std::common_type_t<
                scalr::quantity<int, scalr::time_unit<std::ratio<1, 3>>>,
                scalr::quantity<double, scalr::time_unit<std::ratio<1, 2>>>>>::
            value);
  }
  // SECTION("Comparison") { CHECK(scalr::hours{0.5} < scalr::hours{1}); }
}

TEST_CASE("Named Quantities") {
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
