// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#include <catch2/catch_all.hpp>
#include <limits>
#include <tuple>
#include <type_traits>

#ifdef __INTELLISENSE__
#include <beman/bounds_test/bounds_test.hpp>
#else
import beman.bounds_test;
#endif

namespace bt = beman::bounds_test;

template <typename T>
using nl = std::numeric_limits<T>;

// Macros produce better test names than type lists
#define SIGNED_TYPES   signed char, short, int, long, long long
#define UNSIGNED_TYPES unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
#define ALL_TYPES      SIGNED_TYPES, UNSIGNED_TYPES

template <typename T>
concept promote_range = nl<decltype(+T{})>::max() > nl<T>::max();

template <typename... Ts>
struct promote_helper;

template <typename T1, typename... Ts>
struct promote_helper<T1, Ts...> {
  using promote = decltype(std::tuple_cat(std::conditional_t<promote_range<T1>, std::tuple<T1>, std::tuple<>>{},
                                          typename promote_helper<Ts...>::promote{}));
  using nopromote = decltype(std::tuple_cat(std::conditional_t<promote_range<T1>, std::tuple<>, std::tuple<T1>>{},
                                            typename promote_helper<Ts...>::nopromote{}));
};

template <>
struct promote_helper<> {
  using promote = std::tuple<>;
  using nopromote = std::tuple<>;
};

// Use type lists only for types where we need to check for platform-defined
// range promotion. Note: these check for types where promotion results in an
// increase in value range, not merely a change in type
using signed_promote = promote_helper<SIGNED_TYPES>::promote;
using signed_nopromote = promote_helper<SIGNED_TYPES>::nopromote;
using unsigned_promote = promote_helper<UNSIGNED_TYPES>::promote;
using unsigned_nopromote = promote_helper<UNSIGNED_TYPES>::nopromote;

TEST_CASE("can_convert calls a standard library function", "[bt::can_convert]") {
  STATIC_REQUIRE(bt::can_convert<int>(0));
}

TEST_CASE("can_convert_modular always returns true", "[bt::can_convert_modular]") {
  STATIC_REQUIRE(bt::can_convert_modular<int>(0));
}

TEMPLATE_TEST_CASE("can_increment", "[bt::can_increment]", ALL_TYPES) {
  STATIC_REQUIRE(bt::can_increment(nl<TestType>::min()));
  STATIC_REQUIRE_FALSE(bt::can_increment(nl<TestType>::max()));
}

TEMPLATE_TEST_CASE("can_decrement", "[bt::can_decrement]", ALL_TYPES) {
  STATIC_REQUIRE_FALSE(bt::can_decrement(nl<TestType>::min()));
  STATIC_REQUIRE(bt::can_decrement(nl<TestType>::max()));
}

TEST_CASE("can_promote always returns true", "[bt::can_promote]") {
  STATIC_REQUIRE(bt::can_promote(0));
}

TEMPLATE_LIST_TEST_CASE("can_negate signed types that get promoted to int", "[bt::can_negate]", signed_promote) {
  STATIC_REQUIRE(bt::can_negate(nl<TestType>::min()));
  STATIC_REQUIRE(bt::can_negate(TestType{-1}));
  STATIC_REQUIRE(bt::can_negate(TestType{0}));
  STATIC_REQUIRE(bt::can_negate(TestType{1}));
  STATIC_REQUIRE(bt::can_negate(nl<TestType>::max()));
}

TEMPLATE_LIST_TEST_CASE("can_negate signed types that don't get promoted to int",
                        "[bt::can_negate]",
                        signed_nopromote) {
  STATIC_REQUIRE_FALSE(bt::can_negate(nl<TestType>::min()));
  STATIC_REQUIRE(bt::can_negate(TestType{-1}));
  STATIC_REQUIRE(bt::can_negate(TestType{0}));
  STATIC_REQUIRE(bt::can_negate(TestType{1}));
  STATIC_REQUIRE(bt::can_negate(nl<TestType>::max()));
}

TEMPLATE_LIST_TEST_CASE("can_negate unsigned types that get promoted to int", "[bt::can_negate]", unsigned_promote) {
  STATIC_REQUIRE(bt::can_negate(nl<TestType>::min()));
  STATIC_REQUIRE(bt::can_negate(TestType{0}));
  STATIC_REQUIRE(bt::can_negate(nl<TestType>::max()));
}

