# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.28...4.2)

# XXX include(cmake/bootstrap_vcpkg.cmake)

project(
    beman.bounds_test
    DESCRIPTION "A library for checking integer operation boundary conditions"
    LANGUAGES CXX
    VERSION 0.0.1
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

option(
    BEMAN_BOUNDS_TEST_INSTALL_CONFIG_FILE_PACKAGE
    "Enable creating and installing a CMake config-file package. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# Modules opt in only on compilers that support g++-15 and clang-20+
set(CMAKE_CXX_SCAN_FOR_MODULES 0)
if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 20)
        set(CMAKE_CXX_SCAN_FOR_MODULES 1)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 15)
        set(CMAKE_CXX_SCAN_FOR_MODULES 1)
    endif()
endif()

if(CMAKE_CXX_SCAN_FOR_MODULES)
    add_library(beman.bounds_test)
    set(BEMAN_BOUNDS_TEST_INTERFACE PUBLIC)
else()
    add_library(beman.bounds_test INTERFACE)
    set(BEMAN_BOUNDS_TEST_INTERFACE INTERFACE)
endif()

add_library(beman::bounds_test ALIAS beman.bounds_test)

set_target_properties(
    beman.bounds_test
    PROPERTIES
        VERIFY_INTERFACE_HEADER_SETS ON
)

target_sources(
    beman.bounds_test

    ${BEMAN_BOUNDS_TEST_INTERFACE}
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/beman/bounds_test/bounds_test.hpp
            include/beman/bounds_test/plat/common.hpp
)

if(CMAKE_CXX_SCAN_FOR_MODULES)
    set_target_properties(
        beman.bounds_test
        PROPERTIES
            EXPORT_NAME bounds_test
    )
    target_compile_definitions(beman.bounds_test PUBLIC HAS_CXX_MODULES)
    target_sources(
        beman.bounds_test

        PUBLIC
            FILE_SET CXX_MODULES
            BASE_DIRS include
            FILES
                include/beman/bounds_test/beman.bounds_test.cppm
    )
endif()

include(GNUInstallDirs)
include(cmake/check_plat.cmake)

add_subdirectory(include/beman/bounds_test/plat)

install(
    TARGETS beman.bounds_test
    EXPORT beman.bounds_test-targets
    COMPONENT beman.bounds_test

    FILE_SET CXX_MODULES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}

    FILE_SET HEADERS
)

if(BEMAN_BOUNDS_TEST_INSTALL_CONFIG_FILE_PACKAGE)
    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/beman.bounds_test-config-version.cmake
        COMPATIBILITY ExactVersion
    )

    install(
        FILES
            cmake/beman.bounds_test-config.cmake
            cmake/check_plat.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/beman.bounds_test-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/beman.bounds_test
        COMPONENT beman.bounds_test
    )

    install(
        EXPORT beman.bounds_test-targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/beman.bounds_test
        NAMESPACE beman::
        CXX_MODULES_DIRECTORY cxx-modules
        COMPONENT beman.bounds_test
    )
endif()

if(BEMAN_BOUNDS_TEST_BUILD_TESTS)
    include(CTestUseLaunchers)
    enable_testing()
    add_subdirectory(tests/beman/bounds_test)
endif()

if(BEMAN_BOUNDS_TEST_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
