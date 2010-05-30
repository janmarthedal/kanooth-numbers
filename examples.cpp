/*
 * File:   examples.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-04 18:02Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#include <iostream>
#include <vector>
//#include <boost/cstdint.hpp>
//#include <boost/integer_traits.hpp>

#include <sputsoft/numbers/natural_number.hpp>
#include <sputsoft/numbers/integer.hpp>
#include <sputsoft/numbers/common_functions.hpp>

using sputsoft::numbers::natural_number;
using sputsoft::numbers::integer;
using sputsoft::numbers::factorize;
using sputsoft::numbers::factorial;
using sputsoft::numbers::power;
using sputsoft::numbers::set;
using sputsoft::numbers::quotrem;


natural_number digit_sum(natural_number n)
{
  natural_number res, q;
  unsigned r;

  while (n) {
    quotrem(q, r, n, 10u);
    res += r;
    n = q;
  }

  return res;
}

// Find largest prime factor of 600851475143
void PE3()
{
  natural_number n;
  std::vector<natural_number> factors;

  n = "600851475143";

  std::cout << "Number: " << n << std::endl;
  factorize(n, std::back_inserter(factors));
  std::cout << "Largest prime factor: " << factors.back() << std::endl << std::endl;
  // Answer: 6857
}

// Add the (decimal) digits of 2^1000
void PE16()
{
  natural_number n = power(natural_number(2), 1000);
  std::cout << "2^1000 = " << n << std::endl;
  std::cout << "Digit sum: " << digit_sum(n) << std::endl << std::endl;
  // Answer: 1366
}

// Add the (decimal) digits of 100!
void PE20()
{
  natural_number n = factorial<natural_number>(100);
  std::cout << "100! = " << n << std::endl;
  std::cout << "Digit sum: " << digit_sum(n) << std::endl << std::endl;
  // Answer: 648
}


void factorize_ex1()
{
  natural_number n = "123456789012345678901234567890";
  std::vector<natural_number> factors;

  std::cout << "Number: " << n << std::endl;
  factorize(n, std::back_inserter(factors));

  std::cout << "Factors:";
  for (std::vector<natural_number>::const_iterator p=factors.begin(); p != factors.end(); ++p)
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
