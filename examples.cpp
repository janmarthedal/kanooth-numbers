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
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/integer_traits.hpp>
#include "NonNegativeInteger.hpp"
#include "NNIutils.hpp"

//using com::sputsoft::multiprecision::NonNegativeInteger;

//typedef boost::uint8_t digit_t;
//typedef boost::uint16_t digit_t;
//typedef boost::uint32_t digit_t;
typedef boost::uint64_t digit_t;
typedef NonNegativeInteger<digit_t> NNI;


void check3()
{
  const unsigned int digitbits = boost::integer_traits<digit_t>::digits;
  const digit_t half = (digit_t) 1 << (digitbits - 1);

  NNI u = NNI(half).binary_shift_this(3*digitbits);
  NNI v = NNI(half).binary_shift_this(2*digitbits) + NNI(1);

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
  NNI u = string_to_nni<NNI>("999");
  NNI v = string_to_nni<NNI>("99999999");
  NNI r = string_to_nni<NNI>("99899999001");

  std::cout << "u  : " << u << std::endl;
  std::cout << "v  : " << v << std::endl;
  std::cout << "u*v: " << u*v << std::endl;
  std::cout << "r  : " << r << std::endl;
}

void PE3()
{
  std::vector<NNI> factors;
  factorize(string_to_nni<NNI>("600851475143"), std::back_inserter(factors));
  std::cout << factors.back() << std::endl;
  // 6857
}

void PE20()
{
  NNI n = factorial<NNI>(100);

  std::cout << n << std::endl;

  NNI res = NNI::zero;
  std::pair<NNI,NNI> divrem;

  while (n != NNI::zero) {
    divrem = NNI::divide(n, 10);
    res += divrem.second;
    n = divrem.first;
  }

  std::cout << res << std::endl;
}

void factorize_test1()
{
  std::vector<NNI> factors;
  factorize(string_to_nni<NNI>("123456789012345678901234567890"), std::back_inserter(factors));
  for (std::vector<NNI>::const_iterator p=factors.begin(); p != factors.end(); ++p)
    std::cout << " " << *p << std::endl;
}

void lgs()
{
  NNI x = string_to_nni<NNI>("123456789012345678901234567890");
  //NNI x = NNI(1).binary_shift(20) + NNI(1);
  size_t floor = x.lg_floor();
  size_t ceil = x.lg_ceil();

  std::cout << x << std::endl;
  std::cout << NNI(1).binary_shift(floor) << ", " << floor << std::endl;
  std::cout << NNI(1).binary_shift(ceil) << ", " << ceil << std::endl;
}

void debug()
{
  NNI u = string_to_nni<NNI>("36009210388420562488862885753");
  NNI v = string_to_nni<NNI>("15097386575707753956");
  std::pair<NNI,NNI> divrem = NNI::divide(u, v);
  NNI z = divrem.first*v + divrem.second;

  std::cout << "u : ";  show_internal(std::cout, u);              std::cout << std::endl;
  std::cout << "v : ";  show_internal(std::cout, v);              std::cout << std::endl;
  std::cout << "q': ";  show_internal(std::cout, divrem.first);   std::cout << std::endl;
  std::cout << "r': ";  show_internal(std::cout, divrem.second);  std::cout << std::endl;
  std::cout << "q : ";  show_internal(std::cout, string_to_nni<NNI>("2385128724"));   std::cout << std::endl;
  std::cout << "r : ";  show_internal(std::cout, string_to_nni<NNI>("9367997898882653609"));  std::cout << std::endl;

  std::cout << (z == u ? "OK" : "Check error") << std::endl;
}


int main()
{
  debug();
}
