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
#include <sputsoft/numbers/detail/nat_num_wrap.hpp>


typedef sputsoft::numbers::detail::expr<
          sputsoft::numbers::detail::nat_num_wrap<
            sputsoft::numbers::detail::digit_array<unsigned long>,
            sputsoft::numbers::detail::lowlevel_generic<
              sputsoft::numbers::detail::array_allocator<void>
            >
          >
        > natural_number;


int main()
{
  natural_number a, b=2u, c;

  sputsoft::numbers::set(c, 10u);
  sputsoft::numbers::set(a, b);
  sputsoft::numbers::add(c, b, 3u);
  sputsoft::numbers::add(c, 20u, b);

  std::cout << a.get() << std::endl;
  std::cout << c.get() << std::endl;

  return 0;
}
