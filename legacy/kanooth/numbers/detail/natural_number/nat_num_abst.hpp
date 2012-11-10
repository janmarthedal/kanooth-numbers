/*
 * File:   kanooth/numbers/detail/natural_number/nat_num_abst.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-05-02 7:55Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_NATURAL_NUMBER_ABST_HPP
#define _KANOOTH_NUMBERS_DETAIL_NATURAL_NUMBER_ABST_HPP

#include <kanooth/numbers/number_traits.hpp>
#include <kanooth/numbers/detail/number_abst.hpp>
#include <kanooth/numbers/detail/default_ops.hpp>

namespace kanooth {
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

#ifdef KANOOTH_NATURAL_NUMBER_WITH_INTS

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

template <typename Op, typename T>
struct function_v_fallback {
  typedef typename unary_result<Op, T>::type return_type;
  inline const return_type operator()(const T& v) const {
    return_type r;
    evaluator_rv<Op, return_type, T>()(r, v);
    return r;
  }
};

template <typename T>
struct function_v<ops::unary::abs, numb<natnum<T> > > {
  inline const numb<natnum<T> >& operator()(const numb<natnum<T> >& v) const {
    return v;
  }
};

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
    kanooth::numbers::set(n, 0u);
    if (base >= 2 && base <= 36) {
      unsigned v;
      while (first != last) {
        char c = *first++;
        if (c >= '0' && c <= '9') v = c - '0';
        else if (c >= 'a' && c <= 'z') v = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
        else break;
        kanooth::numbers::mul(n, n, base);
        kanooth::numbers::add(n, n, v);
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
struct ruler_eval<numb<natnum<T> > > {
  inline std::size_t operator()(const numb<natnum<T> >& n) const {
    return n.ruler();
  }
};

template <typename T>
struct show_binary_eval<numb<natnum<T> > > {
  inline std::ostream& operator()(std::ostream& os, const numb<natnum<T> >& n) const {
    return n.show_binary(os);
  }
};

template <typename T>
struct test_bit_eval<numb<natnum<T> > > {
  inline bool operator()(const numb<natnum<T> >& n, std::size_t pos) const {
    return n.test_bit(pos);
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const numb<natnum<T> >& n)
{
  const std::size_t max_digits = n ? floor_log2(n) / 3 + 2 : 2;
  char st[max_digits];
  char* p = st + max_digits;
  *--p = 0;
  numb<natnum<T> > t = n;
  do {
    *--p = divrem(t, t, 10u) + '0';
  } while (t);
  return os << p;
}

} // namespace detail
} // namespace numbers
} // namespace kanooth

#endif // _KANOOTH_NUMBERS_DETAIL_NATURAL_NUMBER_ABST_HPP
