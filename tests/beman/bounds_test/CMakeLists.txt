# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(Catch2 3 REQUIRED)

add_executable(beman.bounds_test.tests.bounds_test)
target_sources(beman.bounds_test.tests.bounds_test PRIVATE bounds_test.test.cpp)
target_link_libraries(
    beman.bounds_test.tests.bounds_test
    PRIVATE beman::bounds_test Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(beman.bounds_test.tests.bounds_test)
