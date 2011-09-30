/*
 * File:   test/test_return_types.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-09-30 06:25Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2011
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

using kanooth::numbers::add_result;

void test_add_results()
{
  static_assert_type<typename add_result<unsigned short, unsigned short>::type, unsigned short>();

  static_assert_type<typename add_result<short, unsigned short>::type, short>();
  static_assert_type<typename add_result<unsigned short, short>::type, short>();
  static_assert_type<typename add_result<short, short>::type, short>();

  static_assert_type<typename add_result<unsigned, unsigned short>::type, unsigned>();
  static_assert_type<typename add_result<unsigned, short>::type, int>();
  static_assert_type<typename add_result<unsigned short, unsigned>::type, unsigned>();
  static_assert_type<typename add_result<short, unsigned>::type, int>();
  static_assert_type<typename add_result<unsigned, unsigned>::type, unsigned>();

  static_assert_type<typename add_result<int, unsigned short>::type, int>();
  static_assert_type<typename add_result<int, short>::type, int>();
  static_assert_type<typename add_result<int, unsigned>::type, int>();
  static_assert_type<typename add_result<unsigned short, int>::type, int>();
  static_assert_type<typename add_result<short, int>::type, int>();
  static_assert_type<typename add_result<unsigned, int>::type, int>();
  static_assert_type<typename add_result<int, int>::type, int>();
}

int main()
{
  test_add_results();
  
  std::cout << "Nothing to see here - if it compiled, the test was successful" << std::endl;
  
  return 0;
}
