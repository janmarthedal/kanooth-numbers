#ifndef KANOOTH_NUMBERS_DOUBLE_INTEGER_HPP
#define KANOOTH_NUMBERS_DOUBLE_INTEGER_HPP

#include <sstream>
#include <kanooth/number_bits.hpp>
#include <kanooth/make_signed.hpp>

namespace kanooth {
namespace numbers {
 
#define DBL_INT_AVOID_CMP 1

template <typename T>
class double_integer
{
public:
    typedef T base_type;
    typedef typename make_signed<base_type>::type signed_base_type;
    static const unsigned base_bits = number_bits<base_type>::value;
    static const unsigned half_base_bits = base_bits / 2;
    static const base_type low_half_mask = base_type(-1) >> half_base_bits;
    static const base_type high_half_mask = low_half_mask << half_base_bits;
    
    double_integer() : low(), high()
    {
    }
    
    double_integer(T v) : low(v), high()
    {
    }

    double_integer(T h, T l) : low(l), high(h)
    {
    }
    
    double_integer& operator=(T v)
    {
        low = v;
        high = T();
    }

    double_integer& operator=(const double_integer& other)
    {
        if (this != &other) {
            low = other.low;
            high = other.high;
        }
        return *this;
    }

    double_integer& operator+=(const double_integer& rhs)
    {
#if DBL_INT_AVOID_CMP
        base_type t1 = (low & low_half_mask) + (rhs.low & low_half_mask);
        base_type t2 = (low >> half_base_bits) + (rhs.low >> half_base_bits) + (t1 >> half_base_bits);
        low = (t1 & low_half_mask) | (t2 << half_base_bits);
        high += (t2 >> half_base_bits) + rhs.high;
#else
        low  += rhs.low;
        high += rhs.high;
        if (low < rhs.low)
            ++high;
#endif
        return *this;
    }
    
    double_integer& operator-=(const double_integer& rhs)
    {
#if DBL_INT_AVOID_CMP
        base_type t1 = (low & low_half_mask) - (rhs.low & low_half_mask);
        base_type t2 = static_cast<base_type>(static_cast<signed_base_type>(t1) >> half_base_bits)
                + (low >> half_base_bits) - (rhs.low >> half_base_bits);
        low = (t1 & low_half_mask) | (t2 << half_base_bits);
        high = high - rhs.high + static_cast<base_type>(static_cast<signed_base_type>(t2) >> half_base_bits);
#else
        if (rhs.low > low)
            --high;
        low  -= rhs.low;
        high -= rhs.high;
#endif
        return *this;
    }

    double_integer& operator*=(const double_integer& rhs)
    {
        return *this = mult
    }

    std::string str() const
    {
        std::stringstream ss;
        if (base_bits == 8) {
            ss << (unsigned)high << "|" << (unsigned)low;
        } else {
            ss << high << "|" << low;
        }
        return ss.str();
    }
    
    void multiply(const double_integer& a, const double_integer& b)
    {
        base_type a0 = a.low & low_half_mask;
        base_type a1 = a.low >> half_base_bits;
        base_type b0 = b.low & low_half_mask;
        base_type b1 = b.low >> half_base_bits;

        base_type k = a0 * b0;
        low = k & low_half_mask;
        k >>= half_base_bits;
        k += a0 * b1;
        low |= k << half_base_bits;
        high = k >> half_base_bits;
        
        k = a1 * b0 + (low >> half_base_bits);
        low = (low & low_half_mask) | (k << half_base_bits);

        high += k >> half_base_bits;        
        high += a1 * b1;
        high += a.low * b.high + a.high * b.low;
    }

private:
    
    T low, high;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const double_integer<T>& di)
{
    return os << di.str();
}

template <typename T>
double_integer<T> operator+(const double_integer<T>& a, const double_integer<T>& b)
{
    double_integer<T> r = a;
    r += b;
    return r;
}

template <typename T>
double_integer<T> operator-(const double_integer<T>& a, const double_integer<T>& b)
{
    double_integer<T> r = a;
    r -= b;
    return r;
}

template <typename T>
double_integer<T> operator*(const double_integer<T>& a, const double_integer<T>& b)
{
    double_integer<T> r;
    r.multiply(a, b);
    return r;
}

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_DOUBLE_INTEGER_HPP
