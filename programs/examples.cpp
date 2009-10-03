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

#include <natural_number.hpp>
#include <natural_number_algorithms.hpp>

using sputsoft::multiprecision::natural_number;
using sputsoft::multiprecision::string_to_natural_number;
using sputsoft::multiprecision::factorial;
using sputsoft::multiprecision::power;

//typedef boost::uint8_t digit_t;
//typedef boost::uint16_t digit_t;
//typedef boost::uint32_t digit_t;
//typedef boost::uint64_t digit_t;
typedef unsigned int digit_t;
typedef natural_number<digit_t> NUM;


NUM digit_sum(NUM n)
{
  NUM res = NUM::zero;
  std::pair<NUM, NUM> divrem;

  while (!n.isZero()) {
    divrem = NUM::divide(n, 10);
    res += divrem.second;
    n = divrem.first;
  }

  return res;
}


// Find largest prime factor of 600851475143
void PE3()
{
  NUM n = string_to_natural_number<NUM>("600851475143");
  std::vector<NUM> factors;

  //std::cout << "Number: " << n << std::endl;
  factorize(n, std::back_inserter(factors));
  std::cout << "Largest prime factor: " << factors.back() << std::endl << std::endl;
  // Answer: 6857
}


// Add the (decimal) digits of 2^1000
void PE16()
{
  NUM n = power(NUM(2), 1000);
  std::cout << "2^1000 = " << n << std::endl;
  std::cout << "Digit sum: " << digit_sum(n) << std::endl << std::endl;
  // Answer: 1366
}


// Add the (decimal) digits of 100!
void PE20()
{
  NUM n = factorial<NUM>(100);
  std::cout << "100! = " << n << std::endl;
  std::cout << "Digit sum: " << digit_sum(n) << std::endl << std::endl;
  // Answer: 648
}


void factorize_ex1()
{
  NUM n = string_to_natural_number<NUM>("123456789012345678901234567890");
  std::vector<NUM> factors;

  std::cout << "Number: " << n << std::endl;
  factorize(n, std::back_inserter(factors));

  std::cout << "Factors:";
  for (std::vector<NUM>::const_iterator p=factors.begin(); p != factors.end(); ++p)
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
