/*
 * File:   math/numbers/detail/digit_array.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_MATH_NUMBERS_DETAIL_DIGIT_ARRAY_HPP_
#define _SPUTSOFT_MATH_NUMBERS_DETAIL_DIGIT_ARRAY_HPP_

namespace sputsoft {

template <typename T>
void swap(T& a, T& b) {
  T t = a;  a = b;  b = t;
}

}

/*namespace sputsoft {
namespace math {
namespace numbers {*/
namespace detail {

template <typename T>
class digit_array {
  template <typename Os, typename S>
  friend Os& operator<<(Os& os, const digit_array<S>& d);
private:
  std::size_t used;
  std::size_t allocated;
  T* digits;
public:
  typedef T digit_type;
  digit_array() : used(0), allocated(0), digits(0) {}
  digit_array(std::size_t min_size) : used(0), allocated(min_size),
    digits(min_size ? new T[min_size] : 0) {}
  ~digit_array() {
    if (digits) delete[] digits;
  }
  void swap(digit_array& other) {
    ::sputsoft::swap(used, other.used);
    ::sputsoft::swap(allocated, other.allocated);
    ::sputsoft::swap(digits, other.digits);
  }
  std::size_t size() const { return used; }
  void set_size(std::size_t n) { used = n; }
  bool will_fit(std::size_t n) const { return n <= allocated; }
  bool is_empty() const { return used == 0; }
  const T* get() const { return digits; }
  T* get() { return digits; }
  const T& operator[](std::size_t n) const { return digits[n]; }
  T& operator[](std::size_t n) { return digits[n]; }
};

template <typename Os, typename T>
Os& operator<<(Os& os, const digit_array<T>& d)
{
  os << "[";
  for (std::size_t n=0; n < d.size(); n++) {
    if (n > 0) os << " ";
    os << d[n];
  }
  return os << "]";
}

} // namespace detail
/*} // namespace numbers
} // namespace math
} // namespace sputsoft*/

#endif // _SPUTSOFT_MATH_NUMBERS_DETAIL_DIGIT_ARRAY_HPP_

