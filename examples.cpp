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
#include "Integer.hpp"

using com::sputsoft::multiprecision::NonNegativeInteger;
using com::sputsoft::multiprecision::Integer;

//typedef unsigned char limb_t;
typedef unsigned short limb_t;
//typedef unsigned int limb_t;
//typedef unsigned long limb_t;
//typedef unsigned long long limb_t;
//typedef boost::uintmax_t limb_t;
typedef NonNegativeInteger<limb_t> NNI;

template <typename NUMBER>
NUMBER construct(const std::string& digits, unsigned short base=10)
{
  NUMBER r = NUMBER::zero;
  if (base >= 2 && base <= 36) {
    NUMBER b(base);
    unsigned short v;
    for (std::string::const_iterator p=digits.begin(); p != digits.end(); ++p) {
      char c = *p;
      if (c >= '0' && c <= '9') v = c - '0';
      else if (c >= 'a' && c <= 'z') v = c - 'a' + 10;
      else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
      else break;
      r *= b;
      r += NUMBER(v);
    }
  }
  return r;
}

/*void check3()
{
  NNI u = NNI(128).binary_shift_this(32) + NNI(128).binary_shift_this(24)
          + NNI(128).binary_shift_this(16) + NNI(127).binary_shift_this(8);
  //NNI u = NNI(128).binary_shift_this(40) + NNI(128).binary_shift_this(32)
  //        + NNI(128).binary_shift_this(24) + NNI(127).binary_shift_this(16);
  NNI v = NNI(128).binary_shift_this(24) + NNI(128).binary_shift_this(16)
          + NNI(128).binary_shift_this(8) + NNI(128);

  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
  std::pair<NNI,NNI> div = u.divide(u, v);
  std::cout << "u/v: " << div.first << std::endl;
  std::cout << "u%v: " << div.second << std::endl;
}

void check4()
{
  NNI u = NNI(1).binary_shift_this(32) + NNI(2).binary_shift_this(16) + NNI(3);
  NNI v = NNI(128).binary_shift_this(24);

  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
  std::pair<NNI,NNI> div = u.divide(u, v);
  std::cout << "u/v: " << div.first << std::endl;
  std::cout << "u%v: " << div.second << std::endl;
}

inline unsigned asmadd(unsigned a, unsigned b)
{
  unsigned sum;
  __asm__ ("addl %1, %0"
           : "=r" (sum)
           : "r" (a), "0" (b)
           : );
  return sum;
}

inline std::pair<unsigned, unsigned> asmdiv(unsigned uhigh, unsigned ulow, unsigned v)
{
  unsigned q, r;
  __asm__ ("divl %%ebx"
           : "=a" (q), "=d" (r)
           : "d" (uhigh), "a" (ulow), "b" (v)
           : );
  return std::make_pair(q, r);
}*/

int main()
{
  NNI u = construct<NNI>("12345678");

  std::cout << "u: " << u << std::endl;

  u.binary_shift_this(10);

  std::cout << "u: " << u << std::endl;

  u.binary_shift_this(-100);

  std::cout << "u: " << u << std::endl;

  //check3();
}
