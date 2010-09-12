/*
 * File:   sputsoft/shared_ptr.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2010-09-10 08:44Z
 *
 * (C) Copyright SputSoft 2010
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _SPUTSOFT_SHARED_PTR_HPP_
#define _SPUTSOFT_SHARED_PTR_HPP_

#ifdef SPUTSOFT_USE_BOOST
#include <boost/shared_ptr.hpp>
#endif

namespace sputsoft {

#ifdef SPUTSOFT_USE_BOOST

using boost::shared_ptr;

#else

template <typename T>
class shared_ptr {
private:
  T* px;
  unsigned* pn;  // Reference count: *pn+1
public:
  shared_ptr() : px(0), pn(0) {}
  shared_ptr(T* p) : px(p), pn(new unsigned) {
    *pn = 0;
  }
  shared_ptr(const shared_ptr& sp) : px(sp.px), pn(sp.pn) {
    if (px) ++*pn;
  }
  shared_ptr& operator=(const shared_ptr& sp) {
    shared_ptr(sp).swap(*this);
    return *this;
  }
  ~shared_ptr() {
    if (px) {
      if (unique()) {
        delete px;
        delete pn;
      } else
        --*pn;
    }
  }
  bool unique() const {
    return !*pn;
  }
  void swap(shared_ptr& other) {
    std::swap(px, other.px);
    std::swap(pn, other.pn);
  }
  T& operator*() const {
    return *px;
  }
  T* operator->() const {
    return px;
  }
  void reset(T* p) {
    shared_ptr(p).swap(*this);
  }
};

#endif

} // namespace sputsoft

#endif // _SPUTSOFT_SHARED_PTR_HPP_
