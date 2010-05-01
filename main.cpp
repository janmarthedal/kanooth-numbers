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

/*#include <sputsoft/numbers/detail/shared_nat_num_wrap.hpp>
typedef sputsoft::numbers::detail::expr<
          boost::shared_ptr<
            sputsoft::numbers::detail::nat_num_wrap<
              sputsoft::numbers::detail::digit_array<unsigned long>,
              sputsoft::numbers::detail::lowlevel_generic<
                sputsoft::numbers::detail::array_allocator<void>
              >
            >
          >
        > natural_number;*/

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
  natural_number a, b=20u, c;

  sputsoft::numbers::set(c, 10u);
  sputsoft::numbers::set(a, b);
  sputsoft::numbers::add(c, b, 3u);
  sputsoft::numbers::sub(c, b, 5u);
  sputsoft::numbers::mul(c, c, b);

  std::cout << a.get() << std::endl;
  std::cout << c.get() << std::endl;

  return 0;
}
