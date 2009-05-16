/* 
 * File:   test.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created on 4. maj 2009, 20:02
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
//typedef unsigned long limb_t;
//typedef unsigned long long limb_t;
//typedef boost::uintmax_t limb_t;
typedef NonNegativeInteger<limb_t> NNI;

/*void check1()
{
  NonNegativeInteger<limb_t> n = boost::integer_traits<limb_t>::const_max;
  std::cout << "n=" << n << std::endl;
  NonNegativeInteger<limb_t> m = n;
  std::cout << "m=" << m << std::endl;
  std::cout << "n=" << n << std::endl;
  for (int i=0; i < 5; ++i) {
    m += n;
    std::cout << "m=" << m << std::endl;
    std::cout << "n=" << n << std::endl;
  }
  for (int i=0; i < 6; ++i) {
    m -= n;
    std::cout << "m=" << m << std::endl;
    std::cout << "n=" << n << std::endl;
  }
  std::cout << (m < n ? "m < n" : "m >= n") << std::endl;
  std::cout << (m == n ? "m == n" : "m != n") << std::endl;
  std::cout << (m < NonNegativeInteger<limb_t>::zero ? "m < 0" : "m >= 0") << std::endl;
}

template <typename T>
void addcheck(const Integer<T>& i1, const Integer<T>& i2)
{
  std::cout << "i1   : " << i1 << std::endl;
  std::cout << "i2   : " << i2 << std::endl;
  std::cout << "i1+i2: " << Integer<T>::add(i1, i2) << std::endl;
}

void addcheck1()
{
  NonNegativeInteger<limb_t> n1 = 2;
  NonNegativeInteger<limb_t> n2 = 8;
  addcheck(Integer<limb_t>(n1, true), Integer<limb_t>(n2, true));
  addcheck(Integer<limb_t>(n1, true), Integer<limb_t>(n2, false));
  addcheck(Integer<limb_t>(n1, false), Integer<limb_t>(n2, true));
  addcheck(Integer<limb_t>(n1, false), Integer<limb_t>(n2, false));
  addcheck(Integer<limb_t>(n2, true), Integer<limb_t>(n1, true));
  addcheck(Integer<limb_t>(n2, true), Integer<limb_t>(n1, false));
  addcheck(Integer<limb_t>(n2, false), Integer<limb_t>(n1, true));
  addcheck(Integer<limb_t>(n2, false), Integer<limb_t>(n1, false));
}

template <typename T>
void subcheck(const Integer<T>& i1, const Integer<T>& i2)
{
  std::cout << "i1   : " << i1 << std::endl;
  std::cout << "i2   : " << i2 << std::endl;
  std::cout << "i1-i2: " << Integer<T>::subtract(i1, i2) << std::endl;
}

void subcheck1()
{
  NonNegativeInteger<limb_t> n1 = 2;
  NonNegativeInteger<limb_t> n2 = 8;
  subcheck(Integer<limb_t>(n1, true), Integer<limb_t>(n2, true));
  subcheck(Integer<limb_t>(n1, true), Integer<limb_t>(n2, false));
  subcheck(Integer<limb_t>(n1, false), Integer<limb_t>(n2, true));
  subcheck(Integer<limb_t>(n1, false), Integer<limb_t>(n2, false));
  subcheck(Integer<limb_t>(n2, true), Integer<limb_t>(n1, true));
  subcheck(Integer<limb_t>(n2, true), Integer<limb_t>(n1, false));
  subcheck(Integer<limb_t>(n2, false), Integer<limb_t>(n1, true));
  subcheck(Integer<limb_t>(n2, false), Integer<limb_t>(n1, false));
}

template <typename T>
void mult(T u, T v)
{
  const int halfbits = boost::integer_traits<T>::digits / 2;
  const T mask = (((T) 1) << halfbits) - 1;
  const T u1 = u >> halfbits;
  const T u0 = u & mask;
  const T v1 = v >> halfbits;
  const T v0 = v & mask;
  std::cout << "u: " << u << std::endl;
  std::cout << "v: " << v << std::endl;
  T x, wl, wh;

  x = u0*v0;
  wl = x & mask;
  x = u1*v0 + (x >> halfbits);
  wh = x >> halfbits;
  x = u0*v1 + (x & mask);
  wl |= (x & mask) << halfbits;
  wh += u1*v1 + (x >> halfbits);

  std::cout << "wh: " << wh << std::endl;
  std::cout << "wl: " << wl << std::endl;
}

// (c,w) = u*v + w + k
template <typename T>
T multadd(const T u, const T v, T& w, const T k)
{
  const int halfbits = boost::integer_traits<T>::digits / 2;
  const T lowmask = (((T) 1) << halfbits) - 1;
  const T u1 = u >> halfbits;
  const T u0 = u & lowmask;
  const T v1 = v >> halfbits;
  const T v0 = v & lowmask;
  T x, y, w0, w2;

  x = w + k;
  y = u0*v0 + (x & lowmask);
  w0 = y & lowmask;
  y = u1*v0 + (x >> halfbits) + (y >> halfbits);
  w2 = y >> halfbits;
  if (x < k) ++w2;
  y = u0*v1 + (y & lowmask);

  w = ((y & lowmask) << halfbits) | w0;
  return u1*v1 + w2 + (y >> halfbits);
}

template <typename T>
void multaddcheck1(T u, T v, T w, T k)
{
  std::cout << "u: " << u << std::endl;
  std::cout << "v: " << v << std::endl;
  std::cout << "w: " << w << std::endl;
  std::cout << "k: " << k << std::endl;

  unsigned long exp = (unsigned long) u * v + w + k;

  k = multadd(u, v, w, k);

  std::cout << "w: " << w << std::endl;
  std::cout << "k: " << k << std::endl;

  unsigned long act = (unsigned long) 65536 * k + w;

  std::cout << (exp == act ? "Ok" : "Fail") << std::endl;
}

void multaddcheck()
{
  limb_t m = boost::integer_traits<limb_t>::const_max;
  limb_t z = 0;
  multaddcheck1((limb_t) (m/5), (limb_t) (m/3), (limb_t) (m/50), (limb_t) (m/30));
  multaddcheck1(m, m, m, m);
  multaddcheck1(m, m, z, m);
  multaddcheck1(m, m, m, z);
}*/

