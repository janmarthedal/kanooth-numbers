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
  typedef expr<nat_num_wrap<Con, LowLevel> > number_type;
  boost::shared_ptr<number_type> num;
public:
  number_type::digit_type get() const {
    return num->get();
  }

  expr() : num(new number_type()) {}
  expr(const expr& rhs) : num(rhs.num) {}
  expr& operator=(const expr& rhs) {
    num = rhs.num;
    return *this;
  }

  expr(unsigned short v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned short v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned short y) { add_int(r.con, x.con, y); }
  static inline void add(expr& r, unsigned short x, const expr& y) { add_int(r.con, y.con, x); }

  expr(unsigned v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned y) { add_int(r.con, x.con, y); }
  static inline void add(expr& r, unsigned x, const expr& y) { add_int(r.con, y.con, x); }

  expr(unsigned long v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned long v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned long y) { add_int(r.con, x.con, y); }
  static inline void add(expr& r, unsigned long x, const expr& y) { add_int(r.con, y.con, x); }

#ifdef SPUTSOFT_HAS_LONG_LONG
  expr(unsigned long long v) : con() { set_int(con, v); }
  static inline void set(expr& r, unsigned long long v) { set_int(r.con, v); }
  static inline void add(expr& r, const expr& x, unsigned long long y) { add_int(r.con, x.con, y); }
  static inline void add(expr& r, unsigned long long x, const expr& y) { add_int(r.con, y.con, x); }
#endif

  static inline bool is_zero(const expr& u) {
    return u.con.is_empty();
  }
  static inline void set(expr& r, const expr& x) {
    set_num(r.con, x.con);
  }
  static inline void add(expr& r, const expr& x, const expr& y) {
    add_num(r.con, x.con, y.con);
  }
};

} // namespace sputsoft
} // namespace detail
} // namespace numbers

#endif	/* _NAT_NUM_WRAP_HPP */

