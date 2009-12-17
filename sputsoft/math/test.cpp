#include <iostream>
#define SPUTSOFT_USE_GMP
#include <sputsoft/math/numbers/common.hpp>
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

/*void run1() 
{
  num n = 1, m = 2;
  num r = test(n + m);
  num s = r;
  std::cout << r << std::endl;
  //std::cout << test<num>(n + m) << std::endl;
}*/

/*void run3()
{
  num n = "x", m = "y";
  num r = test<num>(n + m);
  num s = -(-(r - (m + n)));
  std::cout << s << std::endl;
}*/

/*template <typename U, typename V>
inline V remainder(const U& u, const V& v)
{
  return u % v;
}*/

/*template <typename ML>
unsigned remainder(const ::detail::natural_number<ML>& u, unsigned v)
{
  return 123u;
}*/

/*unsigned operator%(const num& u, unsigned v)
{
  return remainder(u, v);
}*/

void run2()
{
  //num n = 1u;
  num m = 200u;
  num n = test(m, 123);

  /*num r = (4u + m) + (n + 3u) + 10000u;
  std::cout << r << std::endl;*/

  unsigned a = m % 21u;

  std::cout << m << std::endl;
  std::cout << n << std::endl;
  std::cout << a << std::endl;
}

int main()
{
  run2();

  /*std::pair<unsigned, unsigned> qr = divrem(10u, 3u);

  std::cout << qr.first << std::endl;
  std::cout << qr.second << std::endl;*/

  return 0;
}

