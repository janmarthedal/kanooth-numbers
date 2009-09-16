/*
 * File:   SimpleDigitVector.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-26 18:41Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SIMPLEDIGITVECTOR_HPP
#define _SIMPLEDIGITVECTOR_HPP

namespace sputsoft {
namespace multiprecision {
namespace detail {


template <typename T>
class SimpleDigitVector {
public:
  typedef T value_type;
  typedef size_t size_type;
  SimpleDigitVector() : _capacity(2), _first(new T[_capacity]), _last(_first) {}
  explicit SimpleDigitVector(size_type __size)
    : _capacity(std::max(__size, (size_type) 2)), _first(new T[_capacity]), _last(_first) {}
  ~SimpleDigitVector() { delete[] _first; }
  size_type size() const { return _last - _first; }
  bool request_size(size_type req_size) const { return _capacity >= req_size; }
  void set_end(T* end) { _last = end; }
  T* begin() { return _first; }
  T* end() { return _last; }
  const T* begin() const { return _first; }
  const T* end() const { return _last; }
private:
  size_type _capacity;
  T *_first;
  T *_last;
};


}
} // multiprecision
} // sputsoft

#endif	/* _SIMPLEDIGITVECTOR_HPP */
