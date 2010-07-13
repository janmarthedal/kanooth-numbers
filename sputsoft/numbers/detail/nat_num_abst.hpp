/* 
 * File:   sputsoft/numbers/detail/nat_num_abst.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-02 7:55Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_ABST_HPP
#define _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_ABST_HPP

#include <sputsoft/numbers/detail/number_abst.hpp>

#include "default_ops.hpp"

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T>
struct resolve_binary<ops::binary::rem, numb<natnum<T> >, unsigned short> {
  typedef unsigned short return_type;
};
template <typename T>
struct resolve_binary<ops::binary::sub, unsigned short, numb<natnum<T> > > {
  typedef unsigned short return_type;
};

template <typename T>
struct resolve_binary<ops::binary::rem, numb<natnum<T> >, unsigned> {
  typedef unsigned return_type;
};

template <typename T>
struct resolve_binary<ops::binary::rem, numb<natnum<T> >, unsigned long> {
  typedef unsigned long return_type;
};
template <typename T>
struct resolve_binary<ops::binary::sub, unsigned long, numb<natnum<T> > > {
  typedef unsigned long return_type;
};

#ifdef SPUTSOFT_HAS_LONG_LONG
template <typename T>
struct resolve_binary<ops::binary::rem, numb<natnum<T> >, unsigned long long> {
  typedef unsigned long long return_type;
};
template <typename T>
struct resolve_binary<ops::binary::sub, unsigned long long, numb<natnum<T> > > {
  typedef unsigned long long return_type;
};
#endif

/* Make compatible with int literals */
template <typename T>
struct resolve_binary<ops::binary::rem, numb<natnum<T> >, int> {
  typedef unsigned return_type;
};

template <typename T>
struct evaluator_rv<ops::unary::identity, numb<natnum<T> >, int> {
  void operator()(numb<natnum<T> >& r, int v) const {
    sputsoft::numbers::set(r, (unsigned) v);
  }
};
template <typename T>
struct evaluator_rvv<ops::binary::add, numb<natnum<T> >, numb<natnum<T> >, int> {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, int v2) const {
    if (v2 >= 0)
      sputsoft::numbers::add(r, v1, (unsigned) v2);
    else
      sputsoft::numbers::sub(r, v1, (unsigned) -v2);
  }
};
template <typename T>
struct evaluator_rvv<ops::binary::add, numb<natnum<T> >, int, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, int v1, const numb<natnum<T> >& v2) const {
    if (v1 >= 0)
      sputsoft::numbers::add(r, v2, (unsigned) v1);
    else
      sputsoft::numbers::sub(r, v2, (unsigned) -v1);
  }
};
template <typename T>
struct evaluator_rvv<ops::binary::mul, numb<natnum<T> >, numb<natnum<T> >, int> {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, int v2) const {
    sputsoft::numbers::mul(r, v1, (unsigned) v2);
  }
};
template <typename T>
struct evaluator_rvv<ops::binary::mul, numb<natnum<T> >, int, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, int v1, const numb<natnum<T> >& v2) const {
    sputsoft::numbers::mul(r, (unsigned) v1, v2);
  }
};
template <typename T>
struct evaluator_rvv<ops::binary::div, numb<natnum<T> >, numb<natnum<T> >, int> {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, int v2) const {
    sputsoft::numbers::div(r, v1, (unsigned) v2);
  }
};
template <typename T>
struct evaluator_rvv<ops::binary::rem, unsigned, numb<natnum<T> >, int> {
  void operator()(unsigned& r, const numb<natnum<T> >& v1, int v2) const {
    sputsoft::numbers::rem(r, v1, (unsigned) v2);
  }
};
template <typename T>
struct evaluator_rrvv<ops::binary::quotrem, numb<natnum<T> >, unsigned, numb<natnum<T> >, int> {
  void operator()(numb<natnum<T> >& q, unsigned& r, const numb<natnum<T> >& v1, int v2) const {
    sputsoft::numbers::quotrem(q, r, v1, (unsigned) v2);
  }
};
template <typename T>
struct cmp_r2_eval<numb<natnum<T> >, int> {
  static inline int cmp(const numb<natnum<T> >& v1, int v2) {
    return sputsoft::numbers::compare(v1, (unsigned) v2);
  }
};
template <typename T>
struct cmp_r2_eval<int, numb<natnum<T> > > {
  static inline int cmp(int v1, const numb<natnum<T> >& v2) {
    return sputsoft::numbers::compare((unsigned) v1, v2);
  }
};
/*****************************************/

/* Unary abs */

template <typename T>
struct function_v<ops::unary::abs, numb<natnum<T> > > {
  numb<natnum<T> > operator()(const numb<natnum<T> >& v) const {
    return v;
  }
};

