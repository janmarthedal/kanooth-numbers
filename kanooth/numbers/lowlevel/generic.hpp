/*
 * File:   kanooth/numbers/detail/lowlevel/generic.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2012
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_GENERIC_HPP_
#define _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_GENERIC_HPP_

#include <kanooth/number_bits.hpp>

namespace kanooth {
namespace numbers {
namespace lowlevel {

template <typename A = std::allocator<void> >
class generic {
public:

  // n >= 0
  template <typename T>
  static inline void copy_forward(T* z1, const T* x1, const std::size_t n)
  {
    if (n && z1 != x1)
      std::copy(x1, x1 + n, z1);
  }

  // n >= 0
  template <typename T>
  static inline void copy_backward(T* z1, const T* x1, const std::size_t n)
  {
    if (n && z1 != x1)
      std::copy_backward(x1, x1 + n, z1 + n);
  }

  template <typename T>
  static inline void fill_zero(T* rp, const std::size_t n)
  {
    std::fill_n(rp, n, 0);
  }

  // n >= 0
  template <typename T>
  static inline T add_1(T* rp, const T* xp, std::size_t n, const T y)
  {
    if (!n) return y;
    if (y) {
      T z = *xp + y;
      *rp = z;
      ++rp; ++xp; --n;
      if (z < y)
        return inc(rp, xp, n);
    }
    copy_forward(rp, xp, n);
    return 0;
  }

  // xn >= yn >= 0
  template <typename T>
  static T add(T* rp, const T* xp, const std::size_t xn,
               const T* yp, const std::size_t yn)
  {
    T carry = add_n(rp, xp, yp, yn);
    return add_1(rp + yn, xp + yn, xn - yn, carry);    
  }

  // n >= 0
  template <typename T>
  static inline T sub_1(T* rp, const T* xp, std::size_t n, const T y)
  {
    if (!n) return y;
    if (y) {
      T x = *xp;
      *rp = x - y;
      ++rp; ++xp; --n;
      if (x < y)
        return dec(rp, xp, n);
    }
    copy_forward(rp, xp, n);
    return 0;
  }

  // xn >= yn >= 0
  template <typename T>
  static T sub(T* rp, const T* xp, const std::size_t xn,
               const T* yp, const std::size_t yn)
  {
    T borrow = sub_n(rp, xp, yp, yn);
    return sub_1(rp + yn, xp + yn, xn - yn, borrow);
  }

  // n >= 0
  template <typename T>
  static inline T mul_1(T* rp, const T* xp, std::size_t n, const T y) {
    return multiply_sequence_with_digit(xp, xp+n, rp, y);
  }

  // xn >= yn >= 1
  // {rp, xn+yn}, {xp, xn} don't overlap, {rp, xn+yn}, {yp, yn} don't overlap
  template <typename T>
  static inline void mul(T* rp, const T* xp, const std::size_t xn,
                         const T* yp, const std::size_t yn) {
    multiply_sequences(xp, xp+xn, yp, yp+yn, rp);
  }

  template <typename T>
  static inline T quotrem_1(T* z1, const T* x1, std::size_t n, T y) {
    T* z2 = z1 + n;
    unsigned s = nlz(y);
    if (s) {
      T r = lshift(z1, x1, n, s);
      y <<= s;
      while (z2-- != z1)
        double_div_normalized(r, *z2, y, *z2, r);
      return r >> s;
    } else {
      const T* x2 = x1 + n;
      T r = 0;
      while (x2 != x1)
        double_div_normalized(r, *--x2, y, *--z2, r);
      return r;
    }
  }

  // un >= vn >= 1, vp[vn-1] != 0
  // {qp, un-vn+1}, {rp, vn}, {up, un}, {vp, vn} do not overlap
  template <typename T>
  static void quotrem(T* q1, T* r1, const T* u1, const std::size_t un,
                      const T* v1, const std::size_t vn) {
    if (vn == 1)
      r1[0] = quotrem_1(q1, u1, un, v1[0]);
    else {  // vn >= 2
      typename A::template rebind<T>::other alloc;
      T* t1 = alloc.allocate(un+1);
      T* w1alloc;
      const T* w1;
      unsigned shift = nlz(v1[vn-1]);
      // normalize
      if (shift) {
        w1alloc = alloc.allocate(vn);
        t1[un] = lshift(t1, u1, un, shift);
        lshift(w1alloc, v1, vn, shift);
        w1 = w1alloc;
      } else {
        std::copy(u1, u1+un, t1);
        t1[un] = 0;
        w1 = v1;
      }
      T wn1 = w1[vn-1], wn2 = w1[vn-2];
      T tjn, qh, k;

      for (int j=un-vn; j >= 0; --j) {
        tjn = t1[j+vn];
        qh = calc_qh(tjn, t1[j+vn-1], t1[j+vn-2], wn1, wn2);
        k = sequence_mult_digit_sub(t1+j, t1+j+vn, w1, qh);
        t1[j+vn] = tjn - k;
        if (k > tjn) {   // qh too big?
          t1[j+vn] += add_n(t1+j, t1+j, w1, vn);
          --qh;
        }
        q1[j] = qh;
      }

      // denormalize
      if (shift) {
        alloc.deallocate(w1alloc, vn);
        rshift(r1, t1, vn, shift);
      } else
        std::copy(t1, t1+vn, r1);
      alloc.deallocate(t1, un+1);
    }
  }

  // n > 0, z1 >= x1, 1 <= count < digit_bits
  template <typename T>
  static T lshift(T* z1, const T* x1, const std::size_t n, const unsigned count) {
    const unsigned int rcount = kanooth::number_bits<T>::value - count;
    T* z2 = z1 + n;
    const T* x2 = x1 + n;
    T xl = *--x2, xh;
    T r = xl >> rcount;
    while (x2 != x1) {
      xh = xl;
      xl = *--x2;
      *--z2 = (xh << count) | (xl >> rcount);
    }
    *--z2 = xl << count;
    return r;
  }

  // n > 0, z1 <= x1, 1 <= count < digit_bits
  template <typename T>
  static T rshift(T* z1, const T* x1, std::size_t n, unsigned count) {
    const unsigned int rcount = kanooth::number_bits<T>::value - count;
    const T* x2 = x1 + n;
    T xl, xh = *x1++;
    T r = xh << rcount;
    while (x1 != x2) {
      xl = xh;
      xh = *x1++;
      *z1++ = (xh << rcount) | (xl >> count);
    }
    *z1++ = xh >> count;
    return r;
  }

  template <typename T>
  static int comp(const T* xp, const T* yp, std::size_t n) {
    if (xp == yp) return 0;
    xp += n;
    yp += n;
    while (n--) {
      T x = *--xp;
      T y = *--yp;
      if (x < y) return -1;
      if (x > y) return 1;
    }
    return 0;
  }

  template <typename T>
  static void bitwise_and(T* zp, const T* xp, const T* yp, std::size_t n) {
    while (n--)
      *zp++ = *xp++ & *yp++;
  }

  template <typename T>
  static void bitwise_or(T* zp, const T* xp, const T* yp, std::size_t n) {
    while (n--)
      *zp++ = *xp++ | *yp++;
  }

  template <typename T>
  static void bitwise_xor(T* zp, const T* xp, const T* yp, std::size_t n) {
    while (n--)
      *zp++ = *xp++ ^ *yp++;
  }

  template <typename T>
  static void bitwise_and_not(T* zp, const T* xp, const T* yp, std::size_t n) {
    while (n--)
      *zp++ = *xp++ & ~*yp++;
  }

private:

  // n >= 0
  template <typename T>
  static inline T add_n(T* rp, const T* xp, const T* yp, std::size_t n)
  {
    T lx, ly, lz;
    bool carry = false;
    while (n--) {
      lx = *xp++;
      ly = *yp++;
      if (carry) {
        lz = lx + ly + 1;
        carry = lz <= lx;
      } else {
        lz = lx + ly;
        carry = lz < lx;
      }
      *rp++ = lz;
    }
    return carry ? 1 : 0;
  }

  // n >= 0
  template <typename T>
  static inline T sub_n(T* rp, const T* xp, const T* yp, std::size_t n)
  {
    T lx, ly, lz;
    bool borrow = false;
    while (n--) {
      lx = *xp++;
      ly = *yp++;
      if (borrow) {
        lz = lx - ly - 1;
        borrow = lx <= ly;
      } else {
        lz = lx - ly;
        borrow = lx < ly;
      }
      *rp++ = lz;
    }
    return borrow ? 1 : 0;
  }

  // n >= 0
  template <typename T>
  static inline T inc(T* rp, const T* xp, std::size_t n)
  {
    while (n && *xp == T(-1)) {
      *rp = 0;
      ++rp; ++xp; --n;
    }
    if (!n) return 1;
    *rp = *xp + 1;
    ++rp; ++xp; --n;
    copy_forward(rp, xp, n);
    return 0;
  }

  // n >= 0
  template <typename T>
  static inline T dec(T* rp, const T* xp, std::size_t n)
  {
    while (n && !*xp) {
      *rp = T(-1);
      ++rp; ++xp; --n;
    }
    if (!n) return 1;
    *rp = *xp - 1;
    ++rp; ++xp; --n;
    copy_forward(rp, xp, n);
    return 0;
  }

  template <typename T>
  static inline void double_mult_add_add(const T u, const T v, const T a1, const T a2, T& low, T& high)
  {
    const unsigned int halfbits = kanooth::number_bits<T>::value / 2;
    const T lowmask = (((T) 1) << halfbits) - 1;
    T u1 = u >> halfbits, u0 = u & lowmask;
    T v1 = v >> halfbits, v0 = v & lowmask;
    T r  = u0*v0 + (a1 & lowmask) + (a2 & lowmask);
    T s  = u1*v0 + (a1 >> halfbits) + (r >> halfbits);
    T t  = u0*v1 + (s & lowmask) + (a2 >> halfbits);
    high = u1*v1 + (s >> halfbits) + (t >> halfbits);
    low  = (t << halfbits) | (r & lowmask);
  }

  template <typename T>
  static inline void double_mult_add(const T u, const T v, const T a, T& low, T& high)
  {
    double_mult_add_add(u, v, a, T(0), low, high);
  }

  template <typename T>
  static inline T multiply_sequence_with_digit(const T* first, const T* last, T* dst, T v)
  {
    T k = 0;
    while (first != last) {
      double_mult_add(*first++, v, k, *dst, k);
      dst++;
    }
    return k;
  }

  template <typename T>
  static inline T multiply_add_sequence_with_digit(const T* first, const T* last, T* dst, T v)
  {
    T k = 0;
    while (first != last) {
      double_mult_add_add(*first++, v, *dst, k, *dst, k);
      dst++;
    }
    return k;
  }

  // Assumes ufirst != ulast and vfirst != vlast
  template <typename T>
  static inline void multiply_sequences(const T* ufirst, const T* ulast,
      const T* vfirst, const T* vlast, T* wfirst)
  {
    T *wlast = wfirst + (ulast - ufirst);
    *wlast++ = multiply_sequence_with_digit(ufirst, ulast, wfirst++, *vfirst++);
    while (vfirst != vlast)
      *wlast++ = multiply_add_sequence_with_digit(ufirst, ulast, wfirst++, *vfirst++);
  }

  template <typename T>
  static unsigned nlz(T v)
  {
    const unsigned int digitbits = kanooth::number_bits<T>::value;
    const T mask = ((T) 1) << (digitbits - 1);
    unsigned s = 0;
    while (!(v & mask)) {
      v <<= 1;
      ++s;
    }
    return s;
  }

  // Assumes nlz(v) = 0
  template <typename T>
  static void double_div_normalized(T uhigh, T ulow, T v, T& quot, T& rem)
  {
    const unsigned int digitbits = kanooth::number_bits<T>::value;
    const unsigned int halfbits = digitbits / 2;
    const T lowmask = (((T) 1) << halfbits) - 1;
    const T highmask = lowmask << halfbits;
    const T v1 = v >> halfbits;
    const T v0 = v & lowmask;
    const T u1 = ulow >> halfbits;
    const T u0 = ulow & lowmask;
    T q1, q0, r;

    q1 = uhigh / v1;
    r  = uhigh % v1;
    while ((q1 & highmask) || q1*v0 > ((r << halfbits) | u1)) {
      --q1;
      r += v1;
      if (r & highmask) break;
    }
    r = ((r << halfbits) | u1) - q1*v0;
    q0 = r / v1;
    r = r % v1;
    while ((q0 & highmask) || q0*v0 > ((r << halfbits) | u0)) {
      --q0;
      r += v1;
      if (r & highmask) break;
    }
    r = ((r << halfbits) | u0) - q0*v0;

    quot = (q1 << halfbits) | q0;
    rem = r;
  }

  template <typename T>
  static inline T mult_sub_sub(T& w, const T u, const T v, T k)
  {
    T wt;
    double_mult_add(u, v, k, wt, k);
    if (wt > w) ++k;
    w -= wt;
    return k;
  }

  template <typename T>
  static T sequence_mult_digit_sub(T* ufirst, T* ulast, const T* vfirst, T q)
  {
    T k = 0;
    while (ufirst != ulast)
      k = mult_sub_sub(*ufirst++, *vfirst++, q, k);
    return k;
  }

  template <typename T>
  static inline void double_mult(const T u, const T v, T& low, T& high)
  {
    double_mult_add_add(u, v, T(0), T(0), low, high);
  }

  template <typename T>
  static inline T calc_qh(T ujn, T ujn1, T ujn2, T vn1, T vn2)
  {
    T qh, rh, high, low;

    if (ujn == vn1) {
      qh = T(-1);
      rh = ujn1 + vn1;
      if (rh < vn1) return qh;
    } else
      double_div_normalized(ujn, ujn1, vn1, qh, rh);

    double_mult(qh, vn2, low, high);
    while (high > rh || (high == rh && low > ujn2)) {
      --qh;
      rh += vn1;
      if (rh < vn1) break;  // overflow
      double_mult(qh, vn2, low, high);
    }

    return qh;
  }

};

} // namespace detail
} // namespace numbers
} // namespace kanooth

#endif // _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_HPP_
