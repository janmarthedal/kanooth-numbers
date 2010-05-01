/* 
 * File:   nat_num_wrap.hpp
 * Author: jmr
 *
 * Created on April 30, 2010, 5:18 PM
 */

#ifndef _SHARED_NAT_NUM_WRAP_HPP
#define	_SHARED_NAT_NUM_WRAP_HPP

#include <sputsoft/numbers/detail/nat_num_wrap.hpp>
#include <boost/shared_ptr.hpp>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename Con, typename LowLevel>
class expr<boost::shared_ptr<nat_num_wrap<Con, LowLevel> > > {
private:
  typedef typename Con::digit_type digit_type;
  typedef expr<nat_num_wrap<Con, LowLevel> > number_type;
  boost::shared_ptr<number_type> num;

  template <typename T>
  void set(T v) {
    if (!num.unique()) num.reset(new number_type);
    number_type::set(*num, v);
  }

  template <typename V1, typename V2>
  void add(const V1& v1, const V2& v2) {
    if (!num.unique()) num.reset(new number_type);
    number_type::add(*num, v1, v2);
  }

public:
  digit_type get() const {
    return num->get();
  }

  expr() : num(new number_type) {}
  expr(const expr& rhs) : num(rhs.num) {}
  expr& operator=(const expr& rhs) {
    num = rhs.num;
    return *this;
  }

  expr(unsigned short v) : num(new number_type(v)) {}
  static inline void set(expr& r, unsigned short v) { r.set(v); }
  static inline void add(expr& r, const expr& x, unsigned short y) { r.add(*x.num, y); }
  static inline void add(expr& r, unsigned short x, const expr& y) { r.add(*y.num, x); }

  expr(unsigned v) : num(new number_type(v)) {}
  static inline void set(expr& r, unsigned v) { r.set(v); }
  static inline void add(expr& r, const expr& x, unsigned y) { r.add(*x.num, y); }
  static inline void add(expr& r, unsigned x, const expr& y) { r.add(*y.num, x); }

  expr(unsigned long v) : num(new number_type(v)) {}
  static inline void set(expr& r, unsigned long v) { r.set(v); }
  static inline void add(expr& r, const expr& x, unsigned long y) { r.add(*x.num, y); }
  static inline void add(expr& r, unsigned long x, const expr& y) { r.add(*y.num, x); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  expr(unsigned long long v) : num(new number_type(v)) {}
  static inline void set(expr& r, unsigned long long v) { r.set(v); }
  static inline void add(expr& r, const expr& x, unsigned long long y) { r.add(*x.num, y); }
  static inline void add(expr& r, unsigned long long x, const expr& y) { r.add(*y.num, x); }
#endif

  static inline bool is_zero(const expr& u) {
    return u.num->is_zero();
  }
  static inline void set(expr& r, const expr& x) {
    r.num = x.num;
  }
  static inline void add(expr& r, const expr& x, const expr& y) {
    r.add(*x.num, *y.num);
  }
};

} // namespace sputsoft
} // namespace detail
} // namespace numbers

#endif	/* _NAT_NUM_WRAP_HPP */

