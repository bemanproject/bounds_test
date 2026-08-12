# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
check_plat(HAS_GNU_OVERFLOW HAS_MSVC_OVERFLOW)

if(HAS_GNU_OVERFLOW)
    target_include_directories(
        beman.bounds_test
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gnu>
    )
    # elseif(HAS_MSVC_OVERFLOW)
    #   target_include_directories(
    #     beman.bounds_test
    #     PUBLIC
    #         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msvc>
    #   )
else()
    target_include_directories(
        beman.bounds_test
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generic>
    )
endif()

include(GNUInstallDirs)

install(
    DIRECTORY generic gnu msvc
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/beman/bounds_test/plat
    COMPONENT bounds_test_Development
)
