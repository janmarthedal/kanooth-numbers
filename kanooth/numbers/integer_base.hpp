#ifndef KANOOTH_NUMBERS_INTEGER_BASE_HPP
#define KANOOTH_NUMBERS_INTEGER_BASE_HPP

#include <kanooth/config.hpp>
#include <kanooth/is_signed.hpp>
#include <kanooth/make_unsigned.hpp>

namespace kanooth {
namespace numbers {

    namespace {

        template <typename T>
        typename make_unsigned<T>::type abs_int(T v)
        {
            return static_cast<typename make_unsigned<T>::type>(v < 0 ? -v : v);
        }

    }
    
template <typename N>
class integer_base {
    
    typedef N base_type;

public:

    integer_base& data() { return *this; }
    const integer_base& data() const { return *this; }
    
    integer_base()
    {
    }

    integer_base(long v) : number(static_cast<unsigned long>(v < 0 ? -v : v)), positive(v >= 0)
    {
    }

    integer_base(unsigned long v, bool pos = true) : number(v), positive(pos)
    {
    }
    
    integer_base(const base_type& other, bool pos) : number(other), positive(pos)
    {
    }
    
    integer_base(const integer_base& other) : number(other.number), positive(other.positive)
    {
    }
    
    integer_base(const char* s) : number()
    {
        bool pos = true;
        while (*s == '+' || *s == '-') {
            if (*s == '-') pos = !pos;
            ++s;
        }
        number = s;
        positive = pos;
    }
    
    integer_base& operator=(long v)
    {
        number = static_cast<unsigned long>(v >= 0 ? v : -v);
        positive = v >= 0;
        return *this;
    }
    
    integer_base& operator=(unsigned long v)
    {
        number = v;
        positive = true;
        return *this;
    }
    
    integer_base& operator=(const integer_base& other)
    {
        number = other.number;
        positive = other.positive;
        return *this;
    }    
    
    inline bool is_zero() const
    {
        return number.is_zero();
    }
    
    inline bool is_positive() const
    {
        return !number.is_zero() && positive;
    }
    
    inline bool is_negative() const
    {
        return !number.is_zero() && !positive;
    }
    
    inline void negate()
    {
        positive = !positive;
    }

    inline void twos_complement()
    {
        negate();
        subtract(*this, 1ul);
    }

    inline void add(const integer_base& a, const integer_base& b)
    {
        add_number(a, b.number, b.positive);
    }
    
    inline void subtract(const integer_base& a, const integer_base& b)
    {
        add_number(a, b.number, !b.positive);
    }
    
    inline void multiply(const integer_base& a, const integer_base& b)
    {
        number.multiply(a.number, b.number);
        positive = a.positive == b.positive;
    }
    
    inline void divide_truncate(const integer_base& a, const integer_base& b)
    {
        number.divide(a.number, b.number);
        positive = a.positive == b.positive;
    }
    
    inline void modulus_truncate(const integer_base& a, const integer_base& b)
    {
        number.modulus(a.number, b.number);
        positive = a.positive;
    }
    
    static inline void quotrem(integer_base& q, integer_base& r, const integer_base& a, const integer_base& b)
    {
        base_type::quotrem(q.number, r.number, a.number, b.number);
        q.positive = a.positive == b.positive;
        r.positive = a.positive;
    }

    inline void bitwise_and(const integer_base& a, const integer_base& b)
    {
        if (a.is_zero() || b.is_zero())
            *this = 0ul;
        else if (a.positive) {
            if (b.positive) {
                number.bitwise_and(a.number, b.number);
                positive = true;
            } else {
                integer_base tb = b;
                tb.twos_complement();
                number.bitwise_and_not(a.number, tb.number);
                positive = true;
            }
        } else {
            integer_base ta = a;
            ta.twos_complement();
            if (b.positive) {
                number.bitwise_and_not(b.number, ta.number);
                positive = true;
            } else {
                integer_base tb = b;
                tb.twos_complement();
                number.bitwise_or(ta.number, tb.number);
                positive = true;
                twos_complement();
            }
        }
    }

