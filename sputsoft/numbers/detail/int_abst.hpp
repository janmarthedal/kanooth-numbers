/* 
 * File:   sputsoft/numbers/detail/int_abst.hpp
 * Author: jmr
 *
 * Created 2010-05-13 11:00
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_INT_ABST_HPP
#define	_SPUTSOFT_NUMBERS_DETAIL_INT_ABST_HPP

#include <sputsoft/numbers/detail/number_abst.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T>
class intnum;

#define RESOLVE_INTNUM_REM(TYPE) \
template <typename T> \
struct resolve_binary<ops::binary::rem, numb<intnum<T> >, TYPE> { typedef TYPE return_type; }; \
template <typename T> \
struct resolve_binary<ops::binary::rem_floor, numb<intnum<T> >, TYPE> { typedef TYPE return_type; }; \
template <typename T> \
struct resolve_binary<ops::binary::rem_ceil, numb<intnum<T> >, TYPE> { typedef TYPE return_type; }; \
template <typename T> \
struct resolve_binary<ops::binary::rem_trunc, numb<intnum<T> >, TYPE> { typedef TYPE return_type; };

RESOLVE_INTNUM_REM(unsigned short)
RESOLVE_INTNUM_REM(signed short)
RESOLVE_INTNUM_REM(unsigned)
RESOLVE_INTNUM_REM(signed)
RESOLVE_INTNUM_REM(unsigned long)
RESOLVE_INTNUM_REM(signed long)
#ifdef SPUTSOFT_HAS_LONG_LONG
RESOLVE_INTNUM_REM(unsigned long long)
RESOLVE_INTNUM_REM(signed long long)
#endif

template <typename NUM>
inline std::ostream& operator<<(std::ostream& os, const numb<intnum<NUM> >& n) {
  if (n.num && !n.positive) os << "-";
  return os << n.num;
}

template <typename NUM, typename Forw>
struct set_4_eval<numb<intnum<NUM> >, Forw> {
  static void set(numb<intnum<NUM> >& n, Forw first, const Forw last, unsigned base) {
    bool pos=true;
    while (first != last) {
      char c = *first;
      if (c == '-') pos = !pos;
      else if (c != '+') break;
      ++first;
    }
    NUM t;
    sputsoft::numbers::set(t, first, last, base);
    n = numb<intnum<NUM> >(t, pos);
  }
};

} // namespace detail
} // namespace sputsoft
} // namespace numbers

#endif // _SPUTSOFT_NUMBERS_DETAIL_INT_ABST_HPP
