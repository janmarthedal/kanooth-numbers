/*
 * File:   test/type_check.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-07-04 16:50Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>

#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/natural_number.hpp>
#include <kanooth/numbers/integer.hpp>
#include <kanooth/numbers/positive_float.hpp>

using kanooth::is_signed;
using kanooth::is_integral;
using kanooth::make_signed;
using kanooth::make_unsigned;
using kanooth::is_native_int;
using kanooth::is_native_float;
using kanooth::is_native_number;
using kanooth::numbers::natural_number;
using kanooth::numbers::integer;
using kanooth::numbers::positive_float;

template <bool> struct static_assert;
template <> struct static_assert<true> {};

template <typename, typename> struct static_assert_type;
template <typename T> struct static_assert_type<T, T> {};

void test_is_signed()
{
  static_assert<is_signed<short>::value>();
  static_assert<!is_signed<unsigned short>::value>();
  static_assert<is_signed<int>::value>();
  static_assert<!is_signed<unsigned int>::value>();
  static_assert<is_signed<long>::value>();
  static_assert<!is_signed<unsigned long>::value>();
#ifdef KANOOTH_HAS_LONG_LONG
  static_assert<is_signed<long long>::value>();
  static_assert<!is_signed<unsigned long long>::value>();
#endif
  static_assert<is_signed<float>::value>();
  static_assert<is_signed<double>::value>();
  static_assert<is_signed<long double>::value>();
  static_assert<!is_signed<natural_number>::value>();
  static_assert<is_signed<integer>::value>();
  static_assert<!is_signed<positive_float>::value>();
}

void test_is_integral()
{
  static_assert<is_integral<short>::value>();
  static_assert<is_integral<unsigned short>::value>();
  static_assert<is_integral<int>::value>();
  static_assert<is_integral<unsigned int>::value>();
  static_assert<is_integral<long>::value>();
  static_assert<is_integral<unsigned long>::value>();
#ifdef KANOOTH_HAS_LONG_LONG
  static_assert<is_integral<long long>::value>();
  static_assert<is_integral<unsigned long long>::value>();
#endif
  static_assert<!is_integral<float>::value>();
  static_assert<!is_integral<double>::value>();
  static_assert<!is_integral<long double>::value>();
  static_assert<is_integral<natural_number>::value>();
  static_assert<is_integral<integer>::value>();
  static_assert<!is_integral<positive_float>::value>();
}

void test_make_signed()
{
  static_assert_type<make_signed<short>::type,              short>();
  static_assert_type<make_signed<unsigned short>::type,     short>();
  static_assert_type<make_signed<int>::type,                int>();
  static_assert_type<make_signed<unsigned int>::type,       int>();
  static_assert_type<make_signed<long>::type,               long>();
  static_assert_type<make_signed<unsigned long>::type,      long>();
#ifdef KANOOTH_HAS_LONG_LONG
  static_assert_type<make_signed<long long>::type,          long long>();
  static_assert_type<make_signed<unsigned long long>::type, long long>();
#endif
  static_assert_type<make_signed<natural_number>::type,     integer>();
  static_assert_type<make_signed<integer>::type,            integer>();
  static_assert<!make_signed<positive_float>::enabled>();
}

void test_make_unsigned()
{
  static_assert_type<make_unsigned<short>::type,              unsigned short>();
  static_assert_type<make_unsigned<unsigned short>::type,     unsigned short>();
  static_assert_type<make_unsigned<int>::type,                unsigned int>();
  static_assert_type<make_unsigned<unsigned int>::type,       unsigned int>();
  static_assert_type<make_unsigned<long>::type,               unsigned long>();
  static_assert_type<make_unsigned<unsigned long>::type,      unsigned long>();
#ifdef KANOOTH_HAS_LONG_LONG
  static_assert_type<make_unsigned<long long>::type,          unsigned long long>();
  static_assert_type<make_unsigned<unsigned long long>::type, unsigned long long>();
#endif
  static_assert_type<make_unsigned<natural_number>::type,     natural_number>();
  static_assert_type<make_unsigned<integer>::type,            natural_number>();
  static_assert_type<make_unsigned<positive_float>::type,     positive_float>();
}

void test_is_native_int()
{
  static_assert<is_native_int<short>::value>();
  static_assert<is_native_int<unsigned short>::value>();
  static_assert<is_native_int<int>::value>();
  static_assert<is_native_int<unsigned int>::value>();
  static_assert<is_native_int<long>::value>();
  static_assert<is_native_int<unsigned long>::value>();
#ifdef KANOOTH_HAS_LONG_LONG
  static_assert<is_native_int<long long>::value>();
  static_assert<is_native_int<unsigned long long>::value>();
#endif
  static_assert<!is_native_int<float>::value>();
  static_assert<!is_native_int<double>::value>();
  static_assert<!is_native_int<long double>::value>();
  static_assert<!is_native_int<natural_number>::value>();
  static_assert<!is_native_int<integer>::value>();
  static_assert<!is_native_int<positive_float>::value>();
}

void test_is_native_float()
{
  static_assert<!is_native_float<short>::value>();
  static_assert<!is_native_float<unsigned short>::value>();
  static_assert<!is_native_float<int>::value>();
  static_assert<!is_native_float<unsigned int>::value>();
  static_assert<!is_native_float<long>::value>();
  static_assert<!is_native_float<unsigned long>::value>();
#ifdef KANOOTH_HAS_LONG_LONG
  static_assert<!is_native_float<long long>::value>();
  static_assert<!is_native_float<unsigned long long>::value>();
#endif
  static_assert<is_native_float<float>::value>();
  static_assert<is_native_float<double>::value>();
  static_assert<is_native_float<long double>::value>();
  static_assert<!is_native_float<natural_number>::value>();
  static_assert<!is_native_float<integer>::value>();
  static_assert<!is_native_float<positive_float>::value>();
}

void test_is_native_number()
{
  static_assert<is_native_number<short>::value>();
  static_assert<is_native_number<unsigned short>::value>();
  static_assert<is_native_number<int>::value>();
  static_assert<is_native_number<unsigned int>::value>();
  static_assert<is_native_number<long>::value>();
  static_assert<is_native_number<unsigned long>::value>();
#ifdef KANOOTH_HAS_LONG_LONG
  static_assert<is_native_number<long long>::value>();
  static_assert<is_native_number<unsigned long long>::value>();
#endif
  static_assert<is_native_number<float>::value>();
  static_assert<is_native_number<double>::value>();
  static_assert<is_native_number<long double>::value>();
  static_assert<!is_native_number<natural_number>::value>();
  static_assert<!is_native_number<integer>::value>();
  static_assert<!is_native_number<positive_float>::value>();
}

int main()
{
  test_is_signed();
  test_is_integral();
  test_make_signed();
  test_make_unsigned();
  test_is_native_int();
  test_is_native_float();
  test_is_native_number();

  return 0;
}
