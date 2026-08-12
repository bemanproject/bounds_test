# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
include(${CMAKE_CURRENT_LIST_DIR}/beman.bounds_test-targets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/check_plat.cmake)

check_plat(HAS_GNU_OVERFLOW HAS_MSVC_OVERFLOW)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
if(_IMPORT_PREFIX STREQUAL "/")
    set(_IMPORT_PREFIX "")
endif()

if(HAS_GNU_OVERFLOW)
    set_property(
        TARGET beman::bounds_test
        APPEND
        PROPERTY
            INTERFACE_INCLUDE_DIRECTORIES
                "${_IMPORT_PREFIX}/include/beman/bounds_test/plat/gnu"
    )
    # elseif(HAS_MSVC_OVERFLOW)
    #   set_property(TARGET beman::bounds_test
    #     APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    #       "${_IMPORT_PREFIX}/include/beman/bounds_test/plat/msvc"
    #   )
else()
    set_property(
        TARGET beman::bounds_test
        APPEND
        PROPERTY
            INTERFACE_INCLUDE_DIRECTORIES
                "${_IMPORT_PREFIX}/include/beman/bounds_test/plat/generic"
    )
endif()

set(_IMPORT_PREFIX)
set(HAS_GNU_OVERFLOW)
set(HAS_MSVC_OVERFLOW)

foreach(comp IN LISTS beman.bounds_test_FIND_COMPONENTS)
    if(beman.bounds_test_FIND_REQUIRED_${comp})
        set(beman.bounds_test_FOUND FALSE)
        return()
    endif()
endforeach()
