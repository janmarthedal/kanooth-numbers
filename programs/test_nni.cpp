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

#include <detail/lowlevel.hpp>
#include <nonnegative_integer.hpp>
#include <nni_utils.hpp>

using sputsoft::multiprecision::lowlevel::add_sequences_with_overflow;
using sputsoft::multiprecision::lowlevel::double_mult_add_add;
using sputsoft::multiprecision::lowlevel::double_div;
using sputsoft::multiprecision::DivideByZero;
using sputsoft::multiprecision::string_to_nni;
using sputsoft::multiprecision::NonNegativeInteger;


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
    std::cout << "************** Fail on line " << linenum << " **************" << std::endl;
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

  bool overflow;
  T a[3] = { 3, 2, 1 };
  T b[2] = { 2, 1 };
  T c[10];
  T r[3] = { 5, 3, 1 };

  T* end = add_sequences_with_overflow(a, a+3, b, b+2, c, overflow);
  assertTrue<T>(!overflow, ASSERT_ARGS);
  assertTrue<T>(end == c+3, ASSERT_ARGS);
  assertArraysEqual(r, r+3, c, ASSERT_ARGS);

  T a2[3] = { (T) -3, (T) -2, (T) -1 };
  T b2[2] = { (T) -5, (T) -4 };
  T r2[3] = { (T) -8, (T) -5, (T)  0 };

  end = add_sequences_with_overflow(a2, a2+3, b2, b2+2, c, overflow);
  assertTrue<T>(overflow, ASSERT_ARGS);
  assertTrue<T>(end == c+3, ASSERT_ARGS);
  assertArraysEqual(r2, r2+3, c, ASSERT_ARGS);
}

template <typename T>
void test_double_mult_add_add()
{
  std::cout << "  testing double_mult_add_add" << std::endl;

  T high, low;
  T max = (T) -1;

  double_mult_add_add(max, max, max, max, low, high);
  assertTrue<T>(high == max, ASSERT_ARGS);
  assertTrue<T>(low == max, ASSERT_ARGS);
  double_mult_add_add(max, max, (T) 0, max, low, high);
  assertTrue<T>(high == max, ASSERT_ARGS);
  assertTrue<T>(low == 0, ASSERT_ARGS);
  double_mult_add_add(max, max, max, (T) 0, low, high);
  assertTrue<T>(high == max, ASSERT_ARGS);
  assertTrue<T>(low == 0, ASSERT_ARGS);
  double_mult_add_add(max, max, (T) 0, (T) 0, low, high);
  assertTrue<T>(high == max-1, ASSERT_ARGS);
  assertTrue<T>(low == 1, ASSERT_ARGS);
}

template <typename T>
void test_double_div()
{
  std::cout << "  testing double_div" << std::endl;

  const unsigned int bits = boost::integer_traits<T>::digits;
  T q, r;

  double_div( T((T(1) << (bits/2-2)) + 1), T(0),
      T((T(1) << (bits-1)) + (T(1) << (bits/2)) - 1), q, r);
  assertTrue<T>(q == (T(1) << (bits/2-1)), ASSERT_ARGS);
  assertTrue<T>(r == (((T(1) << (bits-1)) + (T(1) << (bits/2-1)))), ASSERT_ARGS);

  double_div(T(-2), T(-2), T(-1), q, r);
  assertTrue<T>(q == T(-1), ASSERT_ARGS);
  assertTrue<T>(r == T(-3), ASSERT_ARGS);
}

template <typename T>
void test_divide_simple()
{
  std::cout << "  testing divide_simple" << std::endl;

  typedef NonNegativeInteger<T> NNI;
  std::pair<NNI, T> divrem;
  bool divbyzero = false;

  try {
    NNI::divide_simple(NNI(1), (T) 0);
  } catch (DivideByZero ex) {
    divbyzero = true;
  }
  assertTrue<T>(divbyzero, ASSERT_ARGS);

  divbyzero = false;
  try {
    NNI::divide_simple(NNI::zero, (T) 0);
  } catch (DivideByZero ex) {
    divbyzero = true;
  }
  assertTrue<T>(divbyzero, ASSERT_ARGS);

  NNI u = string_to_nni<NNI>("417661693688739506639586565361");
  divrem = NNI::divide_simple(u, (T) 1);
  assertTrue<T>(divrem.first == u, ASSERT_ARGS);
  assertTrue<T>(!divrem.second, ASSERT_ARGS);

  divrem = NNI::divide_simple(u, (T) 251);
  assertTrue<T>(divrem.first == string_to_nni<NNI>("1663990811508922337209508228"), ASSERT_ARGS);
  assertTrue<T>(divrem.second == 133, ASSERT_ARGS);
}

