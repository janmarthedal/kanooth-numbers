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

#include <sputsoft/numbers/common_type.hpp>
#include <sputsoft/numbers/detail/number_abst.hpp>
#include <sputsoft/numbers/detail/default_ops.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T> class natnum;

} // namespace detail
} // namespace numbers

// Type traits related to natural numbers numb<natnum<T> >

template <typename T>
struct is_signed<numbers::detail::numb<numbers::detail::natnum<T> > > : public false_type {};

template <typename T>
struct make_unsigned<numbers::detail::numb<numbers::detail::natnum<T> > > {
  typedef numbers::detail::numb<numbers::detail::natnum<T> > type;
};

template <typename T>
struct is_integral<numbers::detail::numb<numbers::detail::natnum<T> > > : public true_type {};


namespace numbers {
namespace detail {

template <typename T>
struct type_rank<numb<natnum<T> > > : public integral_constant<int, 20> {};

#ifdef SPUTSOFT_NATURAL_NUMBER_WITH_INTS

template <typename T>
struct enabler_rv<ops::unary::identity, numb<natnum<T> >, int>
  : public enabler_rv<ops::unary::identity, numb<natnum<T> >, unsigned> {};

  
template <typename Op, typename T>
struct binary_result<Op, numb<natnum<T> >, int>
  : public binary_result<Op, numb<natnum<T> >, unsigned> {};

template <typename Op, typename T>
struct binary_result<Op, int, numb<natnum<T> > >
  : public binary_result<Op, unsigned, numb<natnum<T> > > {};

template <typename T>
struct binary_result<ops::binary::sub, int, numb<natnum<T> > >
  : public binary_result<ops::binary::sub, unsigned, numb<natnum<T> > > {};

template <typename T>
struct binary_result<ops::binary::rem, numb<natnum<T> >, int>
  : public binary_result<ops::binary::rem, numb<natnum<T> >, unsigned> {};

#endif

/*****************************************/

/* Unary abs */

template <typename T>
struct function_v<ops::unary::abs, numb<natnum<T> > > {
  inline const numb<natnum<T> >& operator()(const numb<natnum<T> >& v) const {
    return v;
  }
};

/* Binary sub */

/*template <typename T>
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
};*/

/* bitwise_and */

/*template <typename T>
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
};*/

/* bitwise_or */

/*template <typename T>
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
*/
/* bitwise_xor */
/*
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
*/
/* bitwise_and_not */
/*
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
*/
/* */
/*
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

*/

template <typename T>
struct is_positive_eval<numb<natnum<T> > > {
  inline bool operator()(const numb<natnum<T> >& v) const {
    return !v.is_zero();
  }
};

template <typename T>
struct is_negative_eval<numb<natnum<T> > > {
  inline bool operator()(const numb<natnum<T> >& v) const {
    return false;
  }
};

template <typename T, typename Forw>
struct set_4_eval<numb<natnum<T> >, Forw> {
  void operator()(numb<natnum<T> >& n, Forw first, const Forw last, unsigned base) const {
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

template <typename T>
struct floor_log2_eval<numb<natnum<T> > > {
  inline std::size_t operator()(const numb<natnum<T> >& n) const {
    return n.floor_log2();
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const numb<natnum<T> >& n)
{
  unsigned max_digits = n ? floor_log2(n) / 3 + 2 : 2;
  char st[max_digits];
  char* p = st + max_digits;
  *--p = 0;
  numb<natnum<T> > t = n;
  do {
    unsigned r = divrem(t, t, 10u);
    *--p = (char) r + '0';
  } while (t);
  return os << p;
}

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_NAT_NUM_ABST_HPP
