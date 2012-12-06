#ifndef KANOOTH_NUMBERS_INTEGER_BASE_HPP
#define KANOOTH_NUMBERS_INTEGER_BASE_HPP

namespace kanooth {
namespace numbers {
        
template <typename N>
class integer_base {
    
    typedef N base_type;

public:

    integer_base& data() { return *this; }
    const integer_base& data() const { return *this; }
    
    integer_base() {}

    integer_base(long v) : number(static_cast<unsigned long>(v < 0 ? -v : v)), positive(v >= 0) {}

    integer_base(unsigned long v, bool pos = true) : number(v), positive(pos) {}
    
    integer_base(const base_type& other, bool pos) : number(other), positive(pos) {}
    
    integer_base(const integer_base& other) : number(other.number), positive(other.positive) {}
    
    integer_base(const char* s) : number() {
        bool pos = true;
        while (*s == '+' || *s == '-') {
            if (*s == '-') pos = !pos;
            ++s;
        }
        number = s;
        positive = pos;
    }
    
    integer_base& operator=(long v) {
        number = static_cast<unsigned long>(v >= 0 ? v : -v);
        positive = v >= 0;
        return *this;
    }
    
    integer_base& operator=(unsigned long v) {
        number = v;
        positive = true;
        return *this;
    }
    
    integer_base& operator=(const integer_base& other) {
        number = other.number;
        positive = other.positive;
        return *this;
    }    
    
    inline bool is_zero() const {
        return number.is_zero();
    }
    
    inline bool is_positive() const {
        return !number.is_zero() && positive;
    }
    
    inline bool is_negative() const {
        return !number.is_zero() && !positive;
    }
    
    inline void negate() {
        positive = !positive;
    }

    inline void twos_complement() {
        negate();
        subtract(*this, 1ul);
    }

    inline void add(const integer_base& a, const integer_base& b) {
        add_helper1(a, b.number, b.positive);
    }
    
    inline void add(const integer_base& a, unsigned long b) {
        add_helper2(a, b, true);
    }
    
    inline void add(const integer_base& a, signed long b) {
        add_helper2(a, static_cast<unsigned long>(b >= 0 ? b : -b), b >= 0);
    }
    
    inline void subtract(const integer_base& a, const integer_base& b) {
        add_helper1(a, b.number, !b.positive);
    }
    
    inline void subtract(const integer_base& a, unsigned long b) {
        add_helper2(a, b, false);
    }
    
    inline void subtract(const integer_base& a, signed long b) {
        add_helper2(a, static_cast<unsigned long>(b >= 0 ? b : -b), !(b >= 0));
    }
    
    inline void multiply(const integer_base& a, const integer_base& b) {
        multiply_helper(a, b.number, b.positive);
    }
    
    inline void multiply(const integer_base& a, unsigned long b) {
        multiply_helper(a, b, true);
    }
    
    inline void multiply(const integer_base& a, signed long b) {
        multiply_helper(a, static_cast<unsigned long>(b >= 0 ? b : -b), b >= 0);
    }
    
    inline void divide_truncate(const integer_base& a, const integer_base& b) {
        divide_truncate_helper(a, b.number, b.positive);
    }
    
    inline void divide_truncate(const integer_base& a, unsigned long b) {
        divide_truncate_helper(a, b, true);
    }
    
    inline void divide_truncate(const integer_base& a, signed long b) {
        divide_truncate_helper(a, static_cast<unsigned long>(b >= 0 ? b : -b), b >= 0);
    }
    
    inline void modulus_truncate(const integer_base& a, const integer_base& b) {
        number.modulus(a.number, b.number);
        positive = a.positive;
    }
    
    inline void modulus_truncate(const integer_base& a, unsigned long b) {
        number.modulus(a.number, b);
        positive = a.positive;
    }
    
    inline void modulus_truncate(const integer_base& a, signed long b) {
        number.modulus(a.number, static_cast<unsigned long>(b >= 0 ? b : -b));
        positive = a.positive;
    }

    static inline unsigned long integer_modulus(const integer_base& a, unsigned long b) {
        return base_type::integer_modulus(a.number, b);
    }
    
    static inline void quotrem(integer_base& q, integer_base& r, const integer_base& a, const integer_base& b) {
        base_type::quotrem(q.number, r.number, a.number, b.number);
        q.positive = a.positive == b.positive;
        r.positive = a.positive;
    }

    inline void bitwise_and(const integer_base& a, const integer_base& b) {
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

    inline void bitwise_or(const integer_base& a, const integer_base& b) {
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

    inline void bitwise_xor(const integer_base& a, const integer_base& b) {
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

    inline void left_shift(const integer_base& a, unsigned long count) {
        number.left_shift(a.number, count);
    }

    inline void right_shift(const integer_base& a, unsigned long count) {
        number.right_shift(a.number, count);
    }

    inline bool bit_test(unsigned pos) const {
        return number.bit_test(pos);
    }

    inline void bit_set(unsigned pos) {
        number.bit_set(pos);
    }

    inline void bit_unset(unsigned pos) {
        number.bit_unset(pos);
    }

    inline void bit_flip(unsigned pos) {
        number.bit_flip(pos);
    }

    inline int compare(const integer_base& a) const {
        return compare_helper(a.number, a.sign());
    }

    inline int compare(unsigned long a) const {
        return compare_helper(a, a == 0 ? 0 : 1);
    }
    
    inline int compare(signed long a) const {
        return compare_helper(static_cast<unsigned long>(a >= 0 ? a : -a), a == 0 ? 0 : a < 0 ? -1 : 1);
    }    
    
    std::string str(std::streamsize size, std::ios_base::fmtflags f) const {
        std::string res = number.str(size, f);
        return positive ? res : "-" + res;
    }
    
    void swap(integer_base& other) {
        number.swap(other.number);
        std::swap(positive, other.positive);
    }

private:
    
    inline int sign() const {
        return number.is_zero() ? 0 : positive ? 1 : -1;
    }
    
    void add_helper1(const integer_base& a, const base_type& b_number, bool b_positive) {
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
    
    void add_helper2(const integer_base& a, unsigned long b, bool b_positive) {
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
    void multiply_helper(const integer_base& a, const T& b, bool b_positive) {
        number.multiply(a.number, b);
        positive = a.positive == b_positive;
    }
    
    template <typename T>
    void divide_truncate_helper(const integer_base& a, const T& b, bool b_positive) {
        number.divide(a.number, b);
        positive = a.positive == b_positive;
    }
    
    template <typename T>
    int compare_helper(const T& a, int a_sgn) const {
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
