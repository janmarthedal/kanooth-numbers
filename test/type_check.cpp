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

#include <kanooth/numbers/type_traits.hpp>
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

template <typename I>
void check_with_int()
{
  natural_number n1 = 3, n2;
  typedef typename kanooth::numbers::make_unsigned<I>::type unsigned_int;
  I i = 5;
  unsigned_int k;
  add(n2, n1, i);              std::cout << n2 << std::endl;
  add(n2, i, n1);              std::cout << n2 << std::endl;
  sub(n2, n1, i);              std::cout << n2 << std::endl;
  sub(n2, i, n1);              std::cout << n2 << std::endl;
  sub(k, i, n1);               std::cout << k  << std::endl;
  mul(n2, n1, i);              std::cout << n2 << std::endl;
  mul(n2, i, n1);              std::cout << n2 << std::endl;
  div(n2, n1, i);              std::cout << n2 << std::endl;
  div(n2, i, n1);              std::cout << n2 << std::endl;
  bitwise_and(k, n1, i);       std::cout << k  << std::endl;
  bitwise_and(k, i, n1);       std::cout << k  << std::endl;
  bitwise_or(n2, n1, i);       std::cout << n2 << std::endl;
  bitwise_or(n2, i, n1);       std::cout << n2 << std::endl;
  bitwise_xor(n2, n1, i);      std::cout << n2 << std::endl;
  bitwise_xor(n2, i, n1);      std::cout << n2 << std::endl;
  bitwise_and_not(n2, n1, i);  std::cout << n2 << std::endl;
  bitwise_and_not(k, i, n1);   std::cout << n2 << std::endl;
  check_type<natural_number>(add(n1, i));
  check_type<natural_number>(add(i, n1));
  check_type<natural_number>(sub(n1, i));
  check_type<unsigned_int>(sub(i, n1));
  check_type<natural_number>(mul(n1, i));
  check_type<natural_number>(mul(i, n1));
  check_type<natural_number>(div(n1, i));
  check_type<natural_number>(div(i, n1));   // ??
  check_type<unsigned_int>(bitwise_and(n1, i));
  check_type<unsigned_int>(bitwise_and(i, n1));
  check_type<natural_number>(bitwise_or(n1, i));
  check_type<natural_number>(bitwise_or(i, n1));
  check_type<natural_number>(bitwise_xor(n1, i));
  check_type<natural_number>(bitwise_xor(i, n1));
  check_type<natural_number>(bitwise_and_not(n1, i));
  check_type<unsigned_int>(bitwise_and_not(i, n1));
}

int main()
{
  natural_number n1 = 3, n2 = 7, n3;

  add(n3, n1, n2);              std::cout << n3 << std::endl;
  sub(n3, n2, n1);              std::cout << n3 << std::endl;
  mul(n3, n1, n2);              std::cout << n3 << std::endl;
  div(n3, n1, n2);              std::cout << n3 << std::endl;
  bitwise_and(n3, n1, n2);      std::cout << n3 << std::endl;
  bitwise_or(n3, n1, n2);       std::cout << n3 << std::endl;
  bitwise_xor(n3, n1, n2);      std::cout << n3 << std::endl;
  bitwise_and_not(n3, n1, n2);  std::cout << n3 << std::endl;
  check_type<natural_number>(add(n1, n2));
  check_type<natural_number>(sub(n1, n2));
  check_type<natural_number>(mul(n1, n2));
  check_type<natural_number>(div(n1, n2));
  check_type<natural_number>(bitwise_and(n1, n2));
  check_type<natural_number>(bitwise_or(n1, n2));
  check_type<natural_number>(bitwise_xor(n1, n2));
  check_type<natural_number>(bitwise_and_not(n1, n2));

  std::cout << "***" << std::endl;

  check_with_int<unsigned short>();
  std::cout << "***" << std::endl;
  check_with_int<unsigned>();
  std::cout << "***" << std::endl;
  check_with_int<unsigned long>();
  std::cout << "***" << std::endl;
  check_with_int<int>();
#ifdef SPUTSOFT_HAS_LONG_LONG
  std::cout << "***" << std::endl;
  check_with_int<unsigned long long>();
#endif

  return 0;
}
