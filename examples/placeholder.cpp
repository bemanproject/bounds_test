// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifdef __INTELLISENSE__
#include <beman/bounds_test/bounds_test.hpp>
#else
import beman.bounds_test;
#endif

namespace bt = beman::bounds_test;

int main(int argc, char** /* argv */) {
  return bt::can_convert_modular<unsigned>(argc);
}
