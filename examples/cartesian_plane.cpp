// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <iostream>
#include <optional>
#include <type_traits>

// Alternatively: #include <beman/bounds_test/bounds_test.hpp>
import beman.bounds_test;

namespace bt = beman::bounds_test;

namespace {

template <typename T>
concept SignedNumber = std::is_arithmetic_v<T> && std::is_signed_v<T>;

template <SignedNumber T>
struct Point {
  T x;
  T y;
};

template <SignedNumber T>
constexpr auto try_reflect_x_axis(Point<T>& p) noexcept {
  return bt::can_negate(p.y) ? std::optional<Point<T>>{std::in_place, p.x, static_cast<T>(-p.y)} : std::nullopt;
}

template <SignedNumber T>
constexpr auto try_reflect_y_axis(Point<T>& p) noexcept {
  return bt::can_negate(p.x) ? std::optional<Point<T>>{std::in_place, static_cast<T>(-p.x), p.y} : std::nullopt;
}

} // namespace

int main() {
  auto test_reflect = [&](auto& pt) {
    std::cout << "Original: (" << pt.x << ", " << pt.y << ")\n";
    if (auto r = try_reflect_x_axis(pt)) {
      std::cout << "\tRefleted across X: (" << r->x << ", " << r->y << ")\n";
    } else {
      std::cout << "\tRefelction across X would cause overflow\n";
    }

    if (auto r = try_reflect_y_axis(pt)) {
      std::cout << "\tRefleted across Y: (" << r->x << ", " << r->y << ")\n";
    } else {
      std::cout << "\tRefelction across Y would cause overflow\n";
    }
  };

  Point<int> p{.x = 7, .y = 15};
  test_reflect(p);
  Point<int> q{.x = 12, .y = std::numeric_limits<int>::min()};
  test_reflect(q);
}
