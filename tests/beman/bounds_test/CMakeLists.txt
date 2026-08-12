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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    return()
endif()

#======================================================================
# NOTE: Only for Release build with enabled install rules useful! CK
#======================================================================
if(BEMAN_BOUNDS_TEST_INSTALL_CONFIG_FILE_PACKAGE AND NOT CMAKE_SKIP_INSTALL_RULES)
    # test if the targets are usable from the install directory
    add_test(
        NAME install-to-stagedir
        COMMAND
            ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix
            ${CMAKE_BINARY_DIR}/stagedir --config $<CONFIG>
    )
    add_test(
        NAME find-package-test
        COMMAND
            ${CMAKE_CTEST_COMMAND} # --verbose
            --output-on-failure -C $<CONFIG> #
            --build-and-test "${CMAKE_SOURCE_DIR}/examples"
            "${CMAKE_CURRENT_BINARY_DIR}/find-package-test" #
            --build-generator ${CMAKE_GENERATOR} #
            --build-makeprogram ${CMAKE_MAKE_PROGRAM} #
            --build-options #
            "-D BEMAN_BUILDSYS_SANITIZER=${BEMAN_BUILDSYS_SANITIZER}"
            "-D CMAKE_BUILD_TYPE=$<CONFIG>"
            "-D CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
            "-D CMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}"
            "-D CMAKE_CXX_MODULE_STD=${CMAKE_CXX_MODULE_STD}"
            "-D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}"
            "-D CMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}"
            "-D CMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/stagedir"
            "-D CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
    )
endif()