TEMPLATE_LIST_TEST_CASE("can_negate unsigned types that don't get promoted to int",
                        "[bt::can_negate]",
                        unsigned_nopromote) {
  STATIC_REQUIRE(bt::can_negate(nl<TestType>::min()));
  STATIC_REQUIRE_FALSE(bt::can_negate(nl<TestType>::max()));
}

TEST_CASE("can_bitwise_not is always true", "[bt::can_bitwise_not]") {
  STATIC_REQUIRE(bt::can_bitwise_not(0));
}

TEMPLATE_TEST_CASE("can_increment_modular unsigned", "[bt::can_increment_modular]", UNSIGNED_TYPES) {
  STATIC_REQUIRE(bt::can_increment_modular(nl<TestType>::min()));
  STATIC_REQUIRE(bt::can_increment_modular(nl<TestType>::max()));
}

TEMPLATE_TEST_CASE("can_increment_modular signed", "[bt::can_increment_modular]", SIGNED_TYPES) {
  STATIC_REQUIRE(bt::can_increment_modular(nl<TestType>::min()));
  STATIC_REQUIRE_FALSE(bt::can_increment_modular(nl<TestType>::max()));
}

TEMPLATE_TEST_CASE("can_decrement_modular unsigned", "[bt::can_decrement_modular]", UNSIGNED_TYPES) {
  STATIC_REQUIRE(bt::can_decrement_modular(nl<TestType>::min()));
  STATIC_REQUIRE(bt::can_decrement_modular(nl<TestType>::max()));
}

TEMPLATE_TEST_CASE("can_decrement_modular signed", "[bt::can_decrement_modular]", SIGNED_TYPES) {
  STATIC_REQUIRE_FALSE(bt::can_decrement_modular(nl<TestType>::min()));
  STATIC_REQUIRE(bt::can_decrement_modular(nl<TestType>::max()));
}

TEST_CASE("can_promote_modular always returns true", "[bt::can_promote_modular]") {
  STATIC_REQUIRE(bt::can_promote_modular(0));
}

TEST_CASE("can_negate_modular for unsigned types always returns true", "[bt::can_negate_modular]") {
  STATIC_REQUIRE(bt::can_negate_modular(0));
}

TEST_CASE("can_bitwise_not_modular is always true", "[bt::can_bitwise_not_modular]") {
  STATIC_REQUIRE(bt::can_bitwise_not_modular(0));
}

