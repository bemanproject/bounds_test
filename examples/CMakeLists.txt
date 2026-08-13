# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.28...4.4)

project(beman_bounds_test.example LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    find_package(beman.bounds_test 0.1.0 EXACT REQUIRED)

    enable_testing()
endif()

function(add_bounds_test_example NAME)
    add_executable(beman.bounds_test.examples.${NAME})
    target_sources(beman.bounds_test.examples.${NAME} PRIVATE ${NAME}.cpp)
    target_compile_features(
        beman.bounds_test.examples.${NAME}
        PRIVATE cxx_std_20
    )
    target_link_libraries(
        beman.bounds_test.examples.${NAME}
        PRIVATE beman::bounds_test
    )
    add_test(
        NAME beman.bounds_test.examples.${NAME}
        COMMAND beman.bounds_test.examples.${NAME}
    )
endfunction()

# FIXME: add_bounds_test_example(placeholder)
add_bounds_test_example(cartesian_plane)
