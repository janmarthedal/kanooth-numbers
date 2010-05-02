/* 
 * File:   abstract_natural_number.hpp
 * Author: jmr
 *
 * Created on May 2, 2010, 9:55 AM
 */

#ifndef _ABSTRACT_NATURAL_NUMBER_HPP
#define	_ABSTRACT_NATURAL_NUMBER_HPP

#include <sputsoft/numbers/detail/expr.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T>
class natnum;

template <typename T>
struct floor_log2_evaluator<expr<natnum<T> > > {
  static std::size_t floor_log2(const expr<natnum<T> >& n) {
    return n.floor_log2();
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const expr<natnum<T> >& n)
{
  typedef expr<natnum<T> > num_type;
  unsigned max_digits = n ? floor_log2(n) / 3 + 2 : 2;
  char st[max_digits];
  char* p = st + max_digits;
  *--p = 0;
  num_type t = n;
  do {
    *--p = (char) quotrem(t, t, 10u) + '0';
  } while (t);
  return os << p;
}

template <typename T, typename Forw>
struct set_4_eval<expr<natnum<T> >, Forw> {
  static void set(expr<natnum<T> >& n, Forw first, const Forw last, unsigned base) {
    sputsoft::numbers::set(n, 0u);
    if (base >= 2 && base <= 36) {
      unsigned v;
      while (first != last) {
        char c = *first++;
        if (c >= '0' && c <= '9') v = c - '0';
        else if (c >= 'a' && c <= 'z') v = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
        else break;
        sputsoft::numbers::mul(n, n, base);
        sputsoft::numbers::add(n, n, v);
      }
    }
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif	/* _ABSTRACT_NATURAL_NUMBER_HPP */
