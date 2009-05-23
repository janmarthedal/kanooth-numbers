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
#include "lowlevel.hpp"

#define ASSERT_ARGS __LINE__

template <typename T> const char* get_type_name() { return "Unknown"; }
template <> const char* get_type_name<unsigned char>() { return "unsigned char"; }
template <> const char* get_type_name<unsigned short>() { return "unsigned short"; }
template <> const char* get_type_name<unsigned int>() { return "unsigned int"; }
template <> const char* get_type_name<unsigned long>() { return "unsigned long"; }

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
void test_low_level_addition()
{
  T a[3] = { 3, 2, 1 };
  T b[2] = { 2, 1 };
  T c[10];
  T r[3] = { 5, 3, 1 };

  std::pair<bool, T*> ret = /*com::sputsoft::multiprecision::*/lowlevel::add_sequences_with_overflow(a, a+3, b, b+2, c);
  assertTrue<T>(!ret.first, ASSERT_ARGS);
  assertTrue<T>(ret.second == c + 3, ASSERT_ARGS);
  assertArraysEqual(r, r+3, c, ASSERT_ARGS);
}

template <typename T>
void all_tests_for_type()
{
  test_low_level_addition<T>();
}

int main()
{
  all_tests_for_type<unsigned char>();
  all_tests_for_type<unsigned short>();
  all_tests_for_type<unsigned int>();
  all_tests_for_type<unsigned long>();

  std::cerr.flush();
  std::cout << "Testing done" << std::endl;

  return 0;
}
