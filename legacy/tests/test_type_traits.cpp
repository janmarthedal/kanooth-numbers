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

#include "test_common.hpp"
#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/number_traits.hpp>
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
using kanooth::number_bits;
using kanooth::numbers::is_number;
using kanooth::numbers::natural_number;
using kanooth::numbers::integer;
using kanooth::numbers::positive_float;

void test_is_signed()
{
  KANOOTH_STATIC_ASSERT(is_signed<short>::value);
  KANOOTH_STATIC_ASSERT(!is_signed<unsigned short>::value);
  KANOOTH_STATIC_ASSERT(is_signed<int>::value);
  KANOOTH_STATIC_ASSERT(!is_signed<unsigned int>::value);
  KANOOTH_STATIC_ASSERT(is_signed<long>::value);
  KANOOTH_STATIC_ASSERT(!is_signed<unsigned long>::value);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT(is_signed<long long>::value);
  KANOOTH_STATIC_ASSERT(!is_signed<unsigned long long>::value);
#endif
  KANOOTH_STATIC_ASSERT(is_signed<float>::value);
  KANOOTH_STATIC_ASSERT(is_signed<double>::value);
  KANOOTH_STATIC_ASSERT(is_signed<long double>::value);
  KANOOTH_STATIC_ASSERT(!is_signed<natural_number>::value);
  KANOOTH_STATIC_ASSERT(is_signed<integer>::value);
  KANOOTH_STATIC_ASSERT(!is_signed<positive_float>::value);
}

void test_is_integral()
{
  KANOOTH_STATIC_ASSERT(is_integral<short>::value);
  KANOOTH_STATIC_ASSERT(is_integral<unsigned short>::value);
  KANOOTH_STATIC_ASSERT(is_integral<int>::value);
  KANOOTH_STATIC_ASSERT(is_integral<unsigned int>::value);
  KANOOTH_STATIC_ASSERT(is_integral<long>::value);
  KANOOTH_STATIC_ASSERT(is_integral<unsigned long>::value);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT(is_integral<long long>::value);
  KANOOTH_STATIC_ASSERT(is_integral<unsigned long long>::value);
#endif
  KANOOTH_STATIC_ASSERT(!is_integral<float>::value);
  KANOOTH_STATIC_ASSERT(!is_integral<double>::value);
  KANOOTH_STATIC_ASSERT(!is_integral<long double>::value);
  KANOOTH_STATIC_ASSERT(is_integral<natural_number>::value);
  KANOOTH_STATIC_ASSERT(is_integral<integer>::value);
  KANOOTH_STATIC_ASSERT(!is_integral<positive_float>::value);
}

void test_make_signed()
{
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<short>::type,              short);
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<unsigned short>::type,     short);
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<int>::type,                int);
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<unsigned int>::type,       int);
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<long>::type,               long);
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<unsigned long>::type,      long);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<long long>::type,          long long);
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<unsigned long long>::type, long long);
#endif
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<natural_number>::type,     integer);
  KANOOTH_STATIC_ASSERT_TYPE(make_signed<integer>::type,            integer);
  KANOOTH_STATIC_ASSERT(!make_signed<positive_float>::enabled);
}

void test_make_unsigned()
{
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<short>::type,              unsigned short);
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<unsigned short>::type,     unsigned short);
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<int>::type,                unsigned int);
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<unsigned int>::type,       unsigned int);
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<long>::type,               unsigned long);
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<unsigned long>::type,      unsigned long);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<long long>::type,          unsigned long long);
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<unsigned long long>::type, unsigned long long);
#endif
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<natural_number>::type,     natural_number);
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<integer>::type,            natural_number);
  KANOOTH_STATIC_ASSERT_TYPE(make_unsigned<positive_float>::type,     positive_float);
}

void test_is_native_int()
{
  KANOOTH_STATIC_ASSERT(is_native_int<short>::value);
  KANOOTH_STATIC_ASSERT(is_native_int<unsigned short>::value);
  KANOOTH_STATIC_ASSERT(is_native_int<int>::value);
  KANOOTH_STATIC_ASSERT(is_native_int<unsigned int>::value);
  KANOOTH_STATIC_ASSERT(is_native_int<long>::value);
  KANOOTH_STATIC_ASSERT(is_native_int<unsigned long>::value);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT(is_native_int<long long>::value);
  KANOOTH_STATIC_ASSERT(is_native_int<unsigned long long>::value);
#endif
  KANOOTH_STATIC_ASSERT(!is_native_int<float>::value);
  KANOOTH_STATIC_ASSERT(!is_native_int<double>::value);
  KANOOTH_STATIC_ASSERT(!is_native_int<long double>::value);
  KANOOTH_STATIC_ASSERT(!is_native_int<natural_number>::value);
  KANOOTH_STATIC_ASSERT(!is_native_int<integer>::value);
  KANOOTH_STATIC_ASSERT(!is_native_int<positive_float>::value);
}

