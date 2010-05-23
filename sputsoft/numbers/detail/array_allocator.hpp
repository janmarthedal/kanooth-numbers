/*
 * File:   sputsoft/numbers/detail/array_allocator.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-24 09:00Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_NUMBERS_DETAIL_ARRAY_ALLOCATOR_HPP_
#define _SPUTSOFT_NUMBERS_DETAIL_ARRAY_ALLOCATOR_HPP_

#include <memory>
#include <vector>

namespace sputsoft {
namespace numbers {
namespace detail {

template <typename T, typename A = std::allocator<T> >
class array_allocator {
private:
  A alloc;
public:
  typedef typename A::pointer pointer;
  typedef typename A::size_type size_type;
  std::pair<pointer, std::ptrdiff_t> allocate(size_type n) {
    n = ((n+5)/4)*4;
    return std::make_pair(alloc.allocate(n), n);
  }
  void deallocate(pointer p, size_type n) {
    alloc.deallocate(p, n);
  }
};

template <>
class array_allocator<void> {
public:
  template<typename T>
  struct rebind {
    typedef array_allocator<T> other;
  };
};

} // namespace detail
} // namespace numbers
} // namespace sputsoft

#endif // _SPUTSOFT_NUMBERS_DETAIL_ARRAY_ALLOCATOR_HPP_
