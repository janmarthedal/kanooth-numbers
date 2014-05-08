/*
 * File:   kanooth/numbers/detail/lowlevel/generic_sim_double.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2012
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_GENERIC_SIM_DOUBLE_HPP_
#define _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_GENERIC_SIM_DOUBLE_HPP_

#include <cassert>
#include <kanooth/number_bits.hpp>
#include <kanooth/make_signed.hpp>

namespace kanooth {
namespace numbers {
namespace lowlevel {

template <typename T>
class generic_sim_double {
public:
    
    typedef T digit_type;
    typedef typename make_signed<digit_type>::type signed_digit_type;
    static const unsigned digit_bits = number_bits<digit_type>::value;
    static const unsigned digit_half_bits = digit_bits / 2;
    static const digit_type digit_low_mask = (digit_type(1) << (digit_bits / 2)) - 1;

    static inline void copy_forward(digit_type* z1, const digit_type* x1, const std::size_t n)
    {
      if (n && z1 != x1)
        std::copy(x1, x1 + n, z1);
    }

    static inline void copy_backward(digit_type* z1, const digit_type* x1, const std::size_t n)
    {
      if (n && z1 != x1)
        std::copy_backward(x1, x1 + n, z1 + n);
    }

    static inline void fill_zero(digit_type* rp, const std::size_t n)
    {
      std::fill_n(rp, n, 0);
    }

    static inline digit_type add_1(digit_type* rp, const digit_type* xp, std::size_t n, const digit_type y)
    {
      if (!n) return y;
      if (y) {
        digit_type z = *xp + y;
        *rp = z;
        ++rp; ++xp; --n;
        if (z < y)
          return inc(rp, xp, n);
      }
      copy_forward(rp, xp, n);
      return 0;
    }

    static digit_type add(digit_type* rp, const digit_type* xp, const std::size_t xn,
                 const digit_type* yp, const std::size_t yn)
    {
      assert(xn >= yn);
      digit_type carry = add_n(rp, xp, yp, yn);
      return add_1(rp + yn, xp + yn, xn - yn, carry);    
    }

    static inline digit_type sub_1(digit_type* rp, const digit_type* xp, std::size_t n, const digit_type y)
    {
      if (!n) return y;
      if (y) {
        digit_type x = *xp;
        *rp = x - y;
        ++rp; ++xp; --n;
        if (x < y)
          return dec(rp, xp, n);
      }
      copy_forward(rp, xp, n);
      return 0;
    }

    static digit_type sub(digit_type* rp, const digit_type* xp, const std::size_t xn,
                 const digit_type* yp, const std::size_t yn)
    {
      assert(xn >= yn);
      digit_type borrow = sub_n(rp, xp, yp, yn);
      return sub_1(rp + yn, xp + yn, xn - yn, borrow);
    }

    static inline digit_type mul_1(digit_type* rp, const digit_type* xp, std::size_t n, const digit_type y) {
      return multiply_sequence_with_digit(xp, xp+n, rp, y);
    }

    static inline void mul(digit_type* rp, const digit_type* xp, const std::size_t xn,
                           const digit_type* yp, const std::size_t yn) {
      assert(yn >= 1);
      assert(xn >= yn);
      assert(rp+xn+yn <= xp || rp >= xp+xn);
      assert(rp+xn+yn <= yp || rp >= yp+yn);
      multiply_sequences(xp, xp+xn, yp, yp+yn, rp);
    }

    static inline digit_type quotrem_1(digit_type* z1, digit_type top_digit, const digit_type* x1, std::size_t n, digit_type y) {
      assert(n != 0);
      assert(y != 0);
      assert(y & (digit_type(1) << (digit_bits - 1)));  // nlz(y) == 0
      digit_type* z2 = z1 + n;
      const digit_type* x2 = x1 + n;
      digit_type r = top_digit;
      while (x2 != x1)
        double_div_normalized(r, *--x2, y, *--z2, r);
      return r;
    }

    // u1[un..0] divided by v1[vn-1..0]
    // on exit: quotient in q1, remainder in u1
    static void quotrem(digit_type* q1, digit_type* u1, const std::size_t un,
                        const digit_type* v1, const std::size_t vn) {
        assert(vn >= 2);
        assert(un >= vn);
        assert(v1[vn-1] & (digit_type(1) << (digit_bits - 1)));
        assert(u1[un] < v1[vn-1]);  // note: u1[un] must be well-defined
        assert(q1+un-vn+1 <= u1 || q1 >= u1+un+1);
        assert(q1+un-vn+1 <= v1 || q1 >= v1+vn);
        assert(u1+un+1 <= v1 || u1 >= v1+vn);

        digit_type wn1 = v1[vn-1], wn2 = v1[vn-2];
        digit_type tjn, qh, k;

        for (int j=un-vn; j >= 0; --j) {
            tjn = u1[j+vn];
            qh = calc_qh(tjn, u1[j+vn-1], u1[j+vn-2], wn1, wn2);
            k = sequence_mult_digit_sub(u1+j, u1+j+vn, v1, qh);
            if (k > tjn) {   // if we subtracted too much then qh is one too large
                digit_type k2 = add_n(u1+j, u1+j, v1, vn);
                assert(tjn - k + k2 == 0);
                --qh;
            } else
                assert(k == tjn);
            u1[j+vn] = 0;
            q1[j] = qh;
        }
    }

    static digit_type lshift(digit_type* z1, const digit_type* x1, const std::size_t n, const unsigned count) {
      assert(n != 0);
      assert(z1+n <= x1 || z1 >= x1);
      assert(count >= 1);
      assert(count < digit_bits);
      const unsigned int rcount = digit_bits - count;
      digit_type* z2 = z1 + n;
      const digit_type* x2 = x1 + n;
      digit_type xl = *--x2, xh;
      digit_type r = xl >> rcount;
      while (x2 != x1) {
        xh = xl;
        xl = *--x2;
        *--z2 = (xh << count) | (xl >> rcount);
      }
      *--z2 = xl << count;
      return r;
    }

    static digit_type rshift(digit_type* z1, const digit_type* x1, std::size_t n, unsigned count) {
      assert(n != 0);
      assert(z1 <= x1 || z1 >= x1+n);
      assert(count >= 1);
      assert(count < digit_bits);
      const unsigned int rcount = digit_bits - count;
      const digit_type* x2 = x1 + n;
      digit_type xl, xh = *x1++;
      digit_type r = xh << rcount;
      while (x1 != x2) {
        xl = xh;
        xh = *x1++;
        *z1++ = (xh << rcount) | (xl >> count);
      }
      *z1++ = xh >> count;
      return r;
    }

    static int comp(const digit_type* xp, const digit_type* yp, std::size_t n) {
      if (xp == yp) return 0;
      xp += n;
      yp += n;
      while (n--) {
        digit_type x = *--xp;
        digit_type y = *--yp;
        if (x < y) return -1;
        if (x > y) return 1;
      }
      return 0;
    }

    static void bitwise_and(digit_type* zp, const digit_type* xp, const digit_type* yp, std::size_t n) {
      while (n--)
        *zp++ = *xp++ & *yp++;
    }

    static void bitwise_or(digit_type* zp, const digit_type* xp, const digit_type* yp, std::size_t n) {
      while (n--)
        *zp++ = *xp++ | *yp++;
    }

    static void bitwise_xor(digit_type* zp, const digit_type* xp, const digit_type* yp, std::size_t n) {
      while (n--)
        *zp++ = *xp++ ^ *yp++;
    }

    static void bitwise_and_not(digit_type* zp, const digit_type* xp, const digit_type* yp, std::size_t n) {
      while (n--)
        *zp++ = *xp++ & ~*yp++;
    }

private:

    /*static inline digit_type add_n(digit_type* rp, const digit_type* xp, const digit_type* yp, std::size_t n)
    {
      digit_type lx, ly, lz;
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
    }*/

    static inline digit_type add_n(digit_type* rp, const digit_type* xp, const digit_type* yp, std::size_t n)
    {
      digit_type lx, ly, lz;
      digit_type carry = 0;
      while (n--) {
        lx = *xp++;
        ly = *yp++;
        carry += (lx & digit_low_mask) + (ly & digit_low_mask);
        lz = carry & digit_low_mask;
        carry >>= digit_half_bits;
        carry += (lx >> digit_half_bits) + (ly >> digit_half_bits);
        lz |= carry << digit_half_bits;
        carry >>= digit_half_bits;
        *rp++ = lz;
      }
      return carry ? 1 : 0;
    }

    /*static inline digit_type sub_n(digit_type* rp, const digit_type* xp, const digit_type* yp, std::size_t n)
    {
      digit_type lx, ly, lz;
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
    }*/

    static inline digit_type sub_n(digit_type* rp, const digit_type* xp, const digit_type* yp, std::size_t n)
    {
      digit_type lx, ly, lz;
      digit_type carry = 0;
      while (n--) {
        lx = *xp++;
        ly = *yp++;
        carry += (lx & digit_low_mask) - (ly & digit_low_mask);
        lz = carry & digit_low_mask;
        carry = static_cast<signed_digit_type>(carry) >> digit_half_bits;
        carry += (lx >> digit_half_bits) - (ly >> digit_half_bits);
        lz |= carry << digit_half_bits;
        carry = static_cast<signed_digit_type>(carry) >> digit_half_bits;
        *rp++ = lz;
      }
      return carry ? 1 : 0;
    }

    static inline digit_type inc(digit_type* rp, const digit_type* xp, std::size_t n)
    {
      while (n && *xp == digit_type(-1)) {
        *rp = 0;
        ++rp; ++xp; --n;
      }
      if (!n) return 1;
      *rp = *xp + 1;
      ++rp; ++xp; --n;
      copy_forward(rp, xp, n);
      return 0;
    }

    static inline digit_type dec(digit_type* rp, const digit_type* xp, std::size_t n)
    {
      while (n && !*xp) {
        *rp = digit_type(-1);
        ++rp; ++xp; --n;
      }
      if (!n) return 1;
      *rp = *xp - 1;
      ++rp; ++xp; --n;
      copy_forward(rp, xp, n);
      return 0;
    }

    static inline void double_mult_add_add(const digit_type u, const digit_type v, const digit_type a1, const digit_type a2, digit_type& low, digit_type& high)
    {
      digit_type u1 = u >> digit_half_bits, u0 = u & digit_low_mask;
      digit_type v1 = v >> digit_half_bits, v0 = v & digit_low_mask;
      digit_type r  = u0*v0 + (a1 & digit_low_mask) + (a2 & digit_low_mask);
      digit_type s  = u1*v0 + (a1 >> digit_half_bits) + (r >> digit_half_bits);
      digit_type t  = u0*v1 + (s & digit_low_mask) + (a2 >> digit_half_bits);
      high = u1*v1 + (s >> digit_half_bits) + (t >> digit_half_bits);
      low  = (t << digit_half_bits) | (r & digit_low_mask);
    }

    static inline void double_mult_add(const digit_type u, const digit_type v, const digit_type a, digit_type& low, digit_type& high)
    {
      double_mult_add_add(u, v, a, digit_type(0), low, high);
    }

    static inline digit_type multiply_sequence_with_digit(const digit_type* first, const digit_type* last, digit_type* dst, digit_type v)
    {
      digit_type k = 0;
      while (first != last) {
        double_mult_add(*first++, v, k, *dst, k);
        dst++;
      }
      return k;
    }

    static inline digit_type multiply_add_sequence_with_digit(const digit_type* first, const digit_type* last, digit_type* dst, digit_type v)
    {
      digit_type k = 0;
      while (first != last) {
        double_mult_add_add(*first++, v, *dst, k, *dst, k);
        dst++;
      }
      return k;
    }

    static inline void multiply_sequences(const digit_type* ufirst, const digit_type* ulast,
        const digit_type* vfirst, const digit_type* vlast, digit_type* wfirst)
    {
      assert(ufirst < ulast);
      assert(vfirst < vlast);
      digit_type *wlast = wfirst + (ulast - ufirst);
      *wlast++ = multiply_sequence_with_digit(ufirst, ulast, wfirst++, *vfirst++);
      while (vfirst != vlast)
        *wlast++ = multiply_add_sequence_with_digit(ufirst, ulast, wfirst++, *vfirst++);
    }

    static void double_div_normalized(digit_type uhigh, digit_type ulow, digit_type v, digit_type& quot, digit_type& rem)
    {
      const unsigned int halfbits = digit_bits / 2;
      const digit_type lowmask = (((digit_type) 1) << halfbits) - 1;
      const digit_type highmask = lowmask << halfbits;
      const digit_type v1 = v >> halfbits;
      const digit_type v0 = v & lowmask;
      const digit_type u1 = ulow >> halfbits;
      const digit_type u0 = ulow & lowmask;
      digit_type q1, q0, r;

      assert(v & (digit_type(1) << (digit_bits-1)));  // nlz(v) == 0
      q1 = uhigh / v1;
      r  = uhigh % v1;
      while ((q1 & highmask) || q1*v0 > ((r << halfbits) | u1)) {
        --q1;
        r += v1;
        if (r & highmask) break;
      }
      r = ((r << halfbits) | u1) - q1*v0;
      q0 = r / v1;
      r  = r % v1;
      while ((q0 & highmask) || q0*v0 > ((r << halfbits) | u0)) {
        --q0;
        r += v1;
        if (r & highmask) break;
      }
      r = ((r << halfbits) | u0) - q0*v0;

      quot = (q1 << halfbits) | q0;
      rem = r;
    }

    static inline digit_type mult_sub_sub(digit_type& w, const digit_type u, const digit_type v, digit_type k)
    {
      digit_type wt;
      double_mult_add(u, v, k, wt, k);
      if (wt > w) ++k;
      w -= wt;
      return k;
    }

    static digit_type sequence_mult_digit_sub(digit_type* ufirst, digit_type* ulast, const digit_type* vfirst, digit_type q)
    {
      digit_type k = 0;
      while (ufirst != ulast)
        k = mult_sub_sub(*ufirst++, *vfirst++, q, k);
      return k;
    }

    static inline void double_mult(const digit_type u, const digit_type v, digit_type& low, digit_type& high)
    {
      double_mult_add_add(u, v, digit_type(0), digit_type(0), low, high);
    }

    static inline digit_type calc_qh(digit_type ujn, digit_type ujn1, digit_type ujn2, digit_type vn1, digit_type vn2)
    {
      digit_type qh, rh, high, low;

      if (ujn == vn1) {
        qh = digit_type(-1);
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

#endif // _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_GENERIC_SIM_DOUBLE_HPP_
