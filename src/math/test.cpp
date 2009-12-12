#include <iostream>
//#include <math/numbers/gmp/integer.hpp>
#include <math/numbers/shared.hpp>
#include <math/numbers/number.hpp>
#include <math/numbers/detail/digit_array.hpp>
#include <math/numbers/natural_number.hpp>

//typedef sputsoft::math::numbers::number<sputsoft::math::numbers::gmp::integer> num;
/*typedef sputsoft::math::numbers::number<
  sputsoft::math::numbers::shared<
    sputsoft::math::numbers::gmp::integer> > num;*/
typedef sputsoft::math::numbers::number<
  sputsoft::math::numbers::natural_number<
    sputsoft::math::numbers::detail::digit_array<unsigned>
  > > num;

/*template <typename T>
T test(T x)
{
  T y = x;
  return y;
}

void run1() 
{
  num n = 1, m = 2;
  num r = test(n + m);
  num s = r;
  std::cout << r << std::endl;
  //std::cout << test<num>(n + m) << std::endl;
}*/

int main()
{
  num n = 1;

  std::cout << n << std::endl;

  return 0;
}

