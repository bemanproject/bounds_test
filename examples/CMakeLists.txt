# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_EXAMPLES placeholder cartesian_plane)
message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.bounds_test.examples.${example})
    target_sources(beman.bounds_test.examples.${example} PRIVATE ${example}.cpp)
    target_link_libraries(
        beman.bounds_test.examples.${example}
        PRIVATE beman::bounds_test
    )
endforeach()
