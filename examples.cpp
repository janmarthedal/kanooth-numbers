/*
 * File:   examples.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-04 18:02Z
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
#include <boost/integer_traits.hpp>
#include "NonNegativeInteger.hpp"

//using com::sputsoft::multiprecision::NonNegativeInteger;

//typedef unsigned char limb_t;
typedef unsigned short limb_t;
//typedef unsigned int limb_t;
//typedef unsigned long limb_t;
//typedef unsigned long long limb_t;
//typedef boost::uintmax_t limb_t;
typedef NonNegativeInteger<limb_t> NNI;


template <typename NONNEGNUM>
NONNEGNUM construct(const std::string& digits, unsigned short base=10)
{
  NONNEGNUM r = NONNEGNUM::zero;
  if (base >= 2 && base <= 36) {
    NONNEGNUM b(base);
    unsigned short v;
    for (std::string::const_iterator p=digits.begin(); p != digits.end(); ++p) {
      char c = *p;
      if (c >= '0' && c <= '9') v = c - '0';
      else if (c >= 'a' && c <= 'z') v = c - 'a' + 10;
      else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
      else break;
      r *= b;
      r += NONNEGNUM(v);
    }
  }
  return r;
}

void check3()
{
  NNI u = NNI(128).binary_shift_this(80) + NNI(128).binary_shift_this(64)
          + NNI(128).binary_shift_this(48) + NNI(127).binary_shift_this(32);
  NNI v = NNI(128).binary_shift_this(48) + NNI(128).binary_shift_this(32)
          + NNI(128).binary_shift_this(16) + NNI(128);

  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
  std::pair<NNI,NNI> div = u.divide(u, v);
  std::cout << "u/v: " << div.first << std::endl;
  std::cout << "u%v: " << div.second << std::endl;

  NNI z = div.first*v + div.second;

  std::cout << "Verification: " << (z == u ? "OK" : "FAIL") << std::endl;
}

void check4()
{
  NNI u = construct<NNI>("999");
  NNI v = construct<NNI>("99999999");
  NNI r = construct<NNI>("99899999001");

  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
  std::cout << "u*v: " << u*v << std::endl;
  std::cout << "r  : " << r << std::endl;
}

int main()
{
  check4();
}
