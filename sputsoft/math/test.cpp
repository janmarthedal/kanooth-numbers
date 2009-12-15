#include <iostream>
#include <sputsoft/math/numbers/natural_number.hpp>

//typedef natural_number num;
typedef ::gmp::natural_number num;

template <typename T>
T test(T x)
{
  T y = x;
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

void run2()
{
  num n = 1u, m = 2u;

  num r = (4u + m) + (n + 3u) + 10u;

  std::cout << r << std::endl;
}

int main()
{
  run2();

  return 0;
}