/* Binary sub */

template <typename T>
struct resolve_binary<ops::binary::sub, unsigned, numb<natnum<T> > > {
  typedef unsigned return_type;
};

template <typename T>
struct function_vv<ops::binary::sub, unsigned, numb<natnum<T> > > {
  unsigned operator()(unsigned v1, const numb<natnum<T> >& v2) const {
    return numb<natnum<T> >::subi(v1, v2);
  }
};

template <typename T>
struct resolve_binary<ops::binary::sub, int, numb<natnum<T> > > {
  typedef unsigned return_type;
};

template <typename T>
struct evaluator_rvv<ops::binary::sub, numb<natnum<T> >, numb<natnum<T> >, int> {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, int v2) const {
    if (v2 >= 0)
      sputsoft::numbers::sub(r, v1, (unsigned) v2);
    else
      sputsoft::numbers::add(r, v1, (unsigned) -v2);
  }
};
template <typename T>
struct evaluator_rvv<ops::binary::sub, numb<natnum<T> >, int, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, int v1, const numb<natnum<T> >& v2) const {
    sputsoft::numbers::sub(r, (unsigned) v1, v2);
  }
};
template <typename T>
struct function_vv<ops::binary::sub, int, numb<natnum<T> > > {
  typedef typename resolve_binary<ops::binary::sub, unsigned, numb<natnum<T> > >::return_type return_type;
  return_type sub(int v1, const numb<natnum<T> >& v2) {
    return sputsoft::numbers::sub((unsigned) v1, v2);
  }
};

/* bitwise_and */

template <typename T>
struct resolve_binary<ops::binary::bit_and, numb<natnum<T> >, int> {
  typedef unsigned return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and, int, numb<natnum<T> > > {
  typedef unsigned return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and, numb<natnum<T> >, unsigned short> {
  typedef unsigned short return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and, unsigned short, numb<natnum<T> > > {
  typedef unsigned short return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and, numb<natnum<T> >, unsigned> {
  typedef unsigned return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and, unsigned, numb<natnum<T> > > {
  typedef unsigned return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and, numb<natnum<T> >, unsigned long> {
  typedef unsigned long return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and, unsigned long, numb<natnum<T> > > {
  typedef unsigned long return_type;
};
#ifdef SPUTSOFT_HAS_LONG_LONG
template <typename T>
struct resolve_binary<ops::binary::bit_and, numb<natnum<T> >, unsigned long long> {
  typedef unsigned long long return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and, unsigned long long, numb<natnum<T> > > {
  typedef unsigned long long return_type;
};
#endif

template <typename T>
struct evaluator_rvv<ops::binary::bit_and, numb<natnum<T> >, numb<natnum<T> >, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, const numb<natnum<T> >& v2) const {
    r.bitwise_and(v1, v2);
  }
};

template <typename T>
struct function_vv<ops::binary::bit_and, numb<natnum<T> >, unsigned> {
  unsigned operator()(const numb<natnum<T> >& v1, unsigned v2) const {
    return numb<natnum<T> >::bitwise_and_int(v1, v2);
  }
};

template <typename T>
struct function_vv<ops::binary::bit_and, unsigned, numb<natnum<T> > > {
  unsigned operator()(unsigned v1, const numb<natnum<T> >& v2) const {
    return numb<natnum<T> >::bitwise_and_int(v2, v1);
  }
};

template <typename T>
struct function_vv<ops::binary::bit_and, int, numb<natnum<T> > > {
  unsigned operator()(int v1, const numb<natnum<T> >& v2) const {
    return sputsoft::numbers::bitwise_and((unsigned) v1, v2);
  }
};

template <typename T>
struct function_vv<ops::binary::bit_and, numb<natnum<T> >, int> {
  unsigned operator()(const numb<natnum<T> >& v1, int v2) const {
    return sputsoft::numbers::bitwise_and(v1, (unsigned) v2);
  }
};

/* bitwise_or */

template <typename T>
struct evaluator_rvv<ops::binary::bit_or, numb<natnum<T> >, numb<natnum<T> >, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, const numb<natnum<T> >& v2) const {
    r.bitwise_or(v1, v2);
  }
};

template <typename T>
struct evaluator_rvv<ops::binary::bit_or, numb<natnum<T> >, numb<natnum<T> >, unsigned> {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, unsigned v2) const {
    r.bitwise_or_int(v1, v2);
  }
};

template <typename T>
struct evaluator_rvv<ops::binary::bit_or, numb<natnum<T> >, unsigned, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, unsigned v1, const numb<natnum<T> >& v2) const {
    r.bitwise_or_int(v2, v1);
  }
};

/* bitwise_xor */

template <typename T>
struct evaluator_rvv<ops::binary::bit_xor, numb<natnum<T> >, numb<natnum<T> >, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, const numb<natnum<T> >& v2) const {
    r.bitwise_xor(v1, v2);
  }
};

