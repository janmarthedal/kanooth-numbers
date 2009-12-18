#include <iostream>
#define SPUTSOFT_USE_GMP
#include <sputsoft/math/number_theory/common.hpp>
#include <sputsoft/math/numbers/natural_number.hpp>

//typedef natural_number num;
typedef ::gmp::natural_number num;

template <typename T>
T test(T x, unsigned n)
{
  T y = x;
  y = 1;
  while (n--)
    y += n;
  return y;
}

int main()
{
  num m = 200u;
  num n = m / 13u;

  std::cout << m << std::endl;
  std::cout << n << std::endl;

  return 0;
}

