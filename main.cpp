/*
 * File:   main.cpp
 * Author: jmr
 *
 * Created 2009-12-18 8:48Z
 */

#include <iostream>
#include <sputsoft/numbers/natural_number.hpp>
#include <sputsoft/numbers/integer.hpp>
#include <sputsoft/numbers/detail/overload.hpp>

void test_natnum()
{
  sputsoft::numbers::natural_number a, b=20u, c;
  std::string st = "1734675685675464534533456546";

  sputsoft::numbers::set(b, st);

  std::cout << b << std::endl;

  a = 100u;

  sputsoft::numbers::set(c, 10u);
  sputsoft::numbers::set(a, b);
  std::cout << "a: " << a << std::endl;

  sputsoft::numbers::add(c, b, 3u);
  sputsoft::numbers::sub(c, b, 5u);
  sputsoft::numbers::mul(c, c, c);
  unsigned r = sputsoft::numbers::quotrem(b, c, 5u);
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
  test_div_int(make_int(21), 7);
  test_div_int(make_int(-21), 7);
  test_div_int(make_int(21), -7);
  test_div_int(make_int(-21), -7);
  test_div_num(make_int(21), make_int(7));
  test_div_num(make_int(-21), make_int(7));
  test_div_num(make_int(21), make_int(-7));
  test_div_num(make_int(-21), make_int(-7));
}

void test_int2()
{
  sputsoft::numbers::integer a, b;

  sputsoft::numbers::set(a, "1734675685675464534533456546");
  std::cout << a << std::endl;

  a = 10;

  sputsoft::numbers::negate(b, a);
  std::cout << b << std::endl;
}

using namespace sputsoft::numbers;

void test_natural_number_named_ops()
{
  unsigned short usi = 1;
  unsigned       ui  = 2;
  unsigned long  uli = 3;

  natural_number n1;          // n1 = 0
  natural_number n2 = usi;    // n2 = 1
  natural_number n3 = ui;     // n3 = 2
  natural_number n4 = uli;    // n4 = 3
  natural_number n5 = n2;     // n5 = 1
  natural_number n6(n3);      // n6 = 2

  n1 = n4;                    // n1 = 3
  set(n5, n4);                // n5 = 3
  set(n5, usi);               // n5 = 1
  set(n5, ui);                // n5 = 2
  set(n5, uli);               // n5 = 3

  add(n2, n1, n3);            // n2 = 3 + 2 = 5
  add(n5, n2, usi);           // n5 = 5 + 1 = 6
  add(n6, usi, n5);           // n6 = 1 + 6 = 7
  add(n5, n6, ui);            // n5 = 7 + 2 = 9
  add(n6, ui, n5);            // n6 = 2 + 9 = 11
  add(n5, n6, uli);           // n5 = 11 + 3 = 14
  add(n6, uli, n5);           // n6 = 3 + 14 = 17

  n5 = add(n5, n6);           // n5 = 14 + 17 = 31
  n6 = add(n5, usi);          // n6 = 31 + 1 = 32
  n5 = add(usi, n6);          // n5 = 1 + 32 = 33
  n6 = add(n5, ui);           // n6 = 33 + 2 = 35
  n5 = add(ui, n6);           // n5 = 2 + 35 = 37
  n6 = add(n5, uli);          // n6 = 37 + 3 = 40
  n5 = add(uli, n6);          // n5 = 3 + 40 = 43

  sub(n6, n5, n1);            // n6 = 43 - 3 = 40
  sub(n5, n6, usi);           // n5 = 40 - 1 = 39
  sub(n6, n5, ui);            // n6 = 39 - 2 = 37
  sub(n5, n6, uli);           // n5 = 37 - 3 = 34

  n6 = sub(n5, n1);           // n6 = 34 - 3 = 31
  n5 = sub(n6, usi);          // n5 = 31 - 1 = 30
  n6 = sub(n5, ui);           // n6 = 30 - 2 = 28
  n5 = sub(n6, uli);          // n5 = 28 - 3 = 25

  std::cout << n5 << std::endl;
}

template <typename T>
T test_f(const T& n)
{
  return n + 4;
}

void overload()
{
  sputsoft::numbers::integer a, b=2u, c=123u, q, r;
  a = (b + c) + (b + c);
  r = (a + c) % b;
  r = test_f(r);
  std::cout << r << std::endl;
  quotrem(q, r, a + c, b);
  std::cout << q << " " << r << std::endl;
}

int main()
{
  //test_natnum();
  //test_int();
  //test_natural_number_named_ops();
  overload();

  return 0;
}
