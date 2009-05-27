/*
 * File:   tests.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-19 14:33Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#include <iostream>
#include <vector>
#include <boost/cstdint.hpp>
#include "detail/lowlevel.hpp"
#include "NonNegativeInteger.hpp"
#include "NNIutils.hpp"


#define ASSERT_ARGS __LINE__

template <typename T> const char* get_type_name() { return "Unknown"; }
template <> const char* get_type_name<boost::uint8_t>() { return "8 bits"; }
template <> const char* get_type_name<boost::uint16_t>() { return "16 bits"; }
template <> const char* get_type_name<boost::uint32_t>() { return "32 bits"; }
#ifndef BOOST_NO_INT64_T
template <> const char* get_type_name<boost::uint64_t>() { return "64 bits"; }
#endif

template <typename T>
void show_array(const T* first, const T* last)
{
  std::cerr << "[";
  if (first != last) {
    std::cerr << *first++;
    while (first != last) std::cerr << "," << *first++;
  }
  std::cerr << "]";
}

template <>
void show_array(const unsigned char* first, const unsigned char* last)
{
  std::cerr << "[";
  if (first != last) {
    std::cerr << (unsigned int) *first++;
    while (first != last)
      std::cerr << "," << (unsigned int) *first++;
  }
  std::cerr << "]";
}

template <typename T>
void assertTrue(bool condition, int linenum)
{
  if (!condition)
    std::cerr << "Fail on line " << linenum << std::endl;
}

template <typename T>
void assertArraysEqual(const T* first1, const T* last1, const T* first2, int linenum)
{
  while (first1 != last1) {
    if (*first1 != *first2) {
      std::cerr << "Fail: Arrays not equal (" << get_type_name<T>() << ") on line " << linenum << ", ";
      show_array(first1, last1);
      std::cerr << " != ";
      show_array(first2, first2 + (last1 - first1));
      std::cerr << std::endl;
      break;
    }
    ++first1;  ++first2;
  }
}


template <typename T>
void test_low_level_add_sequences_with_overflow()
{
  std::cout << "  testing add_sequences_with_overflow" << std::endl;

  T a[3] = { 3, 2, 1 };
  T b[2] = { 2, 1 };
  T c[10];
  T r[3] = { 5, 3, 1 };
  bool overflow;

  T* end = lowlevel::add_sequences_with_overflow(a, a+3, b, b+2, c, overflow);
  assertTrue<T>(!overflow, ASSERT_ARGS);
  assertTrue<T>(end == c + 3, ASSERT_ARGS);
  assertArraysEqual(r, r+3, c, ASSERT_ARGS);
}

template <typename T>
void test_double_mult_add_add()
{
  std::cout << "  testing double_mult_add_add" << std::endl;

  T high, low;
  T max = (T) -1;

  lowlevel::double_mult_add_add(max, max, max, max, low, high);
  assertTrue<T>(high == max, ASSERT_ARGS);
  assertTrue<T>(low == max, ASSERT_ARGS);
  lowlevel::double_mult_add_add(max, max, (T) 0, max, low, high);
  assertTrue<T>(high == max, ASSERT_ARGS);
  assertTrue<T>(low == 0, ASSERT_ARGS);
  lowlevel::double_mult_add_add(max, max, max, (T) 0, low, high);
  assertTrue<T>(high == max, ASSERT_ARGS);
  assertTrue<T>(low == 0, ASSERT_ARGS);
  lowlevel::double_mult_add_add(max, max, (T) 0, (T) 0, low, high);
  assertTrue<T>(high == max-1, ASSERT_ARGS);
  assertTrue<T>(low == 1, ASSERT_ARGS);
}

template <typename T>
void test_division()
{
  std::cout << "  testing division" << std::endl;

  const unsigned int limbbits = boost::integer_traits<T>::digits;
  const T half = (T) 1 << (limbbits - 1);
  typedef NonNegativeInteger<T> NNI;

  NNI u = NNI(half).binary_shift_this(3*limbbits);
  NNI v = NNI(half).binary_shift_this(2*limbbits) + NNI(1);

  std::pair<NNI,NNI> div = u.divide(u, v);
  NNI z = div.first*v + div.second;

  assertTrue<T>(z == u, ASSERT_ARGS);
}


template <typename T>
void test_factorization()
{
  std::cout << "  testing factorization" << std::endl;

  typedef NonNegativeInteger<T> NNI;
  std::vector<NNI> factors;
  factorize(string_to_nni<NNI>("600851475143"), std::back_inserter(factors));
  NNI expected = string_to_nni<NNI>("6857");
  assertTrue<T>(factors.back() == expected, ASSERT_ARGS);
}


template <typename T>
void all_tests_for_type()
{
  std::cout << "all tests for type " << get_type_name<T>() << std::endl;

  test_low_level_add_sequences_with_overflow<T>();
  test_double_mult_add_add<T>();
  test_division<T>();
  test_factorization<T>();

  std::cout << std::endl;
}

int main()
{
  all_tests_for_type<boost::uint8_t>();
  all_tests_for_type<boost::uint16_t>();
  all_tests_for_type<boost::uint32_t>();
# ifndef BOOST_NO_INT64_T
  all_tests_for_type<boost::uint64_t>();
# endif

  std::cerr.flush();
  std::cout << "testing done" << std::endl;

  return 0;
}
