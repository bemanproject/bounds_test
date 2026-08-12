# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

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
endfunction()

add_bounds_test_example(placeholder)
add_bounds_test_example(cartesian_plane)
