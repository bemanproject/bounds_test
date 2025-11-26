# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.11.0
    EXCLUDE_FROM_ALL
    FIND_PACKAGE_ARGS 3
)
FetchContent_MakeAvailable(Catch2)

add_executable(beman.bounds_test.tests)
target_sources(beman.bounds_test.tests PRIVATE bounds_test.tests.cpp)
target_compile_features(beman.bounds_test.tests PRIVATE cxx_std_20)
target_link_libraries(
    beman.bounds_test.tests
    PRIVATE beman::bounds_test Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(beman.bounds_test.tests)