template <typename T>
void test_divide_long_1()
{
  std::cout << "  testing divide_long_1" << std::endl;

  typedef NonNegativeInteger<T> NNI;
  const unsigned int limbbits = boost::integer_traits<T>::digits;
  const T half = (T) 1 << (limbbits - 1);

  // Force add back
  NNI u = NNI(half).binary_shift_this(3*limbbits);
  NNI v = NNI(half).binary_shift_this(2*limbbits) + NNI(1);
  std::pair<NNI,NNI> divrem = NNI::divide(u, v);
  NNI z = divrem.first*v + divrem.second;
  assertTrue<T>(z == u, ASSERT_ARGS);
}

boost::minstd_rand generator(2u);

template <typename T>
void test_divide_long_2()
{
  std::cout << "  testing divide_long_2" << std::endl;

  typedef NonNegativeInteger<T> NNI;
  for (std::size_t u_bits=4; u_bits <= 9; ++u_bits) {
    for (std::size_t v_bits=8; v_bits <= 15; ++v_bits) {
      NNI u = NNI::make_random(generator, u_bits+v_bits);
      NNI v = NNI::make_random(generator, v_bits);
      std::pair<NNI,NNI> divrem = NNI::divide(u, v);
      NNI z = divrem.first*v + divrem.second;
      assertTrue<T>(z == u, ASSERT_ARGS);
      /*if (z != u) {
        std::cout << u_bits << " " << v_bits << std::endl;
        std::cout << u << std::endl;
        std::cout << v << std::endl;
        std::cout << divrem.first << std::endl;
        std::cout << divrem.second << std::endl;
      }*/
    }
  }
}

template <typename T>
void test_binary_and()
{
  std::cout << "  testing binary_and" << std::endl;

  typedef NonNegativeInteger<T> NNI;

  NNI u = string_to_nni<NNI>("1110100010101010110101001001001001001010100101010001", 2);
  NNI v = string_to_nni<NNI>(           "11101000101010101101010010010010010010101", 2);
  NNI z = string_to_nni<NNI>(            "1000000101000001001000000010000000010001", 2);

  assertTrue<T>(NNI::binary_and(u, NNI::zero) == NNI::zero, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_and(NNI::zero, u) == NNI::zero, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_and(u, v) == z, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_and(v, u) == z, ASSERT_ARGS);
}

template <typename T>
void test_binary_or()
{
  std::cout << "  testing binary_or" << std::endl;

  typedef NonNegativeInteger<T> NNI;

  NNI u = string_to_nni<NNI>("1110100010101010110101001001001001001010100101010001", 2);
  NNI v = string_to_nni<NNI>(           "11101000101010101101010010010010010010101", 2);
  NNI z = string_to_nni<NNI>("1110100010111111110101011101101011011010110111010101", 2);

  assertTrue<T>(NNI::binary_or(u, NNI::zero) == u, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_or(NNI::zero, u) == u, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_or(u, v) == z, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_or(v, u) == z, ASSERT_ARGS);
}


template <typename T>
void test_binary_xor()
{
  std::cout << "  testing binary_xor" << std::endl;

  typedef NonNegativeInteger<T> NNI;

  NNI u = string_to_nni<NNI>("1110100010101010110101001001001001001010100101010001", 2);
  NNI v = string_to_nni<NNI>(           "11101000101010101101010010010010010010101", 2);
  NNI z = string_to_nni<NNI>("1110100010110111110000011100100011011000110111000100", 2);

  assertTrue<T>(NNI::binary_xor(u, NNI::zero) == u, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(NNI::zero, u) == u, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(u, u) == NNI::zero, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(v, v) == NNI::zero, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(u, v) == z, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(v, u) == z, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(u, z) == v, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(z, u) == v, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(z, v) == u, ASSERT_ARGS);
  assertTrue<T>(NNI::binary_xor(v, z) == u, ASSERT_ARGS);
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
  test_double_div<T>();

  test_divide_simple<T>();
  test_divide_long_1<T>();
  test_divide_long_2<T>();
  test_binary_and<T>();
  test_binary_or<T>();
  test_binary_xor<T>();

  test_factorization<T>();

  std::cout << std::endl;
}

int main()
{
  all_tests_for_type<boost::uint8_t>();
  all_tests_for_type<boost::uint16_t>();
  all_tests_for_type<boost::uint32_t>();
#ifndef BOOST_NO_INT64_T
  all_tests_for_type<boost::uint64_t>();
#endif

  std::cerr.flush();
  std::cout << "testing done" << std::endl;

  return 0;
}
