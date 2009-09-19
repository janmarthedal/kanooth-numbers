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

#include <nonnegative_integer.hpp>
#include <nni_utils.hpp>
#include <integer.hpp>

using sputsoft::multiprecision::NonNegativeInteger;
using sputsoft::multiprecision::Integer;
using sputsoft::multiprecision::string_to_nni;
using sputsoft::multiprecision::factorial;
using sputsoft::multiprecision::power;

//typedef boost::uint8_t digit_t;
//typedef boost::uint16_t digit_t;
//typedef boost::uint32_t digit_t;
//typedef boost::uint64_t digit_t;
typedef unsigned int digit_t;
typedef NonNegativeInteger<digit_t> NNI;


NNI digit_sum(NNI n)
{
  NNI res = NNI::zero;
  std::pair<NNI, NNI> divrem;

  while (!n.isZero()) {
    divrem = NNI::divide(n, 10);
    res += divrem.second;
    n = divrem.first;
  }

  return res;
}


// Find largest prime factor of 600851475143
void PE3()
{
  NNI n = string_to_nni<NNI>("600851475143");
  std::vector<NNI> factors;

  std::cout << "Number: " << n << std::endl;
  factorize(n, std::back_inserter(factors));
  std::cout << "Largest prime factor: " << factors.back() << std::endl << std::endl;
  // Answer: 6857
}


// Add the (decimal) digits of 2^1000
void PE16()
{
  NNI n = power(NNI(2), 1000);
  std::cout << "2^1000 = " << n << std::endl;
  std::cout << "Digit sum: " << digit_sum(n) << std::endl << std::endl;
  // Answer: 1366
}


// Add the (decimal) digits of 100!
void PE20()
{
  NNI n = factorial<NNI>(100);
  std::cout << "100! = " << n << std::endl;
  std::cout << "Digit sum: " << digit_sum(n) << std::endl << std::endl;
  // Answer: 648
}

void factorize_ex1()
{
  NNI n = string_to_nni<NNI>("123456789012345678901234567890");
  std::vector<NNI> factors;

  std::cout << "Number: " << n << std::endl;
  factorize(n, std::back_inserter(factors));

  std::cout << "Factors:";
  for (std::vector<NNI>::const_iterator p=factors.begin(); p != factors.end(); ++p)
    std::cout << " " << *p;
  std::cout << std::endl;
}


int main()
{
  PE3();
  PE16();
  PE20();
  factorize_ex1();
}
