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

using namespace kanooth::numbers;

template <typename EXP, typename ACT> struct type_matcher;

template <typename T>
struct type_matcher<T, T> {
  static void print(const T& v) {
    std::cout << v << std::endl;
  }
};

template <typename EXP, typename ACT>
void check_type(const ACT& v) {
  type_matcher<EXP, ACT>::print(v);
}


int main()
{

  return 0;
}
