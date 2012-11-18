#ifndef KANOOTH_NUMBERS_NATURAL_NUMBER_HPP
#define KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

#include <memory>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>

#include <kanooth/number_bits.hpp>
#include <kanooth/numbers/lowlevel/generic.hpp>

namespace kanooth {
namespace numbers {

// TODO move to own file
template <typename T>
unsigned nlz(T v)
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

        
template <typename T = unsigned long, typename LowLevel = lowlevel::generic, typename Allocator = std::allocator<T> >
class natural_number : private Allocator::template rebind<T>::other {

    typedef typename Allocator::template rebind<T>::other allocator_type;
    typedef typename allocator_type::value_type digit_type;
    typedef typename allocator_type::pointer digit_ptr;
    typedef typename allocator_type::size_type size_type;

public:

    natural_number() : digit_array(0), digits(0), allocated(0) {}

    natural_number(unsigned long v) {
        if (v) {
            allocate(1);
            digits = 1;
            digit_array[0] = v;
        } else {
            digits = allocated = 0;
            digit_array = 0;
        }
    }

    natural_number(const natural_number& other) {
        if (this == &other) return;
        if (other.digits) {
            allocate(other.digits);
            digits = other.digits;
            LowLevel::copy_forward(digit_array, other.digit_array, other.digits);
        } else {
            digits = allocated = 0;
            digit_array = 0;
        }
    }

    natural_number(const char* s) : digits(0), allocated(0), digit_array(0) {
        while (*s) {
            multiply(*this, 10u);
            add(*this, static_cast<unsigned long>(*s - '0'));
            ++s;
        }        
    }

    natural_number& operator=(unsigned long v) {
        natural_number(v).swap(*this);
        return *this;
    }

    // http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
    natural_number& operator=(natural_number other) {
        swap(other);
        return *this;
    }

    natural_number& operator=(const char* s) {
        natural_number(s).swap(*this);
        return *this;
    }

    ~natural_number() {
        if (allocated) {
            allocator().deallocate(digit_array, allocated);
        }
    }

    void swap(natural_number& other) {
        std::swap(digit_array, other.digit_array);
        std::swap(digits, other.digits);
        std::swap(allocated, other.allocated);
    }

    bool is_zero() const {
        return digits == 0;
    }

    void add(const natural_number& a, const natural_number& b) {
        // these checks for zero are not necessary, but an optimization
        if (a.is_zero()) {
            *this = b;
        } else if (b.is_zero()) {
            *this = a;
        } else {
            size_type max_digits = std::max(a.digits, b.digits) + 1;
            if (allocated < max_digits) {
                natural_number other(max_digits, digit_unit);
                other.add_number(a, b);
                swap(other);
            } else {
                add_number(a, b);
            }
        }
    }

    void add(const natural_number& a, unsigned long b) {
        // these checks for zero are not necessary, but an optimization
        if (a.is_zero()) {
            *this = b;
        } else if (b == 0) {
            *this = a;
        } else {
            if (sizeof(unsigned long) <= sizeof(digit_type)) {
                size_type max_digits = a.digits + 1;
                if (allocated < max_digits) {
                    natural_number other(max_digits, digit_unit);
                    other.add_digit(a, b);
                    swap(other);
                } else {
                    add_digit(a, b);
                }
            } else {
                throw std::runtime_error("not implemented yet");
            }
        }
    }

    // result is undefined if a > b, but no crash
    void subtract(const natural_number& a, const natural_number& b) {
        size_type max_digits = a.digits;
        if (allocated < max_digits) {
            natural_number other(max_digits, digit_unit);
            other.subtract_number(a, b);
            swap(other);
        } else {
            subtract_number(a, b);
        }
    }

