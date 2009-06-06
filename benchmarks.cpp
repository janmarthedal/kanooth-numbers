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
void benchmarks()
{
  typedef NonNegativeInteger<T> NNI;

  std::cout << "Running benchmarks for " << get_type_name<T>() << std::endl;
}

int main()
{
  benchmarks<boost::uint8_t>();
}
