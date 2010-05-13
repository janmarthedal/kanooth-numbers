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

template <typename T, typename S>
T make_number(const S& s) {
  T n;
  sputsoft::numbers::set(n, s);
  return n;
}

template <typename N, typename T>
void div_int_test(const N& u, const T& v)
{
  N q1, q2, c;
  sputsoft::numbers::div_floor(q1, u, v);
  T r1 = sputsoft::numbers::rem_floor(u, v);
  sputsoft::numbers::mul(c, q1, v);
  sputsoft::numbers::add(c, c, r1);
  std::cout << q1 << " " << r1 << " " << (sputsoft::numbers::equal(u, c) ? "OK" : "FAIL") << std::endl;
  T r2 = sputsoft::numbers::quotrem_floor(q2, u, v);
  if (!sputsoft::numbers::equal(q1, q2)) std::cout << "q1 != q2" << std::endl;
  if (r1 != r2) std::cout << "r1 != r2" << std::endl;
}

template <typename N, typename T>
void div_ceil_int_test(const N& u, const T& v)
{
  N q1, q2, c;
  sputsoft::numbers::div_ceil(q1, u, v);
  T r1 = sputsoft::numbers::rem_ceil(u, v);
  sputsoft::numbers::mul(c, q1, v);
  sputsoft::numbers::add(c, c, r1);
  std::cout << q1 << " " << r1 << " " << (sputsoft::numbers::equal(u, c) ? "OK" : "FAIL") << std::endl;
  T r2 = sputsoft::numbers::quotrem_ceil(q2, u, v);
  if (!sputsoft::numbers::equal(q1, q2)) std::cout << "q1 != q2" << std::endl;
  if (r1 != r2) std::cout << "r1 != r2" << std::endl;
}

template <typename N, typename T>
void div_trunc_int_test(const N& u, const T& v)
{
  N q1, q2, c;
  sputsoft::numbers::div_trunc(q1, u, v);
  T r1 = sputsoft::numbers::rem_trunc(u, v);
  sputsoft::numbers::mul(c, q1, v);
  sputsoft::numbers::add(c, c, r1);
  std::cout << q1 << " " << r1 << " " << (sputsoft::numbers::equal(u, c) ? "OK" : "FAIL") << std::endl;
  T r2 = sputsoft::numbers::quotrem_trunc(q2, u, v);
  if (!sputsoft::numbers::equal(q1, q2)) std::cout << "q1 != q2" << std::endl;
  if (r1 != r2) std::cout << "r1 != r2" << std::endl;
}

template <typename N>
void div_num_test(const N& u, const N& v)
{
  N q1, r1, q2, r2, c;
  sputsoft::numbers::div_floor(q1, u, v);
  sputsoft::numbers::rem_floor(r1, u, v);
  sputsoft::numbers::mul(c, q1, v);
  sputsoft::numbers::add(c, c, r1);
  std::cout << q1 << " " << r1 << " " << (sputsoft::numbers::equal(u, c) ? "OK" : "FAIL") << std::endl;
  sputsoft::numbers::quotrem_floor(q2, r2, u, v);
  if (!sputsoft::numbers::equal(q1, q2)) std::cout << "q1 != q2" << std::endl;
  if (!sputsoft::numbers::equal(r1, r2)) std::cout << "r1 != r2" << std::endl;
}

template <typename N>
void div_ceil_num_test(const N& u, const N& v)
{
  N q1, r1, q2, r2, c;
  sputsoft::numbers::div_ceil(q1, u, v);
  sputsoft::numbers::rem_ceil(r1, u, v);
  sputsoft::numbers::mul(c, q1, v);
  sputsoft::numbers::add(c, c, r1);
  std::cout << q1 << " " << r1 << " " << (sputsoft::numbers::equal(u, c) ? "OK" : "FAIL") << std::endl;
  sputsoft::numbers::quotrem_ceil(q2, r2, u, v);
  if (!sputsoft::numbers::equal(q1, q2)) std::cout << "q1 != q2" << std::endl;
  if (!sputsoft::numbers::equal(r1, r2)) std::cout << "r1 != r2" << std::endl;
}

template <typename N>
void div_trunc_num_test(const N& u, const N& v)
{
  N q1, r1, q2, r2, c;
  sputsoft::numbers::div_trunc(q1, u, v);
  sputsoft::numbers::rem_trunc(r1, u, v);
  sputsoft::numbers::mul(c, q1, v);
  sputsoft::numbers::add(c, c, r1);
  std::cout << q1 << " " << r1 << " " << (sputsoft::numbers::equal(u, c) ? "OK" : "FAIL") << std::endl;
  sputsoft::numbers::quotrem_trunc(q2, r2, u, v);
  if (!sputsoft::numbers::equal(q1, q2)) std::cout << "q1 != q2" << std::endl;
  if (!sputsoft::numbers::equal(r1, r2)) std::cout << "r1 != r2" << std::endl;
}

sputsoft::numbers::integer make_int(int v) {
  return make_number<sputsoft::numbers::integer>(v);
}

template <typename N, typename T>
void test_div_int(const N& u, const T& v)
{
  div_int_test(u, v);
  div_ceil_int_test(u, v);
  div_trunc_int_test(u, v);
}

template <typename N>
void test_div_num(const N& u, const N& v)
{
  div_num_test(u, v);
  div_ceil_num_test(u, v);
  div_trunc_num_test(u, v);
}

void test_int()
{
  test_div_int(make_int(20), 7);
  test_div_int(make_int(-20), 7);
  test_div_int(make_int(20), -7);
  test_div_int(make_int(-20), -7);
  test_div_num(make_int(20), make_int(7));
  test_div_num(make_int(-20), make_int(7));
  test_div_num(make_int(20), make_int(-7));
  test_div_num(make_int(-20), make_int(-7));
}

int main()
{
  test_int();

  return 0;
}
