# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(Catch2 3 REQUIRED CONFIG)

add_executable(beman.bounds_test.tests)
target_sources(beman.bounds_test.tests PRIVATE bounds_test.test.cpp)
target_compile_features(beman.bounds_test.tests PRIVATE cxx_std_20)
target_link_libraries(
    beman.bounds_test.tests
    PRIVATE beman::bounds_test Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(beman.bounds_test.tests)
