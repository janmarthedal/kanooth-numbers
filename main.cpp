/*
 * File:   main.cpp
 * Author: jmr
 *
 * Created on December 18, 2009, 10:48 AM
 */

#include <iostream>
#include <sputsoft/numbers/natural_number.hpp>

#include "sputsoft/numbers/integer.hpp"

void test_natnum()
{
  sputsoft::numbers::natural_number a, b=20u, c;
  std::string st = "1734675685675464534533456546";

  sputsoft::numbers::set(b, st);

  std::cout << b << std::endl;

  sputsoft::numbers::set(c, 10u);
  sputsoft::numbers::set(a, b);
  std::cout << "a: " << a << std::endl;

  sputsoft::numbers::add(c, b, 3u);
  sputsoft::numbers::sub(c, b, 5u);
  sputsoft::numbers::mul(c, c, c);
  unsigned r = sputsoft::numbers::quotrem(b, c, 7u);
  std::cout << "c: " << c << std::endl;
  std::cout << "b: " << b << " r: " << r << std::endl;
  std::cout << sputsoft::numbers::rem(b, 100u) << std::endl;

  std::cout << sputsoft::numbers::compare(a, b) << std::endl;
}

void test_int()
{
  sputsoft::numbers::integer a;
  std::string st = "-123";

  sputsoft::numbers::set(a, st);

  std::cout << a << std::endl;
}

int main()
{
  test_int();

  return 0;
}
