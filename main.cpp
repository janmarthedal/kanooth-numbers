/*
 * File:   main.cpp
 * Author: jmr
 *
 * Created on December 18, 2009, 10:48 AM
 */

#include <iostream>
#include <boost/integer_traits.hpp>
#include <sputsoft/numbers/detail/digit_array.hpp>
#include <sputsoft/numbers/detail/lowlevel_generic.hpp>

#include <sputsoft/numbers/detail/nat_num_shared.hpp>
typedef sputsoft::numbers::detail::expr<
          sputsoft::numbers::detail::natnum<
            boost::shared_ptr<
              sputsoft::numbers::detail::wrap2<
                sputsoft::numbers::detail::digit_array<unsigned long>,
                sputsoft::numbers::detail::lowlevel_generic<
                  sputsoft::numbers::detail::array_allocator<void>
                >
              >
            >
          >
        > natural_number;

/*#include <sputsoft/numbers/detail/nat_num_impl.hpp>
typedef sputsoft::numbers::detail::expr<
          sputsoft::numbers::detail::natnum<
            sputsoft::numbers::detail::wrap2<
              sputsoft::numbers::detail::digit_array<unsigned long>,
              sputsoft::numbers::detail::lowlevel_generic<
                sputsoft::numbers::detail::array_allocator<void>
              >
            >
          >
        > natural_number;*/


int main()
{
  natural_number a, b=20u, c;
  std::string st = "1734675685675464534533456546";

  //sputsoft::numbers::set(b, st);

  std::cout << b << std::endl;

  sputsoft::numbers::set(c, 10u);
  sputsoft::numbers::set(a, b);
  std::cout << a << std::endl;

  sputsoft::numbers::add(c, b, 3u);
  sputsoft::numbers::sub(c, b, 5u);
  sputsoft::numbers::mul(c, c, c);
  unsigned r = sputsoft::numbers::quotrem(b, c, 7u);
  std::cout << c << std::endl;
  std::cout << b << " " << r << std::endl;

  return 0;
}