    inline void bitwise_or(const integer_base& a, const integer_base& b)
    {
        if (a.is_zero())
            *this = b;
        else if (b.is_zero())
            *this = a;
        else if (a.positive && b.positive) {
            number.bitwise_or(a.number, b.number);
            positive = true;
        } else {
            if (a.positive) {         // b negative
                integer_base tb = b;
                tb.twos_complement();
                number.bitwise_and_not(tb.number, a.number);
            } else if (b.positive) {  // a negative
                integer_base ta = a;
                ta.twos_complement();
                number.bitwise_and_not(ta.number, b.number);
            } else {                  // both negative
                integer_base ta = a, tb = b;
                ta.twos_complement();
                tb.twos_complement();
                number.bitwise_and(ta.number, tb.number);
            }
            positive = true;
            twos_complement();
        }
    }

    inline void bitwise_xor(const integer_base& a, const integer_base& b)
    {
        if (a.is_zero())
            *this = b;
        else if (b.is_zero())
            *this = a;
        else if (a.positive == b.positive) {
            if (a.positive) {
                number.bitwise_xor(a.number, b.number);
            } else {
                integer_base ta = a, tb = b;
                ta.twos_complement();
                tb.twos_complement();
                number.bitwise_xor(ta.number, tb.number);
            }
            positive = true;
        } else {
            if (a.positive) {
                integer_base tb = b;
                tb.twos_complement();
                number.bitwise_xor(a.number, tb.number);
            } else {
                integer_base ta = a;
                ta.twos_complement();
                number.bitwise_xor(ta.number, b.number);
            }
            positive = true;
            twos_complement();
        }
    }

    inline void left_shift(const integer_base& a, unsigned long count)
    {
        number.left_shift(a.number, count);
    }

    inline void right_shift(const integer_base& a, unsigned long count)
    {
        number.right_shift(a.number, count);
    }

    inline bool bit_test(unsigned pos) const
    {
        return number.bit_test(pos);
    }

    inline void bit_set(unsigned pos)
    {
        number.bit_set(pos);
    }

    inline void bit_unset(unsigned pos)
    {
        number.bit_unset(pos);
    }

    inline void bit_flip(unsigned pos)
    {
        number.bit_flip(pos);
    }

    inline int compare(const integer_base& a) const
    {
        return compare_helper(a.number, a.sign());
    }

    inline int compare(unsigned long a) const
    {
        return compare_helper(a, a == 0 ? 0 : 1);
    }
    
    inline int compare(signed long a) const
    {
        return compare_helper(static_cast<unsigned long>(a >= 0 ? a : -a), a == 0 ? 0 : a < 0 ? -1 : 1);
    }    

#if 1
    inline void gcd(const integer_base& a, const integer_base& b)
    {
        number.gcd(a.number, b.number);
        positive = true;
    }
#endif

#if 0
    void gcd(const integer_base& a, const integer_base& b)
    {
        if (a.is_zero()) {
            number = b.number;
        } else if (b.is_zero()) {
            number = a.number;
        } else {
            integer_base u, v, w;
            int c = 0;

            unsigned us = a.number.lsb();
            unsigned vs = b.number.lsb();
            unsigned shift = (std::min)(us, vs);
            u.number.right_shift(a.number, us);
            u.positive = a.positive;
            v.number.right_shift(b.number, vs);
            v.positive = b.positive;

            while (!v.is_zero()) {
                vs = v.number.lsb();
                v.number.right_shift(v.number, vs);
                c += vs;
                if (c > 0) {
                    u.swap(v);
                    c = -c;
                }
                w.add(u, v);
                w.number.right_shift(w.number, 1);
                if (w.number.lsb() != 0) {
                    v = w;
                } else {
                    v.subtract(w, v);
                }
            }

            number.left_shift(u.number, shift);
        }
        positive = true;
    }
#endif

    
    std::string str(std::streamsize size, std::ios_base::fmtflags f) const
    {
        std::string res = number.str(size, f);
        return positive ? res : "-" + res;
    }
    
    void swap(integer_base& other)
    {
        number.swap(other.number);
        std::swap(positive, other.positive);
    }

    inline void add(const integer_base& a, unsigned long b)
    {
        add_int(a, b);
    }
    
    inline void add(const integer_base& a, signed long b)
    {
        add_int(a, b);
    }

    inline void subtract(const integer_base& a, unsigned long b)
    {
        subtract_int(a, b);
    }
    
    inline void subtract(const integer_base& a, signed long b)
    {
        subtract_int(a, b);
    }
    
    inline void multiply(const integer_base& a, unsigned long b)
    {
        multiply_int(a, b);
    }
    