TEMPLATE_TEST_CASE("can_add unsigned", "[bt::can_add]", UNSIGNED_TYPES) {
  using result_t = decltype(TestType{} + TestType{});
  constexpr auto lmax = nl<result_t>::max();
  STATIC_REQUIRE(bt::can_add(result_t{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_add(result_t{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_add(result_t{lmax}, TestType{0}));
  STATIC_REQUIRE_FALSE(bt::can_add(result_t{lmax}, TestType{1}));
}

TEMPLATE_TEST_CASE("can_add signed", "[bt::can_add]", SIGNED_TYPES) {
  using result_t = decltype(TestType{} + TestType{});
  constexpr auto lmin = nl<result_t>::min();
  constexpr auto lmax = nl<result_t>::max();
  STATIC_REQUIRE(bt::can_add(result_t{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_add(result_t{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_add(result_t{lmax}, TestType{-1}));
  STATIC_REQUIRE(bt::can_add(result_t{lmin}, TestType{1}));
  STATIC_REQUIRE_FALSE(bt::can_add(result_t{lmax}, TestType{1}));
  STATIC_REQUIRE_FALSE(bt::can_add(result_t{lmin}, TestType{-1}));
}

TEMPLATE_TEST_CASE("can_subtract unsigned", "[bt::can_subtract]", UNSIGNED_TYPES) {
  using result_t = decltype(TestType{} - TestType{});
  constexpr auto lmin = nl<result_t>::min();
  STATIC_REQUIRE(bt::can_subtract(result_t{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_subtract(result_t{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_subtract(result_t{lmin}, TestType{0}));
  STATIC_REQUIRE_FALSE(bt::can_subtract(result_t{lmin}, TestType{1}));
}

TEMPLATE_TEST_CASE("can_subtract signed", "[bt::can_subtract]", SIGNED_TYPES) {
  using result_t = decltype(TestType{} - TestType{});
  constexpr auto lmin = nl<result_t>::min();
  constexpr auto lmax = nl<result_t>::max();
  STATIC_REQUIRE(bt::can_subtract(result_t{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_subtract(result_t{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_subtract(result_t{lmax}, TestType{1}));
  STATIC_REQUIRE(bt::can_subtract(result_t{lmin}, TestType{-1}));
  STATIC_REQUIRE_FALSE(bt::can_subtract(result_t{lmax}, TestType{-1}));
  STATIC_REQUIRE_FALSE(bt::can_subtract(result_t{lmin}, TestType{1}));
}

TEMPLATE_TEST_CASE("can_divide unsigned", "[bt::can_divide]", UNSIGNED_TYPES) {
  STATIC_REQUIRE(bt::can_divide(TestType{0}, TestType{1}));
  STATIC_REQUIRE(bt::can_divide(TestType{1}, TestType{1}));
  STATIC_REQUIRE_FALSE(bt::can_divide(TestType{1}, TestType{0}));
}

TEMPLATE_TEST_CASE("can_divide signed", "[bt::can_divide]", SIGNED_TYPES) {
  using result_t = decltype(TestType{} / TestType{});
  STATIC_REQUIRE(bt::can_divide(result_t{0}, TestType{1}));
  STATIC_REQUIRE(bt::can_divide(result_t{1}, TestType{1}));
  STATIC_REQUIRE_FALSE(bt::can_divide(result_t{1}, TestType{0}));
  STATIC_REQUIRE_FALSE(bt::can_divide(result_t{nl<result_t>::min()}, TestType{-1}));
}

TEMPLATE_TEST_CASE("can_multiply unsigned", "[bt::can_multiply]", UNSIGNED_TYPES) {
  using result_t = decltype(TestType{} * TestType{});
  constexpr auto lmax = nl<result_t>::max();
  STATIC_REQUIRE(bt::can_multiply(result_t{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_multiply(result_t{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_multiply(result_t{lmax}, TestType{0}));
  STATIC_REQUIRE_FALSE(bt::can_multiply(result_t{lmax}, TestType{2}));
}

TEMPLATE_LIST_TEST_CASE("can_multiply signed promote", "[bt::can_multiply]", signed_promote) {
  using result_t = decltype(TestType{} * TestType{});
  STATIC_REQUIRE(bt::can_multiply(TestType{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_multiply(TestType{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_multiply(nl<TestType>::min(), TestType{2}));
  STATIC_REQUIRE(bt::can_multiply(nl<TestType>::max(), TestType{2}));
  STATIC_REQUIRE_FALSE(bt::can_multiply(nl<result_t>::min(), nl<TestType>::min()));
  STATIC_REQUIRE_FALSE(bt::can_multiply(nl<result_t>::max(), nl<TestType>::max()));
}

TEMPLATE_LIST_TEST_CASE("can_multiply signed no promote", "[bt::can_multiply]", signed_nopromote) {
  constexpr auto lmin = nl<TestType>::min();
  constexpr auto lmax = nl<TestType>::max();
  STATIC_REQUIRE(bt::can_multiply(TestType{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_multiply(TestType{1}, TestType{1}));
  STATIC_REQUIRE_FALSE(bt::can_multiply(lmin, TestType{2}));
  STATIC_REQUIRE_FALSE(bt::can_multiply(lmax, TestType{2}));
  STATIC_REQUIRE_FALSE(bt::can_multiply(lmin, TestType{-1}));
}

TEST_CASE("can_take_remainder is can_divide", "[bt::can_take_remainder]") {
  STATIC_REQUIRE_FALSE(bt::can_take_remainder(0, 0));
}

TEST_CASE("can_bitwise_and is always true", "[bt::can_bitwise_and]") {
  STATIC_REQUIRE(bt::can_bitwise_and(0, 0));
}

TEST_CASE("can_bitwise_xor is always true", "[bt::can_bitwise_xor]") {
  STATIC_REQUIRE(bt::can_bitwise_xor(0, 0));
}

TEST_CASE("can_bitwise_or is always true", "[bt::can_bitwise_or]") {
  STATIC_REQUIRE(bt::can_bitwise_or(0, 0));
}

TEMPLATE_TEST_CASE("can_add_in_place unsigned", "[bt::can_add_in_place]", UNSIGNED_TYPES) {
  constexpr auto lmax = nl<TestType>::max();
  STATIC_REQUIRE(bt::can_add_in_place(TestType{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_add_in_place(TestType{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_add_in_place(TestType{lmax}, TestType{0}));
  STATIC_REQUIRE_FALSE(bt::can_add_in_place(TestType{lmax}, TestType{1}));
}

TEMPLATE_TEST_CASE("can_add_in_place signed", "[bt::can_add_in_place]", SIGNED_TYPES) {
  constexpr auto lmin = nl<TestType>::min();
  constexpr auto lmax = nl<TestType>::max();
  STATIC_REQUIRE(bt::can_add_in_place(TestType{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_add_in_place(TestType{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_add_in_place(TestType{lmax}, TestType{-1}));
  STATIC_REQUIRE(bt::can_add_in_place(TestType{lmin}, TestType{1}));
  STATIC_REQUIRE_FALSE(bt::can_add_in_place(TestType{lmax}, TestType{1}));
  STATIC_REQUIRE_FALSE(bt::can_add_in_place(TestType{lmin}, TestType{-1}));
}

TEMPLATE_TEST_CASE("can_subtract_in_place unsigned", "[bt::can_subtract_in_place]", UNSIGNED_TYPES) {
  constexpr auto lmin = nl<TestType>::min();
  STATIC_REQUIRE(bt::can_subtract_in_place(TestType{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_subtract_in_place(TestType{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_subtract_in_place(TestType{lmin}, TestType{0}));
  STATIC_REQUIRE_FALSE(bt::can_subtract_in_place(TestType{lmin}, TestType{1}));
}

TEMPLATE_TEST_CASE("can_subtract_in_place signed", "[bt::can_subtract_in_place]", SIGNED_TYPES) {
  constexpr auto lmin = nl<TestType>::min();
  constexpr auto lmax = nl<TestType>::max();
  STATIC_REQUIRE(bt::can_subtract_in_place(TestType{0}, TestType{0}));
  STATIC_REQUIRE(bt::can_subtract_in_place(TestType{1}, TestType{1}));
  STATIC_REQUIRE(bt::can_subtract_in_place(TestType{lmax}, TestType{1}));
  STATIC_REQUIRE(bt::can_subtract_in_place(TestType{lmin}, TestType{-1}));
  STATIC_REQUIRE_FALSE(bt::can_subtract_in_place(TestType{lmax}, TestType{-1}));
  STATIC_REQUIRE_FALSE(bt::can_subtract_in_place(TestType{lmin}, TestType{1}));
}

TEMPLATE_TEST_CASE_SIG("can_bitwise_and_in_place",
                       "[bt::can_bitwise_and_in_place]",
                       ((typename A, typename B, int ID), A, B, ID),
                       (signed char, short, 1),
                       (signed char, long long, 1)) {
  constexpr auto amin = std::numeric_limits<A>::lowest();
  constexpr auto amax = std::numeric_limits<A>::max();

  STATIC_REQUIRE(bt::can_bitwise_and_in_place<A, B>(A{5}, B{3}));
  STATIC_REQUIRE(bt::can_bitwise_and_in_place<A, B>(A{-1}, B{1}));
  STATIC_REQUIRE(bt::can_bitwise_and_in_place<A, B>(A{amax}, B{amax}));

  STATIC_REQUIRE_FALSE(bt::can_bitwise_and_in_place<A, B>(A{-1}, B{256}));
  STATIC_REQUIRE_FALSE(bt::can_bitwise_and_in_place<A, B>(A{amin}, B{256}));
}

TEMPLATE_TEST_CASE_SIG("can_bitwise_xor_in_place<int8_t,int16_t>",
                       "[bt::can_bitwise_xor_in_place]",
                       ((typename A, typename B, int ID), A, B, ID),
                       (signed char, short, 1),
                       (signed char, long long, 1)) {
  constexpr auto amax = std::numeric_limits<A>::max();

  STATIC_REQUIRE(bt::can_bitwise_xor_in_place<A, B>(A{-1}, B{1}));
  STATIC_REQUIRE(bt::can_bitwise_xor_in_place<A, B>(A{-128}, B{1}));
  STATIC_REQUIRE(bt::can_bitwise_xor_in_place<A, B>(A{5}, B{3}));

  STATIC_REQUIRE_FALSE(bt::can_bitwise_xor_in_place<A, B>(A{-1}, B{256}));
  STATIC_REQUIRE_FALSE(bt::can_bitwise_xor_in_place<A, B>(A{amax}, B{512}));
}

TEMPLATE_TEST_CASE_SIG("can_bitwise_or_in_place<int8_t,int16_t>",
                       "[bt::can_bitwise_or_in_place]",
                       ((typename A, typename B, int ID), A, B, ID),
                       (signed char, short, 1),
                       (signed char, long long, 1)) {
  STATIC_REQUIRE(bt::can_bitwise_or_in_place<A, B>(A{10}, B{5}));
  STATIC_REQUIRE(bt::can_bitwise_or_in_place<A, B>(A{-128}, B{1}));
  STATIC_REQUIRE(bt::can_bitwise_or_in_place<A, B>(A{1}, B{126}));

  STATIC_REQUIRE_FALSE(bt::can_bitwise_or_in_place<A, B>(A{64}, B{192}));
  STATIC_REQUIRE_FALSE(bt::can_bitwise_or_in_place<A, B>(A{0}, B{256}));
}

TEST_CASE("can_take_remainder_in_place is can_divide_in_place", "[bt::can_take_remainder_in_place]") {
  STATIC_REQUIRE_FALSE(bt::can_take_remainder_in_place<int>(0, 0));
}

TEMPLATE_TEST_CASE("can_shift_left_in_place_modular signed", "[bt::can_shift_left_in_place_modular]", SIGNED_TYPES) {
  constexpr auto digits = nl<TestType>::digits;
  STATIC_REQUIRE(bt::can_shift_left_in_place_modular(TestType{0}, 0));
  STATIC_REQUIRE(bt::can_shift_left_in_place_modular(TestType{1}, 1));
  STATIC_REQUIRE(bt::can_shift_left_in_place_modular(TestType{1}, digits));
  STATIC_REQUIRE_FALSE(bt::can_shift_left_in_place_modular(TestType{1}, -1));
  STATIC_REQUIRE_FALSE(bt::can_shift_left_in_place_modular(TestType{1}, digits + 1));
}

TEMPLATE_TEST_CASE("can_shift_left_in_place_modular unsigned",
                   "[bt::can_shift_left_in_place_modular]",
                   UNSIGNED_TYPES) {
  constexpr auto digits = nl<TestType>::digits;
  STATIC_REQUIRE(bt::can_shift_left_in_place_modular(TestType{0}, 0));
  STATIC_REQUIRE(bt::can_shift_left_in_place_modular(TestType{1}, 1));
  STATIC_REQUIRE(bt::can_shift_left_in_place_modular(TestType{1}, digits - 1));
  STATIC_REQUIRE_FALSE(bt::can_shift_left_in_place_modular(TestType{1}, -1));
  STATIC_REQUIRE_FALSE(bt::can_shift_left_in_place_modular(TestType{1}, digits));
}

TEMPLATE_TEST_CASE("can_shift_right_in_place_modular signed", "[bt::can_shift_right_in_place_modular]", SIGNED_TYPES) {
  constexpr auto digits = nl<TestType>::digits;
  STATIC_REQUIRE(bt::can_shift_right_in_place_modular(TestType{0}, 0));
  STATIC_REQUIRE(bt::can_shift_right_in_place_modular(TestType{1}, 1));
  STATIC_REQUIRE(bt::can_shift_right_in_place_modular(TestType{1}, digits));
  STATIC_REQUIRE_FALSE(bt::can_shift_right_in_place_modular(TestType{1}, -1));
  STATIC_REQUIRE_FALSE(bt::can_shift_right_in_place_modular(TestType{1}, digits + 1));
}

TEMPLATE_TEST_CASE("can_shift_right_in_place_modular unsigned",
                   "[bt::can_shift_right_in_place_modular]",
                   UNSIGNED_TYPES) {
  constexpr auto digits = nl<TestType>::digits;
  STATIC_REQUIRE(bt::can_shift_right_in_place_modular(TestType{0}, 0));
  STATIC_REQUIRE(bt::can_shift_right_in_place_modular(TestType{1}, 1));
  STATIC_REQUIRE(bt::can_shift_right_in_place_modular(TestType{1}, digits - 1));
  STATIC_REQUIRE_FALSE(bt::can_shift_right_in_place_modular(TestType{1}, -1));
  STATIC_REQUIRE_FALSE(bt::can_shift_right_in_place_modular(TestType{1}, digits));
}

TEST_CASE("can_bitwise_and_in_place_modular is always true", "[bt::can_bitwise_and_in_place_modular]") {
  STATIC_REQUIRE(bt::can_bitwise_and_in_place_modular(0, 0));
}

TEST_CASE("can_bitwise_xor_in_place_modular is always true", "[bt::can_bitwise_xor_in_place_modular]") {
  STATIC_REQUIRE(bt::can_bitwise_xor_in_place_modular(0, 0));
}

TEST_CASE("can_bitwise_or_in_place_modular is always true", "[bt::can_bitwise_or_in_place_modular]") {
  STATIC_REQUIRE(bt::can_bitwise_or_in_place_modular(0, 0));
}
