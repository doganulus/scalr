find_package(Catch2 3 REQUIRED)
include(Catch)

add_executable(
  scalr_tests
    scalr_core.test.cpp
)

target_compile_features(scalr_tests INTERFACE cxx_std_14)

target_link_libraries(
  scalr_tests
    PUBLIC
    Catch2::Catch2WithMain
    scalr
)

catch_discover_tests(scalr_tests)
