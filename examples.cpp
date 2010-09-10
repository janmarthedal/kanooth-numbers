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

#include <sputsoft/numbers/natural_number.hpp>
#include <sputsoft/numbers/integer.hpp>
#include <sputsoft/numbers/common_functions.hpp>
#include <sputsoft/numbers/number_theory.hpp>
#include <sputsoft/numbers/common_type.hpp>

using namespace sputsoft::numbers;

// Find largest prime factor of 600851475143
void PE3()
{
  std::vector<natural_number> factors;
  natural_number n = "600851475143";
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
  std::cout << "Digit sum: " << sideways_sum(n, 10) << std::endl << std::endl;
  // Answer: 1366
}

// Add the (decimal) digits of 100!
void PE20()
{
  natural_number n = factorial<natural_number>(100);
  std::cout << "100! = " << n << std::endl;
  std::cout << "Digit sum: " << sideways_sum(n, 10) << std::endl << std::endl;
  // Answer: 648
}

void factorize_example()
{
  natural_number n = "123456789012345678901234567890";
  std::vector<natural_number> factors;

  std::cout << "Number: " << n << std::endl;
  factorize(n, std::back_inserter(factors));

  std::cout << "Factors:";
  for (std::vector<natural_number>::const_iterator p=factors.begin(); p != factors.end(); ++p)
    std::cout << " " << *p;
  std::cout << std::endl << std::endl;
}

void gcd_example()
{
  integer a = "123456789012345678901234567890";
  integer b = "-9876543210";
  integer c;
  
  gcd(c, a, b);

  std::cout << "gcd(" << a << "," << b << ") = " << c << std::endl << std::endl;
}

void bits1()
{
  natural_number a, b, c, d;
  unsigned e;

  set(a, "11101000010101101010", 2);
  set(b,   "101000101111000111", 2);
  //set(d, "11101000111111101111", 2);  // a | b
  set(d,   "101000000101000010", 2);  // a & b
  //set(d, "11101000010100111111", 2);  // a ^ 85
  //set(d, "11000000111010101101", 2);  // a ^ b
  //set(d,         "101010000101", 2);  // b & ~a
  //set(d, "11000000010000101000", 2);  // a & ~b

  //bitwise_and_not(c, a, 85u);
  bitwise_and(e, a, ~85u);

  std::cout << a << std::endl;
  std::cout << e << std::endl;
  std::cout << c << std::endl;
}

void bits2()
{
  integer a = -2, b = 7, c;

  bitwise_and(c, a, b);
  std::cout << c << std::endl;
  bitwise_and(c, -5, b);
  std::cout << c << std::endl;
  bitwise_and(c, b, 5);
  std::cout << c << std::endl;
  bitwise_and(c, -2, 7);
  std::cout << c << std::endl;
}

void t(short v) {  std::cout << "short " << v << std::endl; }
void t(unsigned short v) { std::cout << "unsigned short " << v << std::endl; }
void t(int v) {  std::cout << "int " << v << std::endl; }
void t(unsigned int v) { std::cout << "unsigned int " << v << std::endl; }
void t(long v) {  std::cout << "long " << v << std::endl; }
void t(unsigned long v) { std::cout << "unsigned long " << v << std::endl; }
void t(long long v) {  std::cout << "long long " << v << std::endl; }
void t(unsigned long long v) { std::cout << "unsigned long long " << v << std::endl; }
void t(natural_number v) {  std::cout << "natural_number " << v << std::endl; }
void t(integer v) { std::cout << "integer " << v << std::endl; }

int main()
{
  /*PE3();
  PE16();
  PE20();
  factorize_example();*/
  gcd_example();
  /*bits2();
  common_type<natural_number, int>::type a = 1;
  t(a);*/

  return 0;
}
