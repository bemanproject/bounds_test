// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_BOUNDS_TEST_BOUNDS_TEST_HPP
#define BEMAN_BOUNDS_TEST_BOUNDS_TEST_HPP

#include <concepts>
#include <limits>
#include <utility>

#ifdef __INTELLISENSE__
    #include "plat/generic/beman/bounds_test/plat/plat.hpp"
#else
    #include <beman/bounds_test/plat/plat.hpp>
#endif

namespace beman::bounds_test {

// Useful forward declarations

template <std::integral A, std::integral B>
constexpr bool can_add_in_place(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_subtract_in_place(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_add_in_place_modular(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_subtract_in_place_modular(A a, B b) noexcept;

// End forward declarations

template <std::integral R, std::integral A>
constexpr bool can_convert(A a) noexcept {
    return std::in_range<R>(a);
}

template <std::integral R, std::integral A>
constexpr bool can_convert_modular(A /* a */) noexcept {
    return true;
}

template <std::integral A>
constexpr bool can_increment(A a) noexcept {
    return can_add_in_place(a, 1);
}

template <std::integral A>
constexpr bool can_decrement(A a) noexcept {
    return can_subtract_in_place(a, 1);
}

template <std::integral A>
constexpr bool can_promote(A /* a */) noexcept {
    return true;
}

template <std::integral A>
constexpr bool can_negate(A a) noexcept {
    using result_t = decltype(-a);
    if constexpr (std::unsigned_integral<result_t>)
        return !a;
    return a != std::numeric_limits<result_t>::min();
}

template <std::integral A>
constexpr bool can_bitwise_not(A /* a */) noexcept {
    return true;
}

template <std::integral A>
constexpr bool can_increment_modular(A a) noexcept {
    return can_add_in_place_modular(a, 1);
}

template <std::integral A>
constexpr bool can_decrement_modular(A a) noexcept {
    return can_subtract_in_place_modular(a, 1);
}

template <std::integral A>
constexpr bool can_promote_modular(A /* a */) noexcept {
    return true;
}

template <std::integral A>
constexpr bool can_negate_modular(A a) noexcept {
    using result_t = decltype(-a);
    if constexpr (std::unsigned_integral<result_t>)
        return true;
    return a != std::numeric_limits<result_t>::min();
}

template <std::integral A>
constexpr bool can_bitwise_not_modular(A /* a */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_add(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_add(a, b, decltype(a + b){});
}

template <std::integral A, std::integral B>
constexpr bool can_subtract(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_sub(a, b, decltype(a - b){});
}

template <std::integral A, std::integral B>
constexpr bool can_multiply(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_mul(a, b, decltype(a * b){});
}

template <std::integral A, std::integral B>
constexpr bool can_divide(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_div(a, b, decltype(a / b){});
}

template <std::integral A, std::integral B>
constexpr bool can_take_remainder(A a, B b) noexcept {
    return can_divide(a, b);
}

template <std::integral A, std::integral B>
constexpr bool can_shift_left(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_shift_right(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_bitwise_and(A /* a */, B /* b */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_bitwise_xor(A /* a */, B /* b */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_bitwise_or(A /* a */, B /* b */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_compare(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_add_modular(A a, B b) noexcept {
    if constexpr (std::unsigned_integral<decltype(a + b)>)
        return true;
    return can_add(a, b);
}

template <std::integral A, std::integral B>
constexpr bool can_subtract_modular(A a, B b) noexcept {
    if constexpr (std::unsigned_integral<decltype(a - b)>)
        return true;
    return can_subtract(a, b);
}

template <std::integral A, std::integral B>
constexpr bool can_multiply_modular(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_shift_left_modular(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_shift_right_modular(A, B) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_bitwise_and_modular(A /* a */, B /* b */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_bitwise_xor_modular(A /* a */, B /* b */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_bitwise_or_modular(A /* a */, B /* b */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_add_in_place(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_add(a, b, a);
}

template <std::integral A, std::integral B>
constexpr bool can_subtract_in_place(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_sub(a, b, a);
}

template <std::integral A, std::integral B>
constexpr bool can_multiply_in_place(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_mul(a, b, a);
}

template <std::integral A, std::integral B>
constexpr bool can_divide_in_place(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_div(a, b, a);
}

template <std::integral A, std::integral B>
constexpr bool can_take_remainder_in_place(A a, B b) noexcept {
    return can_divide_in_place(a, b);
}

template <std::integral A, std::integral B>
constexpr bool can_shift_left_in_place(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_shift_right_in_place(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_bitwise_and_in_place(A a, B b) noexcept {
    return std::in_range<A>(a & b);
}

template <std::integral A, std::integral B>
constexpr bool can_bitwise_xor_in_place(A a, B b) noexcept {
    return std::in_range<A>(a ^ b);
}

template <std::integral A, std::integral B>
constexpr bool can_bitwise_or_in_place(A a, B b) noexcept {
    return std::in_range<A>(a | b);
}

template <std::integral A, std::integral B>
constexpr bool can_add_in_place_modular(A a, B b) noexcept {
    if constexpr (std::unsigned_integral<A>)
        return true;
    return can_add_in_place(a, b);
}

template <std::integral A, std::integral B>
constexpr bool can_subtract_in_place_modular(A a, B b) noexcept {
    if constexpr (std::unsigned_integral<A>)
        return true;
    return can_subtract_in_place(a, b);
}

template <std::integral A, std::integral B>
constexpr bool can_multiply_in_place_modular(A a, B b) noexcept;

template <std::integral A, std::integral B>
constexpr bool can_shift_left_in_place_modular(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_shift(a, b, a);
};

template <std::integral A, std::integral B>
constexpr bool can_shift_right_in_place_modular(A a, B b) noexcept {
    return ::beman::bounds_test::detail::can_shift(a, b, a);
};

template <std::integral A, std::integral B>
constexpr bool can_bitwise_and_in_place_modular(A /* a */, B /* b */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_bitwise_xor_in_place_modular(A /* a */, B /* b */) noexcept {
    return true;
}

template <std::integral A, std::integral B>
constexpr bool can_bitwise_or_in_place_modular(A /* a */, B /* b */) noexcept {
    return true;
}

} // namespace beman::bounds_test

#endif // BEMAN_BOUNDS_TEST_BOUNDS_TEST_HPP