    inline void multiply(const integer_base& a, signed long b)
    {
        multiply_int(a, b);
    }
    
    inline void divide_truncate(const integer_base& a, unsigned long b)
    {
        divide_truncate_int(a, b);
    }
    
    inline void divide_truncate(const integer_base& a, signed long b)
    {
        divide_truncate_int(a, b);
    }
    
    inline void modulus_truncate(const integer_base& a, unsigned long b)
    {
        modulus_truncate_int(a, b);
    }
    
    inline void modulus_truncate(const integer_base& a, signed long b)
    {
        modulus_truncate_int(a, b);
    }

    static inline unsigned long integer_modulus(const integer_base& a, unsigned long b)
    {
        return base_type::integer_modulus(a.number, b);
    }
    
#ifdef KANOOTH_HAS_LONG_LONG
    
    inline void add(const integer_base& a, unsigned long long b)
    {
        add_int(a, b);
    }
    
    inline void add(const integer_base& a, signed long long b)
    {
        add_int(a, b);
    }

    inline void subtract(const integer_base& a, unsigned long long b)
    {
        subtract_int(a, b);
    }
    
    inline void subtract(const integer_base& a, signed long long b)
    {
        subtract_int(a, b);
    }
    
    inline void multiply(const integer_base& a, unsigned long long b)
    {
        multiply_int(a, b);
    }
    
    inline void multiply(const integer_base& a, signed long long b)
    {
        multiply_int(a, b);
    }
    
    inline void divide_truncate(const integer_base& a, unsigned long long b)
    {
        divide_truncate_int(a, b);
    }
    
    inline void divide_truncate(const integer_base& a, signed long long b)
    {
        divide_truncate_int(a, b);
    }
    
    inline void modulus_truncate(const integer_base& a, unsigned long long b)
    {
        modulus_truncate_int(a, b);
    }
    
    inline void modulus_truncate(const integer_base& a, signed long long b)
    {
        modulus_truncate_int(a, b);
    }

    static inline unsigned long long integer_modulus(const integer_base& a, unsigned long long b)
    {
        return base_type::integer_modulus(a.number, b);
    }
    
#endif
    
private:
    
    inline int sign() const
    {
        return number.is_zero() ? 0 : positive ? 1 : -1;
    }
    
    void add_number(const integer_base& a, const base_type& b_number, bool b_positive)
    {
        if (a.positive == b_positive) {
            positive = a.positive;
            number.add(a.number, b_number);
        } else if (a.number.compare(b_number) >= 0) {
            positive = a.positive;
            number.subtract(a.number, b_number);
        } else {
            positive = b_positive;
            number.subtract(b_number, a.number);
        }
    }

    template <typename T>
    inline void add_int(const integer_base& a, T b)
    {
        add_int_helper(a, abs_int(b), b >= 0);
    }
    
    template <typename T>
    inline void subtract_int(const integer_base& a, T b)
    {
        add_int_helper(a, abs_int(b), b < 0);
    }
    
    template <typename T>
    void add_int_helper(const integer_base& a, T b, bool b_positive)
    {
        //STATIC_ASSERT(is_unsigned<T>::value)
        if (a.positive == b_positive) {
            positive = a.positive;
            number.add(a.number, b);
        } else if (a.number.compare(b) >= 0) {
            positive = a.positive;
            number.subtract(a.number, b);
        } else {
            positive = b_positive;
            number.subtract(base_type(b), a.number);
        }
    }
    
    template <typename T>
    void multiply_int(const integer_base& a, const T& b)
    {
        number.multiply(a.number, abs_int(b));
        positive = a.positive == (b >= 0);
    }
    
    template <typename T>
    void divide_truncate_int(const integer_base& a, const T& b)
    {
        number.divide(a.number, abs_int(b));
        positive = a.positive == (b >= 0);
    }
    
    template <typename T>
    inline void modulus_truncate_int(const integer_base& a, T b)
    {
        number.modulus(a.number, abs_int(b));
        positive = a.positive;
    }
    
    template <typename T>
    int compare_helper(const T& a, int a_sgn) const
    {
        int sgn = sign();
        if (sgn == a_sgn) {
            int c = number.compare(a);
            return positive ? c : -c;
        } else {
            return sgn - a_sgn;
        }
    }

    base_type number;
    bool positive;
    
};

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_INTEGER_BASE_HPP
