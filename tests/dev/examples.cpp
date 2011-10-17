/*
 * File:   examples.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-04 18:02Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include <vector>

#include <kanooth/numbers/natural_number.hpp>
#include <kanooth/numbers/integer.hpp>
#include <kanooth/numbers/rational_number.hpp>
#include <kanooth/numbers/positive_float.hpp>
#include <kanooth/numbers/floating_point.hpp>
#include <kanooth/numbers/common_functions.hpp>
#include <kanooth/numbers/number_theory.hpp>

using namespace kanooth::numbers;

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
  natural_number n = power(natural_number(2u), 1000);
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
  natural_number a = "123456789012345678901234567890";
  natural_number b = "9876543210";
  //long b = -9876543210L;
  natural_number c;

  c = gcd(a, b);

  std::cout << "gcd(" << a << "," << b << ") = " << c << std::endl << std::endl;
}

void sqrt_example()
{
  natural_number x = mul_power(natural_number(2u), 10u, 80);
  natural_number y = integer_sqrt(x);

  std::cout << "sqrt(" << x << ") = " << y << " " << mul(y, y) << " < " << mul(add(y, 1u), add(y, 1u)) << std::endl;
}

void sqrt_example2()
{
  positive_float f = 2u, g;

  g = sqrt(f);
  std::cout << "sqrt: " << g << std::endl;

  double d = sqrt(2.0d);
  std::cout << "sqrt: " << d << std::endl;
}

void show_number(int x) { std::cout << "int " << x << std::endl; }
void show_number(unsigned x) { std::cout << "uint " << x << std::endl; }
void show_number(double x) { std::cout << "double " << x << std::endl; }

void div_example_help(int a, int b)
{
  show_number(ceil_div(a, b));
  double c=a, d=b;
  show_number(ceil_div(c, d));
}

void div_example()
{
  div_example_help(7, 3);
  div_example_help(-7, 3);
  div_example_help(7, -3);
  div_example_help(-7, -3);
  div_example_help(6, 3);
  div_example_help(-6, 3);
  div_example_help(6, -3);
  div_example_help(-6, -3);
  show_number(ceil_div(13u, 4u));
  show_number(ceil_div(13, 4));
}

void rem_example_help(int a, int b)
{
  show_number(ceil_rem(a, b));
  double c=a, d=b;
  show_number(ceil_rem(c, d));
}

void rem_example()
{
  rem_example_help(7, 3);
  rem_example_help(-7, 3);
  rem_example_help(7, -3);
  rem_example_help(-7, -3);
  rem_example_help(6, 3);
  rem_example_help(-6, 3);
  rem_example_help(6, -3);
  rem_example_help(-6, -3);
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

/*void bits2()
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
}*/


void t(short v) {  std::cout << "short " << v << std::endl; }
void t(unsigned short v) { std::cout << "unsigned short " << v << std::endl; }
void t(int v) {  std::cout << "int " << v << std::endl; }
void t(unsigned int v) { std::cout << "unsigned int " << v << std::endl; }
void t(long v) {  std::cout << "long " << v << std::endl; }
void t(unsigned long v) { std::cout << "unsigned long " << v << std::endl; }
void t(long long v) {  std::cout << "long long " << v << std::endl; }
void t(unsigned long long v) { std::cout << "unsigned long long " << v << std::endl; }
//void t(natural_number v) {  std::cout << "natural_number " << v << std::endl; }
//void t(integer v) { std::cout << "integer " << v << std::endl; }


template <typename T>
struct retval {};

template <>
struct retval<int> {
  typedef void type;
};

template <typename T>
typename retval<T>::type testfunc(T v) {
  std::cout << v << std::endl;
}


void check_rounding2(natural_number n)
{
  positive_float f;

  f.set_precision(64);
  div(f, n, 8u);
  show_binary(std::cout, f);
  std::cout << std::endl;

  f.set_precision(32);
  show_binary(std::cout, f);
  std::cout << std::endl;
}

void check_rounding()
{
  natural_number b, n;

  b = mul_power(natural_number(1u), 2u, 34);
  for (unsigned k=1; k < 16; ++k) {
    add(n, b, k);
    check_rounding2(n);
  }
}

/*void check_rational()
{
  rational_number r = 2, s = -4, t;
  std::cout << r << std::endl;
  std::cout << s << std::endl;
  div(t, r, s);
  std::cout << t << std::endl;
  sub(t, t, 4);
  std::cout << t << std::endl;
  div(t, 3, 7);
  std::cout << t << std::endl;
  add(t, t, 2);
  std::cout << t << std::endl;
}*/

void check_integer()
{
  integer a;
  natural_number n = 2u;
  set(a, n);
  std::cout << a << std::endl;
  negate(a, n);
  std::cout << a << std::endl;
  abs(n, a);
  std::cout << n << std::endl;
  sub(a, a, n);
  std::cout << a << std::endl;
}


#include <limits>

int main()
{
  std::cout << std::numeric_limits<float>::digits << std::endl;
  std::cout << std::numeric_limits<double>::digits << std::endl;
  std::cout << std::numeric_limits<long double>::digits << std::endl;
  
  check_integer();

  floating_point f = 2, g = 3;
  //positive_float f = 2u, g = 3u;
  f.set_rounding_mode(CEIL);
  add(f, f, g);
  std::cout << f << std::endl;

  return 0;
}
