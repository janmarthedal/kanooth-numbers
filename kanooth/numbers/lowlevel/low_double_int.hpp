#ifndef KANOOTH_NUMBERS_LOW_DOUBLE_INT_HPP
#define KANOOTH_NUMBERS_LOW_DOUBLE_INT_HPP

#include <kanooth/number_bits.hpp>
#include <kanooth/signed_right_shift.hpp>

namespace kanooth {
namespace numbers {
namespace lowlevel {

template <typename T>
class low_double_int
{
public:
    typedef T base_type;
    static const unsigned base_bits = number_bits<base_type>::value;
    static const unsigned half_base_bits = base_bits / 2;
    //static const base_type low_half_mask = ~base_type() >> half_base_bits;
    static const base_type low_half_mask = (base_type(1) << half_base_bits) - 1;
    static const base_type high_half_mask = low_half_mask << half_base_bits;
    
    low_double_int() : low(), high()
    {
    }
    
    low_double_int(base_type v) : low(v), high()
    {
    }

    inline low_double_int& operator=(base_type v)
    {
        low = v;
        high = base_type();
    }

    inline operator base_type() const
    {
        return low;
    }

    inline operator bool() const
    {
        return low || high;
    }
    
    inline low_double_int& operator+=(base_type v)
    {
        base_type s = (low & low_half_mask) + (v & low_half_mask);
        base_type t = (s >> half_base_bits) + (low >> half_base_bits) + (v >> half_base_bits);
        low = (s & low_half_mask) | (t << half_base_bits);
        high += t >> half_base_bits;
        return *this;
    }
    
    inline low_double_int& operator-=(base_type v)
    {
        using ::kanooth::signed_right_shift;
        base_type s = (low & low_half_mask) - (v & low_half_mask);
        base_type t = signed_right_shift(s, half_base_bits) + (low >> half_base_bits) - (v >> half_base_bits);
        low = (s & low_half_mask) | (t << half_base_bits);
        high += signed_right_shift(t, half_base_bits);
        return *this;
    }
    
    inline low_double_int& operator*=(base_type v)
    {
        assert(!high);
        base_type a0 = low & low_half_mask;
        base_type a1 = low >> half_base_bits;
        base_type b0 = v & low_half_mask;
        base_type b1 = v >> half_base_bits;
        base_type s = a0 * b0;
        base_type l = s & low_half_mask;
        s >>= half_base_bits;
        s += a0 * b1;
        l |= s << half_base_bits;
        base_type h = s >> half_base_bits;
        s = (l >> half_base_bits) + a1 * b0;
        l = (l & low_half_mask) | (s << half_base_bits);
        h += (s >> half_base_bits) + a1 * b1;
        low = l;
        high = h;
        return *this;
    }
    
    inline low_double_int& operator/=(base_type v)
    {
        return *this;
    }
    
    inline low_double_int& operator%=(base_type v)
    {
        return *this;
    }
    
    inline low_double_int& operator+=(const low_double_int& rhs)
    {
        base_type s = (low & low_half_mask) + (rhs.low & low_half_mask);
        base_type t = (s >> half_base_bits) + (low >> half_base_bits) + (rhs.low >> half_base_bits);
        low = (s & low_half_mask) | (t << half_base_bits);
        high += rhs.high + (t >> half_base_bits);
        return *this;
    }
    
    inline low_double_int& operator-=(const low_double_int& rhs)
    {
        using ::kanooth::signed_right_shift;
        base_type s = (low & low_half_mask) - (rhs.low & low_half_mask);
        base_type t = signed_right_shift(s, half_base_bits) + (low >> half_base_bits) - (rhs.low >> half_base_bits);
        low = (s & low_half_mask) | (t << half_base_bits);
        high += signed_right_shift(t, half_base_bits) - rhs.high;
        return *this;
    }

    inline low_double_int& operator*=(const low_double_int& rhs)
    {
        assert(!rhs.high);
        return *this *= rhs.low;
    }
    
    inline low_double_int& operator/=(const low_double_int& rhs)
    {
        assert(!rhs.high);
        return *this /= rhs.low;
    }
    
    inline low_double_int& operator%=(const low_double_int& rhs)
    {
        assert(!rhs.high);
        return *this %= rhs.low;
    }
    
    inline low_double_int& operator<<=(unsigned amount)
    {
        assert(amount == base_bits);
        high = low;
        low = base_type();
        return *this;
    }
    
    inline low_double_int& operator>>=(unsigned amount)
    {
        assert(amount == base_bits);
        low = high;
        high = base_type();
        return *this;
    }

    inline void signed_right_shift(unsigned amount)
    {        
        assert(amount == base_bits);
        low = high;
        high = ::kanooth::signed_right_shift(high, base_bits);
    }
    
private:
    
    base_type low, high;
};

template <typename T>
inline low_double_int<T> operator+(const low_double_int<T>& a, const low_double_int<T>& b)
{
    low_double_int<T> r = a;
    r += b;
    return r;
}

template <typename T>
inline low_double_int<T> operator-(const low_double_int<T>& a, const low_double_int<T>& b)
{
    low_double_int<T> r = a;
    r -= b;
    return r;
}

template <typename T>
inline low_double_int<T> operator*(const low_double_int<T>& a, const low_double_int<T>& b)
{
    low_double_int<T> r = a;
    r *= b;
    return r;
}

template <typename T>
inline low_double_int<T> operator*(const low_double_int<T>& a, const T& b)
{
    low_double_int<T> r = a;
    r *= b;
    return r;
}

template <typename T>
inline low_double_int<T> operator/(const low_double_int<T>& a, const low_double_int<T>& b)
{
    low_double_int<T> r = a;
    r /= b;
    return r;
}

template <typename T>
inline low_double_int<T> operator%(const low_double_int<T>& a, const low_double_int<T>& b)
{
    low_double_int<T> r = a;
    r %= b;
    return r;
}

template <typename T>
inline low_double_int<T> operator<<(const low_double_int<T>& a, unsigned amount)
{
    low_double_int<T> r(a);
    r <<= amount;
    return r;
}

template <typename T>
inline low_double_int<T> operator>>(const low_double_int<T>& a, unsigned amount)
{
    low_double_int<T> r(a);
    r >>= amount;
    return r;
}

} // namespace lowlevel
} // namespace numbers

namespace detail {

template <typename B>
struct signed_right_shift_eval<numbers::lowlevel::low_double_int<B> > {
    numbers::lowlevel::low_double_int<B> operator()(const numbers::lowlevel::low_double_int<B>& v, unsigned amount) const {
        numbers::lowlevel::low_double_int<B> r = v;
        r.signed_right_shift(amount);
        return r;        
    }
};
    
} // namespace detail
} // namespace kanooth

#endif // KANOOTH_NUMBERS_LOW_DOUBLE_INT_HPP
