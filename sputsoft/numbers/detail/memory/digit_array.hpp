/*
 * File:   kanooth/numbers/detail/memory/digit_array.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_MEMORY_DIGIT_ARRAY_HPP_
#define _KANOOTH_NUMBERS_DETAIL_MEMORY_DIGIT_ARRAY_HPP_

#include <memory>
#include <kanooth/numbers/detail/memory/array_allocator.hpp>

namespace kanooth {
namespace numbers {
namespace detail {

template <typename T=unsigned long, typename A=array_allocator<T> >
class digit_array {
private:
  A alloc;
  std::size_t used;
  std::size_t allocated;
  T* digits;
public:
  typedef T digit_type;
  digit_array() : used(0), allocated(0), digits(0) {}
  // min_size > 0
  explicit digit_array(std::size_t min_size) : used(0) {
    std::pair<T*, std::ptrdiff_t> m = alloc.allocate(min_size);
    allocated = m.second;
    digits = m.first;
  }
  digit_array(const digit_array& other) : used(other.used) {
    if (other.used) {
      std::pair<T*, std::ptrdiff_t> m = alloc.allocate(used);
      digits = m.first;
      allocated = m.second;
      std::copy(other.digits, other.digits + other.used, digits);
    } else {
      digits = 0;
      allocated = 0;
    }
  }
  // see http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
  digit_array& operator=(digit_array other) {
    other.swap(*this);
    return *this;
  }
  ~digit_array() {
    if (digits) alloc.deallocate(digits, allocated);
  }
  void swap(digit_array& other) {
    std::swap(used, other.used);
    std::swap(allocated, other.allocated);
    std::swap(digits, other.digits);
  }
  std::size_t size() const { return used; }
  void set_size(std::size_t n) { used = n; }
  bool request_size(std::size_t n) const { return n <= allocated; }
  bool is_empty() const { return !used; }
  const T* get() const { return digits; }
  T* get() { return digits; }
  const T& operator[](std::size_t n) const { return digits[n]; }
  T& operator[](std::size_t n) { return digits[n]; }
};

} // namespace detail
} // namespace numbers
} // namespace kanooth

#endif // _KANOOTH_NUMBERS_DETAIL_MEMORY_DIGIT_ARRAY_HPP_
