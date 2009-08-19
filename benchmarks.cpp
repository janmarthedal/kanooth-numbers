/*
 * File:   benchmarks.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-06-06 09:14Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#include <iostream>
#include <boost/cstdint.hpp>
#include <boost/timer.hpp>
#include <boost/random.hpp>
#include "NonNegativeInteger.hpp"

//using com::sputsoft::multiprecision::NonNegativeInteger;

template <typename T> const char* get_type_name() { return "Unknown"; }
template <> const char* get_type_name<boost::uint8_t>() { return "8 bits"; }
template <> const char* get_type_name<boost::uint16_t>() { return "16 bits"; }
template <> const char* get_type_name<boost::uint32_t>() { return "32 bits"; }
#ifndef BOOST_NO_INT64_T
template <> const char* get_type_name<boost::uint64_t>() { return "64 bits"; }
#endif

// Try boost::mt19937 or boost::ecuyer1988 instead of boost::minstd_rand
boost::minstd_rand generator(42u);


template <typename T>
void add_random(int u_bits, int v_bits, int runs)
{
  typedef NonNegativeInteger<T> NNI;

  std::cout << "  Addition ";  std::cout.flush();

  NNI u = NNI::make_random(generator, u_bits);
  NNI v = NNI::make_random(generator, v_bits);
  NNI w;

  boost::timer loop_time;
  for (int k=runs; k != 0; --k)
    w = NNI::add(u, v);
  double t = loop_time.elapsed();

  std::cout << 1.0e-6*runs*std::max(u_bits, v_bits)/t << " Mbits/s" << std::endl;
}

template <typename T>
void long_multiplication_random(int u_bits, int v_bits, int runs)
{
  typedef NonNegativeInteger<T> NNI;

  std::cout << "  Long multiplication ";  std::cout.flush();

  NNI u = NNI::make_random(generator, u_bits);
  NNI v = NNI::make_random(generator, v_bits);
  NNI w;

  boost::timer loop_time;
  for (int k=runs; k != 0; --k)
    w = NNI::multiply(u, v);
  double t = loop_time.elapsed();

  std::cout << 1.0e-6*runs*(u_bits+v_bits)/t << " Mbits/s" << std::endl;
}

template <typename T>
void long_division_random(long u_bits, long v_bits, int runs)
{
  typedef NonNegativeInteger<T> NNI;

  std::cout << "  Long division ";  std::cout.flush();

  NNI u = NNI::make_random(generator, u_bits+v_bits);
  NNI v = NNI::make_random(generator, v_bits);
  std::pair<NNI, NNI> divrem;

  boost::timer loop_time;
  for (int k=runs; k != 0; --k)
    divrem = NNI::divide(u, v);
  double t = loop_time.elapsed();

  std::cout << 1.0e-6*runs*(u_bits+v_bits)/t << " Mbits/s" << std::endl;
}

template <typename T>
void long_division_check_random(long u_bits, long v_bits, int runs)
{
  typedef NonNegativeInteger<T> NNI;

  std::cout << "  Long division check ";  std::cout.flush();

  NNI u = NNI::make_random(generator, u_bits+v_bits);
  NNI v = NNI::make_random(generator, v_bits);
  std::pair<NNI, NNI> divrem;
  int passed = 0;

  boost::timer loop_time;
  for (int k=runs; k != 0; --k) {
    divrem = NNI::divide(u, v);
    if (u == divrem.first * v + divrem.second)
      ++passed;
  }
  double t = loop_time.elapsed();

  std::cout << 1.0e-6*runs*(u_bits+v_bits)/t << " Mbits/s" << std::endl;

  if (passed != runs)
    std::cout << "  ** Check failed **" << std::endl;
}

template <typename T>
void benchmarks()
{
  std::cout << "Running benchmarks for " << get_type_name<T>() << std::endl;

  add_random<T>(800000, 400000, 10000);
  add_random<T>(600000, 600000, 1000);
  //long_multiplication_random<T>(80000, 40000, 10);
  //long_division_random<T>(80000, 40000, 10);
  //long_division_check_random<T>(80000, 40000, 10);
}

int main()
{
  benchmarks<boost::uint8_t>();
  benchmarks<boost::uint16_t>();
  benchmarks<boost::uint32_t>();
#ifndef BOOST_NO_INT64_T
  benchmarks<boost::uint64_t>();
#endif
}