    void subtract(const natural_number& a, unsigned long b) {
        if (sizeof(unsigned long) <= sizeof(digit_type)) {
            size_type max_digits = a.digits;
            if (allocated < max_digits) {
                natural_number other(max_digits, digit_unit);
                other.subtract_digit(a, b);
                swap(other);
            } else {
                subtract_digit(a, b);
            }
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }

    void multiply(const natural_number& a, const natural_number& b) {
        if (a.is_zero() || b.is_zero()) {
            *this = 0lu;
        } else {
            size_type res_digits = a.digits + b.digits;
            if (allocated < res_digits || this == &a || this == &b) {
                natural_number other(res_digits, digit_unit);
                other.multiply_number(a, b);
                swap(other);
            } else {
                multiply_number(a, b);
            }
        }
    }

    void multiply(const natural_number& a, unsigned long b) {
        if (b == 0) {
            *this = 0lu;
        } else if (b == 1) {
            *this = a;
        } else if (sizeof(unsigned long) <= sizeof(digit_type)) {
            size_type res_digits = a.digits + 1;
            if (allocated < res_digits) {
                natural_number other(res_digits, digit_unit);
                other.multiply_digit(a, b);
                swap(other);
            } else {
                multiply_digit(a, b);
            }
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }

    void divide(const natural_number& a, const natural_number& b) {
        natural_number r(a.digits + 1);
        quotrem(*this, r, a, b);
    }

    inline void divide(const natural_number& a, unsigned long b) {
        quotrem(a, b);
    }

    void modulus(const natural_number& a, const natural_number& b) {
        natural_number q(a.digits - b.digits + 1);
        quotrem(q, *this, a, b);
    }

    unsigned long modulus(unsigned long b) const {
        natural_number q(digits, digit_unit);
        return q.quotrem(*this, b);
    }
    
    static void quotrem(natural_number& q, natural_number& r, const natural_number& a, const natural_number& b) {
        if (&q == &r)
            throw std::invalid_argument("q must be different from r");
        if (b.is_zero())
            throw std::overflow_error("division by zero");
        else if (a.digits < b.digits) {
            q = 0lu;
            r = a;
        } else if (&a == &b) {  // necessary?
            q = 1lu;
            r = 0lu;
        } else {
            unsigned shift = nlz(b.digit_array[b.digits-1]);
            size_type r_res_digits = a.digits + 1;  // note: auxiliary digit
            if (r.allocated >= r_res_digits && (&r != &b || shift != 0)) {
                quotrem_number_step1(q, r, a, b, shift);
            } else {
                natural_number rem(r_res_digits, digit_unit);
                quotrem_number_step1(q, rem, a, b, shift);
                rem.swap(r);
            }
        }    
    }
    
    unsigned long quotrem(const natural_number& a, unsigned long b) {
        if (!b)
            throw std::overflow_error("division by zero");
        else if (a.is_zero()) {
            *this = 0lu;
            return b;
        } else if (sizeof(unsigned long) <= sizeof(digit_type)) {
            digit_type denom = b, remainder;
            unsigned shift = nlz(denom);
            if (shift) {
                denom <<= shift;
                natural_number num(a.digits, digit_unit);
                num.digits = a.digits;
                digit_type top_digit = LowLevel::lshift(num.digit_array, a.digit_array, a.digits, shift);
                remainder = quotrem_digit_step1(top_digit, num, denom);
                remainder >>= shift;
            } else {
                remainder = quotrem_digit_step1(0, a, denom);
            }
            return remainder;
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }
    
    int compare(const natural_number& a) const {
        if (digits < a.digits) return -1;
        if (digits > a.digits) return 1;
        return LowLevel::comp(digit_array, a.digit_array, digits);
    }

    std::string str(std::streamsize /*digits*/, std::ios_base::fmtflags f) const {
        if (is_zero())
            return "0";
        unsigned max_decimals = (digits * kanooth::number_bits<digit_type>::value) / 3 + 1;
        char chars[max_decimals];
        char *end = chars + max_decimals;
        char *begin = end;
        natural_number n = *this;
        while (!n.is_zero()) {
            unsigned long d = n.quotrem(n, 10u);
            *--begin = d + '0';
        }
        return std::string(begin, end);
    }

private:

    enum size_request {
        digit_unit
    };
    
    natural_number(size_type min_size, size_request) : digits(0) {
        assert(min_size >= 1);
        allocate(min_size);
    }

    inline void set_digits_1(size_type max_digits) {
        assert(max_digits >= 1);
        digits = max_digits;
        if (!digit_array[digits-1])
            --digits;
    }

    inline void set_digits_n(size_type max_digits) {
        digits = max_digits;
        while (digits && !digit_array[digits-1])
            --digits;
    }

    inline void set_digits_carry(size_type base_digits, digit_type carry) {
        digits = base_digits;
        if (carry)
            digit_array[digits++] = carry;
    }

    void add_number(const natural_number& a, const natural_number& b) {
        if (a.digits >= b.digits) {
            digit_type carry = LowLevel::add(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
            set_digits_carry(a.digits, carry);
        } else {
            digit_type carry = LowLevel::add(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
            set_digits_carry(b.digits, carry);
        }
    }
    
    void add_digit(const natural_number& a, digit_type b) {
        digit_type carry = LowLevel::add_1(digit_array, a.digit_array, a.digits, b);
        set_digits_carry(a.digits, carry);
    }
    
    void subtract_number(const natural_number& a, const natural_number& b) {
        LowLevel::sub(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        set_digits_n(a.digits);
    }

    void subtract_digit(const natural_number& a, digit_type b) {
        LowLevel::sub_1(digit_array, a.digit_array, a.digits, b);
        set_digits_n(a.digits);
    }
    
    void multiply_number(const natural_number& a, const natural_number& b) {
        if (a.digits >= b.digits) {
            LowLevel::mul(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        } else {
            LowLevel::mul(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
        }
        set_digits_1(a.digits + b.digits);
    }
    
    void multiply_digit(const natural_number& a, digit_type b) {
        digit_type carry = LowLevel::mul_1(digit_array, a.digit_array, a.digits, b);
        set_digits_carry(a.digits, carry);
    }
    
    static void quotrem_number_step1(natural_number& q, natural_number& rem, const natural_number& a, const natural_number& b, unsigned shift) {
        assert(rem.allocated >= a.digits + 1);
        assert(&rem != &b || shift != 0);  // &rem == &b is ok when shift != 0 since a temporary instead of b will be used as denominator
        rem.digits = a.digits;
        if (shift) {
            natural_number denom(b.digits, digit_unit);
            rem.digit_array[a.digits] = LowLevel::lshift(rem.digit_array, a.digit_array, a.digits, shift);
            LowLevel::lshift(denom.digit_array, b.digit_array, b.digits, shift);
            denom.digits = b.digits;
            quotrem_number_step2(q, rem, denom, shift);
        } else {
            // the following will then do nothing if &rem == &a
            LowLevel::copy_forward(rem.digit_array, a.digit_array, a.digits);
            rem.digit_array[a.digits] = 0;
            quotrem_number_step2(q, rem, b, 0);
        }
    }

    static void quotrem_number_step2(natural_number& quot, natural_number& num, const natural_number& denom, unsigned shift) {
        assert(denom.digit_array[denom.digits-1] & (digit_type(1) << (kanooth::number_bits<digit_type>::value - 1)));  // denominator normalized
        assert(&num != &denom);
        size_type q_res_digits = num.digits - denom.digits + 1;
        if (quot.allocated < q_res_digits || &quot == &denom) {
            natural_number tmp_quot(q_res_digits, digit_unit);
            quotrem_number_step3(tmp_quot, num, denom, shift);
            tmp_quot.swap(quot);
        } else {
            quotrem_number_step3(quot, num, denom, shift);
        }
    }

    static void quotrem_number_step3(natural_number& q, natural_number& num, const natural_number& denom, unsigned shift) {
        LowLevel::quotrem(q.digit_array, num.digit_array, num.digits, denom.digit_array, denom.digits);
        q.set_digits_1(num.digits - denom.digits + 1);
        if (shift)
            LowLevel::rshift(num.digit_array, num.digit_array, denom.digits, shift);
        num.set_digits_n(denom.digits);  // num is now the remainder
    }
    
    digit_type quotrem_digit_step1(digit_type top_digit, const natural_number& num, digit_type denom) {
        assert(denom & (digit_type(1) << (kanooth::number_bits<digit_type>::value - 1)));  // denominator normalized
        size_type res_digits = num.digits;
        if (allocated < res_digits) {
            natural_number quot(res_digits, digit_unit);
            digit_type remainder = quot.quotrem_digit_step2(top_digit, num, denom);
            swap(quot);
            return remainder;
        } else
            return quotrem_digit_step2(top_digit, num, denom);
    }

    digit_type quotrem_digit_step2(digit_type top_digit, const natural_number& num, digit_type denom) {
        T remainder = LowLevel::quotrem_1(digit_array, top_digit, num.digit_array, num.digits, denom);
        set_digits_1(num.digits);
        return remainder;
    }
    
    void allocate(size_type size) {
        allocated = size;
        digit_array = allocator().allocate(allocated);
    }
    
    allocator_type& allocator() { return *this; }
    digit_ptr digit_array;
    size_type digits;
    size_type allocated;
};

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

