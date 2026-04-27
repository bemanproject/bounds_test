# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.30...4.3)

project(
    beman.bounds_test
    DESCRIPTION "A library for checking integer operation boundary conditions"
    LANGUAGES CXX
    VERSION 0.1.0
)

# [CMAKE.SKIP_TESTS]
option(
    BEMAN_BOUNDS_TEST_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# [CMAKE.SKIP_EXAMPLES]
option(
    BEMAN_BOUNDS_TEST_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# for find of beman_install_library and configure_build_telemetry
include(infra/cmake/beman-install-library.cmake)
include(infra/cmake/BuildTelemetryConfig.cmake)

add_library(beman.bounds_test)
add_library(beman::bounds_test ALIAS beman.bounds_test)

target_sources(
    beman.bounds_test
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include
            FILES
                include/beman/bounds_test/bounds_test.hpp
                include/beman/bounds_test/plat/common.hpp
    PUBLIC
        FILE_SET CXX_MODULES
            BASE_DIRS include
            FILES include/beman/bounds_test/beman.bounds_test.cppm
)

set_target_properties(
    beman.bounds_test
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ${PROJECT_IS_TOP_LEVEL}
)

include(cmake/check_plat.cmake)

add_subdirectory(include/beman/bounds_test/plat)

beman_install_library(beman.bounds_test TARGETS beman.bounds_test)
configure_build_telemetry()

include(GNUInstallDirs)

install(
    FILES cmake/check_plat.cmake cmake/beman.bounds_test-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/beman.bounds_test
)

install(
    DIRECTORY
        include/beman/bounds_test/plat/generic
        include/beman/bounds_test/plat/gnu
        include/beman/bounds_test/plat/msvc
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/beman/bounds_test/plat
)

if(BEMAN_BOUNDS_TEST_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/beman/bounds_test)
endif()

if(BEMAN_BOUNDS_TEST_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