void test_is_native_float()
{
  KANOOTH_STATIC_ASSERT(!is_native_float<short>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<unsigned short>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<int>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<unsigned int>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<long>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<unsigned long>::value);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT(!is_native_float<long long>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<unsigned long long>::value);
#endif
  KANOOTH_STATIC_ASSERT(is_native_float<float>::value);
  KANOOTH_STATIC_ASSERT(is_native_float<double>::value);
  KANOOTH_STATIC_ASSERT(is_native_float<long double>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<natural_number>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<integer>::value);
  KANOOTH_STATIC_ASSERT(!is_native_float<positive_float>::value);
}

void test_is_native_number()
{
  KANOOTH_STATIC_ASSERT(is_native_number<short>::value);
  KANOOTH_STATIC_ASSERT(is_native_number<unsigned short>::value);
  KANOOTH_STATIC_ASSERT(is_native_number<int>::value);
  KANOOTH_STATIC_ASSERT(is_native_number<unsigned int>::value);
  KANOOTH_STATIC_ASSERT(is_native_number<long>::value);
  KANOOTH_STATIC_ASSERT(is_native_number<unsigned long>::value);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT(is_native_number<long long>::value);
  KANOOTH_STATIC_ASSERT(is_native_number<unsigned long long>::value);
#endif
  KANOOTH_STATIC_ASSERT(is_native_number<float>::value);
  KANOOTH_STATIC_ASSERT(is_native_number<double>::value);
  KANOOTH_STATIC_ASSERT(is_native_number<long double>::value);
  KANOOTH_STATIC_ASSERT(!is_native_number<natural_number>::value);
  KANOOTH_STATIC_ASSERT(!is_native_number<integer>::value);
  KANOOTH_STATIC_ASSERT(!is_native_number<positive_float>::value);
}

void test_number_bits()
{
  KANOOTH_STATIC_ASSERT(number_bits<short>::value + 1 == number_bits<unsigned short>::value);
  KANOOTH_STATIC_ASSERT(number_bits<short>::value <= number_bits<int>::value);
  KANOOTH_STATIC_ASSERT(number_bits<int>::value + 1 == number_bits<unsigned int>::value);
  KANOOTH_STATIC_ASSERT(number_bits<int>::value <= number_bits<long>::value);
  KANOOTH_STATIC_ASSERT(number_bits<long>::value + 1 == number_bits<unsigned long>::value);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT(number_bits<long>::value <= number_bits<long long>::value);
  KANOOTH_STATIC_ASSERT(number_bits<long long>::value + 1 == number_bits<unsigned long long>::value);
#endif
}

void test_is_number()
{
  KANOOTH_STATIC_ASSERT(!is_number<char>::value);
  KANOOTH_STATIC_ASSERT(!is_number<int*>::value);
  
  KANOOTH_STATIC_ASSERT(is_number<short>::value);
  KANOOTH_STATIC_ASSERT(is_number<unsigned short>::value);
  KANOOTH_STATIC_ASSERT(is_number<int>::value);
  KANOOTH_STATIC_ASSERT(is_number<unsigned int>::value);
  KANOOTH_STATIC_ASSERT(is_number<long>::value);
  KANOOTH_STATIC_ASSERT(is_number<unsigned long>::value);
#ifdef KANOOTH_HAS_LONG_LONG
  KANOOTH_STATIC_ASSERT(is_number<long long>::value);
  KANOOTH_STATIC_ASSERT(is_number<unsigned long long>::value);
#endif
  KANOOTH_STATIC_ASSERT(is_number<float>::value);
  KANOOTH_STATIC_ASSERT(is_number<double>::value);
  KANOOTH_STATIC_ASSERT(is_number<long double>::value);
  KANOOTH_STATIC_ASSERT(is_number<natural_number>::value);
  KANOOTH_STATIC_ASSERT(is_number<integer>::value);
  KANOOTH_STATIC_ASSERT(is_number<positive_float>::value);
}

int main()
{
  test_is_signed();
  test_is_integral();
  test_make_signed();
  test_make_unsigned();
  test_is_native_int();
  test_is_native_float();
  test_number_bits();
  test_is_native_number();

  std::cout << "Nothing to see here - if it compiled, the test was successful" << std::endl;
  
  return 0;
}
