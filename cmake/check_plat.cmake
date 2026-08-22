# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
function(check_plat HAS_GNU_VAR HAS_MSVC_VAR)
    include(CheckSourceCompiles)
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

    check_source_compiles(
        CXX
        "void f() {
    int a, b, c;
    __builtin_add_overflow(a, b, &c);
    __builtin_sub_overflow(a, b, &c);
    __builtin_mul_overflow(a, b, &c);
  }"
        HAS_GNU_OVERFLOW
    )

    check_source_compiles(
        CXX
        "
  #include <intrin.h>
  #include <cstdint>
  void f() {
    std::int32_t a, b, c;
    _add_overflow_i32(0, a, b, &c);
    _sub_overflow_i32(0, a, b, &c);
  }"
        HAS_MSVC_OVERFLOW
    )

    set(${HAS_GNU_VAR} ${HAS_GNU_OVERFLOW} PARENT_SCOPE)
    set(${HAS_MSVC_VAR} ${HAS_MSVC_OVERFLOW} PARENT_SCOPE)
endfunction()
