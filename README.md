# beman.bounds_test: A library for checking integer operation boundary conditions

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable-next-line line-length -->
![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg) ![Continuous Integration Tests](https://github.com/bemanproject/bounds_test/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/bemanproject/bounds_test/actions/workflows/pre-commit-check.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/bemanproject/bounds_test/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/bounds_test?branch=main) ![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)

`beman.bounds_test` is a C++ library providing overflow and undefined behavior
checking for integer operations. The library conforms to [The Beman Standard](https://github.com/bemanproject/beman/blob/main/docs/beman_standard.md).

**Implements**: [P1619 Functions for Testing Boundary Conditions on Integer Operations](https://wg21.link/P1619)
targeted at C++29.

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#under-development-and-not-yet-ready-for-production-use)

## License

`beman.bounds_test` is licensed under the Apache License v2.0 with LLVM Exceptions.

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

Full runnable examples can be found in [`examples/`](examples/).

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

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++20 standard or greater
* CMake 3.30 or later
* (Test Only) Catch2

You can disable building tests by setting CMake option `BEMAN_BOUNDS_TEST_BUILD_TESTS` to
`OFF` when configuring the project.

### Supported Platforms

| Compiler | Version | C++ Standards | Standard Library  |
|----------|---------|---------------|-------------------|
| GCC      | 15-13   | C++26-C++20   | libstdc++         |
| GCC      | 12-11   | C++23, C++20  | libstdc++         |
| Clang    | 22-19   | C++26-C++20   | libstdc++, libc++ |
| Clang    | 18      | C++26-C++20   | libc++            |
| Clang    | 18      | C++23, C++20  | libstdc++         |
| Clang    | 17      | C++26-C++20   | libc++            |
| Clang    | 17      | C++20         | libstdc++         |
| MSVC     | latest  | C++23         | MSVC STL          |

## Development

See the [Contributing Guidelines](CONTRIBUTING.md).

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

### Build

You can build bounds_test using a CMake workflow preset:

```bash
cmake --workflow --preset gcc-release
```

To list available workflow presets, you can invoke:

```bash
cmake --list-presets=workflow
```

For details on building beman.bounds_test without using a CMake preset, refer to the
[Contributing Guidelines](CONTRIBUTING.md).

### Installation

To install beman.bounds_test globally after building with the `gcc-release` preset, you can
run:

```bash
sudo cmake --install build/gcc-release
```

Alternatively, to install to a prefix, for example `/opt/beman`, you can run:

```bash
sudo cmake --install build/gcc-release --prefix /opt/beman
```

This will generate the following directory structure:

```txt
/opt/beman
├── include
│   └── beman
│       └── bounds_test
│           ├── bounds_test.hpp
│           └── ...
└── lib
    ├── cmake
    │   └── beman.bounds_test
    │       ├── beman.bounds_test-config-version.cmake
    │       ├── beman.bounds_test-config.cmake
    │       ├── beman.bounds_test-targets-debug.cmake
    │       └── beman.bounds_test-targets.cmake
    └── libbeman.bounds_test.a
```

### CMake Configuration

If you installed beman.bounds_test to a prefix, you can specify that prefix to your CMake
project using `CMAKE_PREFIX_PATH`; for example, `-DCMAKE_PREFIX_PATH=/opt/beman`.

You need to bring in the `beman.bounds_test` package to define the `beman::bounds_test` CMake
target:

```cmake
find_package(beman.bounds_test REQUIRED)
```

You will then need to add `beman::bounds_test` to the link libraries of any libraries or
executables that include `beman.bounds_test` headers.

```cmake
target_link_libraries(yourlib PUBLIC beman::bounds_test)
```

### Using beman.bounds_test

To use `beman.bounds_test` in your C++ project,
include an appropriate `beman.bounds_test` header from your source code.

```c++
#include <beman/bounds_test/bounds_test.hpp>
```

> [!NOTE]
>
> `beman.bounds_test` headers are to be included with the `beman/bounds_test/` prefix.
> Altering include search paths to spell the include target another way (e.g.
> `#include <bounds_test.hpp>`) is unsupported.

## Contributing

Please do! Issues and pull requests are appreciated.
