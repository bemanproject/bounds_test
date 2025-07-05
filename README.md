# beman.bounds_test: A library for checking integer operation boundary conditions

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable-next-line line-length -->
![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg) ![Continuous Integration Tests](https://github.com/rishyak/rish.bounds_test/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/rishyak/rish.bounds_test/actions/workflows/pre-commit.yml/badge.svg) ![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)

`beman.bounds_test` is a C++ library providing overflow and undefined behavior
checking for integer operations. The library conforms to [The Beman Standard](https://github.com/bemanproject/beman/blob/main/docs/BEMAN_STANDARD.md).

**Implements**: [P1619 Functions for Testing Boundary Conditions on Integer Operations](https://wg21.link/P1619)
targeted at C++29.

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/BEMAN_LIBRARY_MATURITY_MODEL.md#under-development-and-not-yet-ready-for-production-use)

## Overview

The integer operations in C++ have boundary conditions that may readily be
encountered by novices. Unfortunately for those novices, expressing these
conditions in the language requires detailed knowledge of the language, a degree
of mathematical subtlety, and considerable care.

`beman.bounds_test` provides functions that name and express these conditions
simply and directly, in a form conducive to use in assertions.

## Usage

The following is an example of using `can_negate` to verify that a unary `-`
operator will produce the expected result in the promotion type used to evaluate
the expression.

```cpp
import std;
import beman.bounds_test;

signed char small = std::numeric_limits<signed char>::min();
int big = std::numeric_limits<int>::min();

// Pass (assuming sizeof signed char < sizeof int)
static_assert(beman::bounds_test::can_negate(small));

// Fail, need to cast to larger type prior to negation
static_assert(beman::bounds_test::can_negate(big));
```

## Integrate beman.bounds_test into your project

`beman.bounds_test` is available as both a header and a module. It requires
a minimum language standard of C++20.

```cpp
// As a module
import beman.bounds_test

// As a header
#include <beman/bounds_test/bounds_test.hpp>
```

`beman.bounds_test` relies on CMake-based platform introspection to determine
the correct set of implementation-specific headers to use. As such, it is best
consumed as a CMake dependency.

```cmake
find_package(beman.bounds_test)
target_link_libraries(<target> PRIVATE beman::bounds_test)
```

## Building beman.bounds_test

`beman.bounds_test` has no dependencies when being built without tests, so is
easily built via typical CMake workflows. The CMakePresets provide some
examples, but a trivial build might be performed as follows:

```plaintext
cmake -B build -G Ninja \
  -DCMAKE_CXX_STANDARD=20 \
  -DBEMAN_BOUNDS_TEST_BUILD_TESTS=OFF \
  -DBEMAN_BOUNDS_TEST_BUILD_EXAMPLES=OFF

cmake --build build
cmake --install build --prefix <destination>
```

When building tests `beman.bounds_test` relies on [Catch2](https://github.com/catchorg/Catch2)
to provide testing infrastructure. This can be provided as part of the build
environment, or can be provided by vcpkg as part of the build. In order to
bootstrap vcpkg, use the `-DBEMAN_BOUNDS_TEST_BOOTSTRAP_VCPKG=ON` option.

## Implementation Details

All provided checks are fully `constexpr`, and so have zero runtime cost where
they can be evaluated at compile-time. For the runtime case, wherever possible
`beman.bounds_test` delegates to compiler builtins for bounds checking.

This has trivial cost on most platforms, for example `can_add()` typically
resolves to a single [`setno`](https://www.felixcloutier.com/x86/setcc)
instruction on x86, or is optimized out entirely in favor of a conditional jump.
However, where compiler builtins are not available generic range-checking is
used instead. This optimizes less well than the builtins.

The builtin checks used by `beman.bounds_test` can be found in
`cmake/check_plat.cmake`.

## License

Source is licensed with the Apache 2.0 license with LLVM exceptions

// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

Documentation and associated papers are licensed with the Creative Commons
Attribution 4.0 International license.

// SPDX-License-Identifier: CC-BY-4.0

The intent is that the source and documentation are available for use by people
how they wish.

The README itself is licensed with CC0 1.0 Universal. Copy the contents and
incorporate in your own work as you see fit.

// SPDX-License-Identifier: CC0-1.0

## Contributing

Please do! Issues and pull requests are appreciated.