NNI construct(const std::string& digits, const limb_t base)
{
  NNI r = NNI::zero;
  if (base >= 2 && base <= 36) {
    NNI b(base);
    unsigned short v;
    for (std::string::const_iterator p=digits.begin(); p != digits.end(); ++p) {
      char c = *p;
      if (c >= '0' && c <= '9') v = c - '0';
      else if (c >= 'a' && c <= 'z') v = c - 'a' + 10;
      else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
      else break;
      r = b*r + NNI(v);
    }
  }
  return r;
}

void check()
{
  NNI u = construct("200000000", (limb_t) 10);
  NNI v = construct("500000000", (limb_t) 10);

  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
  v.binary_shift_self(10);
  std::cout << "v  : " << v << std::endl;
  v.binary_shift_self(-30);
  std::cout << "v  : " << v << std::endl;
}

void check2()
{
  //NNI u = construct("99999999", (limb_t) 10);
  //NNI v = construct("999", (limb_t) 10);
  NNI u(2);  // 2^1
  NNI v(1);

  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
  u *= u;  // 2^2
  u *= u;  // 2^4
  u *= u;  // 2^8
  u *= u;  // 2^16
  u *= u;  // 2^32
  u *= u;  // 2^64
  u *= u;  // 2^128
  u *= u;  // 2^256
  u *= u;  // 2^512
  u *= u;  // 2^1024
  v = v.binary_shift(1024);
  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
}

void check3()
{
  NNI v(20000);
  NNI u = NNI(19999)*NNI(19999);

  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
  std::pair<NNI,NNI> div = u.divide(u, v);
  std::cout << "u/v: " << div.first << std::endl;
  std::cout << "u%v: " << div.second << std::endl;
}

int main()
{
  check();

  /*NonNegativeInteger<simplevec<limb_t> > u(60000);
  NonNegativeInteger<simplevec<limb_t> > v(18345);

  u += u;

  std::cout << u << std::endl;
  std::cout << v << std::endl;
  u = u - v;
  std::cout << u << std::endl;
  u -= v;
  std::cout << u << std::endl;
  u -= v;
  std::cout << u << std::endl;
  u -= u;
  std::cout << u << std::endl;*/
}