template <typename T>
struct evaluator_rvv<ops::binary::bit_xor, numb<natnum<T> >, numb<natnum<T> >, unsigned> {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, unsigned v2) const {
    r.bitwise_or_int(v1, v2);
  }
};

template <typename T>
struct evaluator_rvv<ops::binary::bit_xor, numb<natnum<T> >, unsigned, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, unsigned v1, const numb<natnum<T> >& v2) const {
    r.bitwise_xor_int(v2, v1);
  }
};

/* bitwise_and_not */

template <typename T>
struct resolve_binary<ops::binary::bit_and_not, int, numb<natnum<T> > > {
  typedef unsigned return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and_not, unsigned short, numb<natnum<T> > > {
  typedef unsigned short return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and_not, unsigned, numb<natnum<T> > > {
  typedef unsigned return_type;
};
template <typename T>
struct resolve_binary<ops::binary::bit_and_not, unsigned long, numb<natnum<T> > > {
  typedef unsigned long return_type;
};
#ifdef SPUTSOFT_HAS_LONG_LONG
template <typename T>
struct resolve_binary<ops::binary::bit_and_not, unsigned long long, numb<natnum<T> > > {
  typedef unsigned long long return_type;
};
#endif

template <typename T>
struct evaluator_rvv<ops::binary::bit_and_not, numb<natnum<T> >, numb<natnum<T> >, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, const numb<natnum<T> >& v2) const {
    r.bitwise_and_not(v1, v2);
  }
};

template <typename T>
struct evaluator_rvv<ops::binary::bit_and_not, numb<natnum<T> >, numb<natnum<T> >, unsigned> {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, unsigned v2) const {
    r.bitwise_and_not_ni(v1, v2);
  }
};

template <typename T>
struct function_vv<ops::binary::bit_and_not, unsigned, numb<natnum<T> > > {
  unsigned operator()(unsigned v1, const numb<natnum<T> >& v2) const {
    return numb<natnum<T> >::bitwise_and_not_in(v1, v2);
  }
};

template <typename T>
struct evaluator_rvv<ops::binary::bit_and_not, numb<natnum<T> >, numb<natnum<T> >, int> {
  void operator()(numb<natnum<T> >& r, const numb<natnum<T> >& v1, int v2) const {
    sputsoft::numbers::bitwise_and_not(v1, (unsigned) v2);
  }
};

template <typename T>
struct function_vv<ops::binary::bit_and_not, int, numb<natnum<T> > > {
  unsigned operator()(int v1, const numb<natnum<T> >& v2) const {
    return sputsoft::numbers::bitwise_and_not((unsigned) v1, v2);
  }
};

/* */

template <typename T>
struct evaluator_rrvv<ops::binary::quotrem, numb<natnum<T> >, numb<natnum<T> >, numb<natnum<T> >, numb<natnum<T> > > {
  void operator()(numb<natnum<T> >& q, numb<natnum<T> >& r, const numb<natnum<T> >& v1, const numb<natnum<T> >& v2) const {
    numb<natnum<T> >::quotrem(q, r, v1, v2);
  }
};

template <typename T>
struct function_rvv<ops::binary::quotrem, numb<natnum<T> >, numb<natnum<T> >, unsigned> {
  typedef typename resolve_binary<ops::binary::rem, numb<natnum<T> >, unsigned>::return_type return_type;
  return_type operator()(numb<natnum<T> >& q, const numb<natnum<T> >& v1, unsigned v2) const {
    return q.quotrem_int(v1, v2);
  }
};

template <typename T>
struct is_positive_r1_eval<numb<natnum<T> > > {
  static inline bool is_positive(const numb<natnum<T> >& v) {
    return !sputsoft::numbers::is_zero(v);
  }
};

template <typename T>
struct is_negative_r1_eval<numb<natnum<T> > > {
  static inline bool is_negative(const numb<natnum<T> >& v) {
    return false;
  }
};

template <typename T>
struct log2_floor_evaluator<numb<natnum<T> > > {
  static std::size_t log2_floor(const numb<natnum<T> >& n) {
    return n.log2_floor();
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const numb<natnum<T> >& n)
{
  unsigned max_digits = n ? log2_floor(n) / 3 + 2 : 2;
  char st[max_digits];
  char* p = st + max_digits;
  *--p = 0;
  numb<natnum<T> > t = n;
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

#endif // _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_ABST_HPP
