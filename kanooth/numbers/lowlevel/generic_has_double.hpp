/*
 * File:   kanooth/numbers/detail/lowlevel/generic_has_double.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-12-15 12:00Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2009-2012
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_GENERIC_HAS_DOUBLE_HPP_
#define _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_GENERIC_HAS_DOUBLE_HPP_

#include <cassert>
#include <kanooth/number_bits.hpp>
#include <kanooth/signed_right_shift.hpp>

namespace kanooth {
namespace numbers {
namespace lowlevel {

template <typename BASE, typename DOUBLE>
class generic_has_double {
public:
    
    typedef BASE digit_type;
    typedef DOUBLE double_type;
    static const unsigned digit_bits = number_bits<digit_type>::value;

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
            double_type z = *xp;
            z += y;
            *rp = z;
            ++rp;
            ++xp;
            --n;
            if (z >> digit_bits)
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
        double_type r = top_digit;
        while (x2 != x1) {
            r <<= digit_bits;
            r |= *--x2;
            *--z2 = r / y;
            r %= y;
        }
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
        const unsigned rcount = digit_bits - count;
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
        const unsigned rcount = digit_bits - count;
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

    static inline digit_type add_n(digit_type* rp, const digit_type* xp, const digit_type* yp, std::size_t n)
    {
        double_type carry = 0;
        while (n--) {
            carry += *xp;
            carry += *yp;
            *rp = carry;
            carry >>= digit_bits;
            ++rp;
            ++xp;
            ++yp;
        }
        return static_cast<digit_type>(carry);
    }

    static inline digit_type sub_n(digit_type* rp, const digit_type* xp, const digit_type* yp, std::size_t n)
    {
        double_type carry = 0;
        while (n--) {
            carry += *xp;
            carry -= *yp;
            *rp = carry;
            carry = signed_right_shift(carry, digit_bits);
            ++xp;
            ++yp;
            ++rp;
        }
        return carry ? 1 : 0;
    }

    static inline digit_type inc(digit_type* rp, const digit_type* xp, std::size_t n)
    {
        while (n && *xp == digit_type(-1)) {
            *rp = 0;
            ++rp;
            ++xp;
            --n;
        }
        if (!n) return 1;
        *rp = *xp + 1;
        ++rp;
        ++xp;
        --n;
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

    static inline digit_type multiply_sequence_with_digit(const digit_type* first, const digit_type* last, digit_type* dst, digit_type v)
    {
        double_type k = 0;
        while (first != last) {
            k += static_cast<double_type>(*first) * v;
            *dst = k;
            k >>= digit_bits;
            ++first;
            ++dst;
        }
        return static_cast<digit_type>(k);
    }

    static inline digit_type multiply_add_sequence_with_digit(const digit_type* first, const digit_type* last, digit_type* dst, digit_type v)
    {
        double_type k = 0;
        while (first != last) {
            k += static_cast<double_type>(*first) * static_cast<double_type>(v) + *dst;
            *dst = static_cast<digit_type>(k);
            k >>= digit_bits;
            ++first;
            ++dst;
        }
        return static_cast<digit_type>(k);
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

    static inline digit_type sequence_mult_digit_sub(digit_type* ufirst, digit_type* ulast, const digit_type* vfirst, digit_type q)
    {
        double_type k1 = 0, k2 = 0;
        while (ufirst != ulast) {
            k1 += static_cast<double_type>(*vfirst) * static_cast<double_type>(q);
            k2 += static_cast<double_type>(*ufirst) - static_cast<digit_type>(k1);
            *ufirst = static_cast<digit_type>(k2);
            k1 >>= digit_bits;
            k2 = signed_right_shift(k2, digit_bits);
            ++ufirst;
            ++vfirst;
        }
        return static_cast<digit_type>(k1 - k2);
    }

    static inline digit_type calc_qh(digit_type ujn, digit_type ujn1, digit_type ujn2, digit_type vn1, digit_type vn2)
    {
        digit_type qh, rh, high, low;
        double_type dbl;

        if (ujn == vn1) {
            qh = digit_type(-1);
            rh = ujn1 + vn1;
            if (rh < vn1) return qh;
        } else {
            dbl = (static_cast<double_type>(ujn) << digit_bits) | ujn1;
            qh = dbl / vn1;
            rh = dbl % vn1;
        }

        dbl = static_cast<double_type>(qh) * static_cast<double_type>(vn2);
        low = static_cast<digit_type>(dbl);
        high = dbl >> digit_bits;
        
        while (high > rh || (high == rh && low > ujn2)) {
            --qh;
            rh += vn1;
            if (rh < vn1) break;  // overflow
            dbl = static_cast<double_type>(qh) * static_cast<double_type>(vn2);
            low = static_cast<digit_type>(dbl);
            high = dbl >> digit_bits;
        }

        return qh;
    }

};

} // namespace detail
} // namespace numbers
} // namespace kanooth

#endif // _KANOOTH_NUMBERS_DETAIL_LOWLEVEL_GENERIC_HAS_DOUBLE_HPP_
