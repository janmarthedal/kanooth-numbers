/* 
 * File:   abstract_natural_number.hpp
 * Author: jmr
 *
 * Created on May 2, 2010, 9:55 AM
 *
 * $Id$
 */

#ifndef _ABSTRACT_NATURAL_NUMBER_HPP
#define	_ABSTRACT_NATURAL_NUMBER_HPP

#include <sputsoft/numbers/detail/number_abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T>
class natnum;

template <typename T>
struct resolve_binary<ops::binary::remainder, numb<natnum<T> >, unsigned short> {
  typedef unsigned short return_type;
};

template <typename T>
struct resolve_binary<ops::binary::remainder, numb<natnum<T> >, unsigned> {
  typedef unsigned return_type;
};

template <typename T>
struct resolve_binary<ops::binary::remainder, numb<natnum<T> >, unsigned long> {
  typedef unsigned long return_type;
};

#ifdef SPUTSOFT_HAS_LONG_LONG
template <typename T>
struct resolve_binary<ops::binary::remainder, numb<natnum<T> >, unsigned long long> {
  typedef unsigned long long return_type;
};
#endif

template <typename T>
struct log2_floor_evaluator<numb<natnum<T> > > {
  static std::size_t log2_floor(const numb<natnum<T> >& n) {
    return n.log2_floor();
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const numb<natnum<T> >& n)
{
  typedef numb<natnum<T> > num_type;
  unsigned max_digits = n ? log2_floor(n) / 3 + 2 : 2;
  char st[max_digits];
  char* p = st + max_digits;
  *--p = 0;
  num_type t = n;
  do {
    unsigned r = quotrem(t, t, 10u);
    *--p = (char) r + '0';
  } while (t);
  return os << p;
}

template <typename T, typename Forw>
struct set_4_eval<numb<natnum<T> >, Forw> {
  static void set(numb<natnum<T> >& n, Forw first, const Forw last, unsigned base) {
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
